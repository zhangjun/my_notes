#!/usr/bin/env python
# coding=utf-8

import urllib.request
from bs4 import BeautifulSoup
import re
import os

def get_file(data):
    #file_list = re.findall(r'a href="(^[0-9a-zA-Z].[a-zA-Z])">', data)
    file_list = re.findall(r'a href="(^[?].+[a-zA-Z])">', data)
    return file_list

def parse(data):
    soup = BeautifulSoup(data, "lxml")
    table = soup.find("table")
    tr_data = table.findAll("tr")
    for item in tr_data:
        td_data = item.findAll("td")
        if len(td_data) < 2:
            continue
        #print(len(td_data))
        print(td_data[1].string)
        print(td_data[1].text)
        print(td_data[1].a['href'])

def download(url):
    f = urllib.request.urlopen(url)
    data = f.read().decode('utf-8')
    return data


if __name__ == '__main__':
    url = "ftp://127.0.0.1/index.html"
    data = download(url)
    #file_list = get_file(data)
    #print(file_list)
    parse(data)
