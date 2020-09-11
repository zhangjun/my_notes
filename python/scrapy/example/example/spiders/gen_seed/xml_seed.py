#!/usr/bin/env python

import sys
#import os
import requests
import urllib
import urllib2
from lxml import etree

#seed_file = sys.argv[2]


## requests
#def http_get(url):
#    data = requests.get(url)
#    print data
#    return data

# urllib
def http_get(url):
    try:
        req = urllib2.Request(url)
        data = urllib2.urlopen(req, timeout = 5)
        result = data.read()
    except urllib2.URLError, e:
        print "timeout"
        return ''
    return result

def parse_site(url):
    content = http_get(url)
    print content
    if content == '':
        return
    
    # from file
    #tree = etree.parse(content)
    try:
        tree = etree.XML(content)
        key_field = ['loc']
        if len(tree.nsmap) > 0:
            ns = tree.nsmap[None]
            key_field.append("{%s}loc" % ns)

        tree = etree.ElementTree(tree)
        for element in tree.iter(key_field):
        #for element in tree.iter():
            #print element.tag, element.text
            print "loc: ", element.text.strip()
    except Exception, e:
        print "parse fail, ", e
        return
    #print tree.sitemapindex
    #root = tree.getroot()
    #for url in root:
    #    print url.type
    #return content

def crawl(file):
    for line in open(sitemap_file):
        ori = line.split()[0]
        dtype = line.split()[1]
        parse_site(ori)
        print ori, dtype


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "usage: ", sys.argv[0], " seed_file"
        sys.exit(0)

    sitemap_file = sys.argv[1]
    crawl(sitemap_file)
