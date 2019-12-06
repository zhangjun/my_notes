#!/usr/bin/env python

import socket
import struct import pack, unpack, calcsize
from sogou_log import sogou_log

def sendMsg(sock, s):
    head = pack('2I', len(s), 4)
    sogou_log("sendMsg head.len: %d" % len(s))
    pad = 0
    if len(s) < 1000:
        pad = 1000 - len(s)
    
    padding = ''
    for i in xrange(pad):
        padding += pack('B', 0)

    pkg = s + padding
    plen = len(pkg)

    try:
        sock.sendall(head)
        slen = 0
        sock.sendall(pkg)
        #while slen < plen:
        #    slen += sock.send(pkg[slen:])
    except:
        pass
    
def recvMsg(sock):
    head = sock.recv(calcsize('2I'))
    if len(head) != calcsize('2I'):
        return '', 0

    size, flag = unpack('2I', head)
    recv_size = 1000 if size < 1000 else size

    buf = sock.recv(recv_size)
    while len(buf) < recv_size:
        buf += sock.recv(recv_size - len(buf))

    return buf[:size], size
