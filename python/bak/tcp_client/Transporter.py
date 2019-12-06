#! /usr/bin/python

import sys
import socket
from struct import pack, unpack, calcsize
from sogou_log import sogou_log

RETRY_TIME = 1

class Transporter():
	def __init__(self, **args):
		if 'conn' in args:
			self.conn = args['conn']
			self.IP,self.port = self.conn.getpeername()
		elif 'IP' in args and 'port' in args:
			self.IP = args['IP']
			self.port = args['port']
			self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.conn.connect((self.IP, self.port))
		self.conn.settimeout(20)
		self.conn.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
		sogou_log("Transpotor: getsockopt: before socket.SO_SNDBUF %d" % self.conn.getsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF))
		self.conn.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 10*1024*1024)
		sogou_log("Transpotor: getsockopt: after socket.SO_SNDBUF %d" % self.conn.getsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF))
	
	def reconnect(self):
		sogou_log("reconnect\n")
		self.conn.close()
		self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			self.conn.connect((self.IP, self.port))
			self.conn.settimeout(20)
			self.conn.setsockopt(socket.IPPROTO_TCP, TCP_NODELAY, 1)
			self.conn.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 10*1024*1024)
			return True
		except:
			return False

	def sendMsg(self, s):
		head = pack('2I', len(s), 4)
		sogou_log("Transpotor: head.len: %d" % len(s))
		pad = 0
		if len(s) < 1000:
			pad = 1000 - len(s)
		padding = ''
		for i in xrange(pad):
			padding += pack('B', 0)
		#print "len(head): %d" % len(head)
		pk = s + padding
		plen = len(pk)
		try_time = RETRY_TIME
		while try_time >= 0:
			try:
				if try_time != RETRY_TIME:
					sogou_log("resend %d\n" % (RETRY_TIME-try_time))
				self.conn.send(head)
				slen = 0
				while slen < plen:
					slen += self.conn.send(pk[slen:])
				break
			except:
				if not self.reconnect():
					break
				try_time -= 1

	def recvMsg(self):
		head = self.conn.recv(calcsize('2I'))
		if len(head) != calcsize('2I'):
			return '',0
		size,flag = unpack('2I', head)
		#pad = 0
		#if size < 1000:
		#	pad = 1000 - size
		#rlen = size+pad
		#rlen = size < 1000 and 1000 or size
		recv_size = 1000 if size < 1000 else size
		#print "rlen:%d" % rlen
		buf = self.conn.recv(recv_size)
		while len(buf) < recv_size:
			buf += self.conn.recv(recv_size-len(buf))
		return buf[:size],size
		
