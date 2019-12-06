#!/usr/bin/env python

import sys
import requests
import json

filename = "ocr_json" 
filename = "ocr_res" 
#data = open(filename, 'rb').read()
data = open(filename).read()


val = json.loads(str(data))
print(val["success"])
#for i in range(len(val["result"])):
#    del val["result"][i]["background"]
#    val["result"][i]["trans_content"] = "sanwish"

url = "http://10.141.178.160:9098/sg_json_test_test"
#r = requests.post(url, data = data)
print(json.dumps(val))
r = requests.post(url, data = json.dumps(val))
print(r.text)

