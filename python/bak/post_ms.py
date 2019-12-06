#!/usr/bin/env python

import urllib
import urllib2
import time
import json

import requests

url = "http://api.image.sogou/v1/face_detect.json"

youdao_url = "http://aidemo.youdao.com/ocrapi"
ms_url = "https://southeastasia.api.cognitive.microsoft.com/vision/v2.0/ocr"
ms_url = "https://westcentralus.api.cognitive.microsoft.com/vision/v2.0/recognizeText"

#youdao_url = "https://aidemo.youdao.com/ocrtransapi"
#youdao_url = "https://aidemo.youdao.com/ocrtransapi1"
param_data = {"imgBase":"xxx","company":"","lang":"zh-en"}
param_data = {"company":"","lang":"","imgUrl":"http://ai.youdao.com/images/p-fanyi/ocrtrans3.jpeg"}
param_data = {"company":"","lang":"","imgUrl":"https://img01.sogoucdn.com/net/a/04/link?appid=100520145&url=http%3A%2F%2Fimg02.sogoucdn.com%2Fapp%2Fa%2F100520146%2F16A029FD2CE86A564FB918BB599A002D"}

# https://aidemo.youdao.com/trans
# param_data = {"from":"zh-CHS","to":"","q":"test"}

headers = {"Content-Type":"application/x-www-form-urlencoded"}

# microsoft
#image_url = "http://ai.youdao.com/images/p-fanyi/ocrtrans3.jpeg"
image_url = "https://img01.sogoucdn.com/net/a/04/link?appid=100520145&url=http%3A%2F%2Fimg02.sogoucdn.com%2Fapp%2Fa%2F100520146%2F624C0FC7AA4402C6729EF733CD76BC65"
headers = {'Content-Type':'application/json','Ocp-Apim-Subscription-Key': '685da00a145844eb8079542c7171ad46' }
#params  = {'language': 'zh-Hans', 'detectOrientation': 'true', 'mode': 'Handwritten'}
params  = {'language': 'zh-Hans', 'detectOrientation': 'true', 'mode': 'Handwritten'}

data = {'url': image_url}

to = "en"
#param_data = {"from":"zh-CHS","to":to}
#param_data = {"lang":"zh-en","imgUrl":"http://aidemo.youdao.com/images/demo/d_3.png"}

start = time.time()
#print params
#r = requests.post(ms_url, headers = headers, json = data)
r = requests.post(ms_url, params = urllib.urlencode(params), headers = headers, json = data)
#r = requests.post(url, data = params, header = headers)
#r = urllib.urlopen(url, params)
print r.text
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
