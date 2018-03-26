#!/usr/bin/env python
# coding=utf-8

import json

def isJson(str):
	try:
		json.loads(str)
	except ValueError:
		return False
	return True

if __name__ == '__main__':
	
	file0 = 'test'
	file1 = 'mobile_query_69'
	fd0 = open(file0, 'w')
	fd1 = open(file1, 'w')

	data = open(file)

	cnt = 0
	for line in open(f ile):
		#line = line.strip()
		if isJson(line):
			fd1.write(line)
		else:
			fd0.write(line)
		cnt = cnt + 1
#		if cnt > 20:
#			break

	fd0.close()
	fd1.close()
	print cnt
