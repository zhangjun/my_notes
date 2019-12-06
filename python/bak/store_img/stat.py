#!/usr/bin/env python

file = "lang"

for line in open(file):
	line = line.strip()
	try:
		pic_name = line.split("reqid=")[1].split("||")[0]
		lang = line.split("lang': ['")[1].split("'")[0]
		print pic_name, lang
	except:
		continue
