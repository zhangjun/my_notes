#!/usr/bin/env python

import urllib
import urllib2
import time

import requests

url = "http://api.image.sogou/v1/face_detect.json"

youdao_url = "http://aidemo.youdao.com/ocrapi"
ms_url = "https://eastasia.api.cognitive.microsoft.com/vision/v2.0/ocr"

#youdao_url = "https://aidemo.youdao.com/ocrtransapi"
#youdao_url = "https://aidemo.youdao.com/ocrtransapi1"
param_data = {"imgBase":"xxx","company":"","lang":"zh-en"}
param_data = {"company":"","lang":"","imgUrl":"http://ai.youdao.com/images/p-fanyi/ocrtrans3.jpeg"}
param_data = {"company":"","lang":"","imgUrl":"https://img01.sogoucdn.com/net/a/04/link?appid=100520145&url=http%3A%2F%2Fimg02.sogoucdn.com%2Fapp%2Fa%2F100520146%2F16A029FD2CE86A564FB918BB599A002D"}

# https://aidemo.youdao.com/trans
# param_data = {"from":"zh-CHS","to":"","q":"test"}

headers = {"Content-Type":"application/x-www-form-urlencoded"}

# microsoft
image_url = "http://ai.youdao.com/images/p-fanyi/ocrtrans3.jpeg"
headers = {'Ocp-Apim-Subscription-Key': '2af7edd4-8587-42fc-9970-4c18f90985f0' }
params  = {'language': 'unk', 'detectOrientation': 'true'}
data = {'url': image_url}

to = "en"
#param_data = {"from":"zh-CHS","to":to}
#param_data = {"lang":"zh-en","imgUrl":"http://aidemo.youdao.com/images/demo/d_3.png"}

start = time.time()
params = urllib.urlencode(param_data)
print params
#r = requests.post(url, data = payload, header = headers)
#r = requests.post(url, data = params, header = headers)
#r = urllib.urlopen(url, params)
r = urllib.urlopen(ms_url, params)
print r.read()
end = time.time()
cost = float ((end - start) * 1000)
print "cost: %f ms" % cost



# get
# r = urllib.urlopen("%s?%s" % (url, params))

#req = urllib2.Request(url, params, headers)
#response = urllib2.urlopen(req)
#print response.read()
#
#def Post(url, params):
#	headers = {"Content-Type":"application/x-www-form-urlencoded"}
#	req = urllib2.Request(url, params, headers)
#	response = urllib2.urlopen(req)
#	return response.read()
