#!/usr/bin/env python

import json
import base64

file = "json"

def base64_img(data, img):
    f = open(img, "w")
    f.write(base64.b64decode(data))
    f.close()


img = "dst.jpg"

data = open(file, 'rb').read()
data = json.loads(data)
if 'pic' in data:
    base64_img(data['pic'], img)
