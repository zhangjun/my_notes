#!/usr/bin/env python

import json
import base64

file = "json"
file = "log/17log"

def base64_img(data, img):
    f = open(img, "w")
    f.write(base64.b64decode(data))
    f.close()



data = open(file, 'rb')

cnt = 0
for item in data:
	try:
		img = "17dst.jpg"
		data = item.split("{", 1)[1]
		data = json.loads('{' + data)
		img = "jpgs/"+str(cnt) + img
		if 'pic' in data:
		    base64_img(data['pic'], img)
		    cnt = cnt + 1
	except:
		continue
