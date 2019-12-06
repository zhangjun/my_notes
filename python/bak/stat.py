#!/bin/bash/env python

file = 'bwidth_24'
id = [ str(i) for i in xrange(41)]
val = [ 0 for i in xrange(41)]
data = dict(zip(id, val))

print data

for line in open(file):
	line = line.strip().split(",")
	width = int(line[1].split(":")[1])
	data[str(width / 100)] += 1

for key, val in data.items():
	print key, val
