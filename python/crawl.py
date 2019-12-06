#!/usr/bin/env python
# coding=utf-8

import urllib.request
from bs4 import BeautifulSoup
import re
import os
import sys

def get_file(data):
    #file_list = re.findall(r'a href="(^[0-9a-zA-Z].[a-zA-Z])">', data)
    file_list = re.findall(r'a href="(^[?].+[a-zA-Z])">', data)
    return file_list

def parse(data):
    soup = BeautifulSoup(data, "lxml")
    table = soup.find("pre")
    a_data = table.findAll("a")
    print(a_data[0].a['href'])
    #for item in tr_data:
    #    td_data = item.findAll("td")
    #    if len(td_data) < 2:
    #        continue
    #    #print(len(td_data))
    #    #print(td_data[1].string)
    #    #print(td_data[1].text)
    #    print(td_data[1].a['href'])

def download(url):
    f = urllib.request.urlopen(url)
    #data = f.read().decode('utf-8')
    data = f.read()
    return data

def crawl(url, path):
    if not os.path.isdir(path):
        os.makedirs(path)

    data = download(url)
    soup = BeautifulSoup(data, "lxml")
    content = soup.find("pre")
    a_content = content.findAll("a")

    print(len(a_content))
    for item in a_content:
        name = item["href"]
        if "memdb1" in name or "resin" in name or "sogou-pack-" in name or "rpm" in name:
            file_url = "%s/%s" %(url, name)
            print(file_url)
            remote_data = download(file_url)
            fd = open("%s/%s" % (path, name), "wb")
            fd.write(remote_data)
            fd.close()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("usage: %s url dir" % sys.argv[0])
        sys.exit()

    url = "http://mirrors.sogou/sogou/7/x86_64/op/"
    url = sys.argv[1]
    dirpath = sys.argv[2]
    #url = "http://el7.dailybuild.sogou-inc.com/rev/web_suggestion/wap_gosugg.ini/HEAD/main/last/source/tools/Tester/"
    #data = download(url)
    #file_list = get_file(data)
    #print(file_list)
    #parse(data)
    crawl(url, dirpath)
