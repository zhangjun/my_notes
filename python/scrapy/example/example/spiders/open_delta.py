# -*- coding: utf-8 -*-
from scrapy.spiders import Spider
from scrapy.http import Request
#from memcache import Client
import hashlib,requests
import time,logging,re

import sys
sys.path.append(".")
from XMLSaxHandler import MyGeneralHandler
import xml.sax
import tools
from datetime import datetime

def oneline(s):
    return ' '.join(s.split())

def str2md5(s):
    md5 = hashlib.md5()
    md5.update(s)
    return md5.hexdigest()

class XmlSpider(Spider):
    name = 'open_delta'
    allowed_domains = []
    def read_seeds(self,file,type):
        seeds=[]
        ok = 0
        error = 0
        try:
            for line in open(file,"r"):
                self.logger.info("[read_seeds] handle line:" + line)
                parts =  line.split()
                if len(parts)!=2:
                    self.logger.error("[read_seeds] parts error line:%s" % line)
                    error = error + 1
                    continue
                url = parts[0].strip()
                cate = parts[1].strip()
                if cate not in ('1','2','3','4','5','6','7','9'):
                    self.logger.error("[read_seeds] cate error line:%s" % line)
                    error = error + 1
                    continue
                ok = ok  + 1
                seeds.append((url,cate,type))
        except:
            return False
        self.logger.info("[read_seeds] finish, ok:%d,error:%d,file:%s" % (ok,error,file))
        return seeds

    def __init__(self,*args,**kargs):
        super(XmlSpider, self).__init__(*args,**kargs)
        xmlfile = kargs.get("xml","")
        indexfile = kargs.get("index","")
        self.nofilter = (kargs.get("nofilter","") == "true")
        self.start_urls = []
        if not xmlfile and not indexfile:
            raise ValueError("Seeds file error: Must specify seeds file! exam: scrapy crawl **spider -a xml=seeds.txt -a index=seeds2.txt [-a nofilter=true or false], xml and index at least has 1\n\t\tfile format:url\tcate")
        if xmlfile:
            seeds = self.read_seeds(xmlfile,"xml") 
            if seeds == False:
               raise ValueError("read file error:" + xmlfile) 
            self.start_urls.extend(seeds) 
        if indexfile:
            seeds = self.read_seeds(indexfile,"index") 
            if seeds == False:
               raise ValueError("read file error:" + indexfile) 
            self.start_urls.extend(seeds) 
        if not self.start_urls:
            raise ValueError("Empty seeds")
        self.logger.info("nofilter:%s,start urls:%s" % (self.nofilter,self.start_urls))
        #self.mc = Client(['127.0.0.1:11211'])

    def start_requests(self):
        print(self.start_urls)
        for (url,cate,type) in self.start_urls:
            yield Request(tools.add_timestamp(url),meta={'cate':cate,'type':type})

    def extract_text(self, item, field_name):
        tmp  = item.xpath('%(field_name)s/text()' % locals())
        if tmp:
            value = tmp[0].extract().strip()
            return oneline(value)

    def extract_join_text(self, item, field_name):
        L = []
        for tmp in item.xpath('%(field_name)s/text()' % locals()).extract():
            tmp = tmp.strip()
            if tmp:
                L.append(tmp)
        if L:
            return oneline(u';'.join(L))

    def is_response_changed(self, response):
        #if 'mgtv' in response.url:
        #    return True
        #if tools.del_timestamp(response.url) == "http://www.mgtv.com/xml/sogou/pay/dianshiju_add.xml":
        #    return True
        if self.nofilter:
            self.logger.info("[is_response_changed] nofilter = True, force response changed, url:%s" % (response.url))
            return True
        body_md5 = str2md5(response.body)
        #self.logger.info("[is_response_changed] url:%s, body: %s, new_size: %s" % (tools.del_timestamp(response.url), response.body, len(response.body)))
        self.logger.info("[is_response_changed] url:%s, new_size: %s" % (tools.del_timestamp(response.url), len(response.body)))
        #old_body_md5 = self.mc.get(tools.del_timestamp(response.url))
        #if old_body_md5 == body_md5:
        #    self.logger.info("[is_response_changed] url:%s,same md5:%s,size:%s" % (tools.del_timestamp(response.url),body_md5,len(response.body)))
        #    return False
        #self.mc.set(tools.del_timestamp(response.url), body_md5)
        #self.logger.info("[is_response_changed] url:%s,diff md5, new:%s,old:%s, new_size:%s" % (tools.del_timestamp(response.url),body_md5,str(old_body_md5), len(response.body)))
        return True

    def parse(self, response):
        #print type(response.body)
        t0 = time.time()
        urltype = response.request.meta["type"]
        if urltype == 'xml':
            #need judge whether the whole file changed
            if not self.is_response_changed(response):
                dt = (time.time() - t0) * 1000
                self.logger.info('[parse] [%s] [not change] %s dt=%.3f ms' % (response.url,urltype, dt))
                return
        #bring self.nofilter to pipelines
        handler = MyGeneralHandler(response.request.meta['cate'],response.url,self.nofilter)
        try:
            if response.encoding == 'utf-8':
                xml.sax.parseString(response.body,handler)
            else:
                xml.sax.parseString(response.body.decode(response.encoding,'ignore').encode("utf-8").replace("gbk","utf-8").replace("gb18030","utf-8"),handler)
        except Exception as e:
            logging.error("[PARSE EXCEPTION][sax first] url:|%s|,error:|%s|,encoding:%s,size:|||%s" %(response.url,e,response.encoding,len(response.body)))
            if 'mgtv'  in response.url:
                try:
                    handler = MyGeneralHandler(response.request.meta['cate'],response.url,self.nofilter)
                    r = requests.get(response.url)
                    xml.sax.parseString(r.content,handler)
                except Exception as e:
                    logging.error("[PARSE EXCEPTION][sax second] url:|%s|,error:|%s|,encoding:%s,content:|||%s" %(response.url,e,response.encoding,len(r.content)))
    
        dt = (time.time() - t0) * 1000
        if handler.items and not handler.items[0]:
            self.logger.info('[parse] [%s] type:%s,find NONE,%s' % (response.url, str(handler.items)))
        self.logger.info('[open_delta] [parse] [%s] type:%s,item_num:%s,cost:%d' % (response.url, urltype,len(handler.items), dt))
        return handler.items 

    def parse_old(self, response):
        t0 = time.time()
        items = []
        # extract returns unicode
        for loc in response.xpath('//sitemap/loc/text()').extract():
            loc = loc.strip()
            new_task = Request(loc)
            #self.logger.info('#new_task: ' + repr(new_task))
            items.append(new_task)

        if not self.is_response_changed(response):
            dt = (time.time() - t0) * 1000
            self.logger.info('#parse: [cached] %s %s dt=%.3f ms' % (response.url, len(items), dt))
            return items

        for item in response.xpath('/urlset/url'):
            D_url = {
            }
            names = ['workName','director','region','type','showTime','hot','score','language','introduction','definition','totalnumber','seasonId','serialName','otherName','source_type','status','nowEpisode']
            for name in names:
                v = self.extract_text(item, name)
                if v:
                    D_url[name] = v
            names_map = {
                'update_info' : 'update',
                'site_updateTime' : 'updateTime',
                'poster' : 'imageLink',
                'setAddress' : 'listLink',
                'horizontalPoster' : 'imageLink2',
            }
            for name, field in names_map.items():
                v = self.extract_text(item, field)
                if v:
                    D_url[name] = v
            v = self.extract_text(item, 'introduction')
            if v:
                D_url['introduction'] = self.extract_text(item, 'introduction')
            v = self.extract_join_text(item, 'starrings/starring/name')
            if v:
                D_url['starring'] = v
            v = self.extract_join_text(item, 'starrings/starring/role')
            if v:
                D_url['role'] = v
            for detail in item.xpath('detail'):
                D_detail = D_url.copy()
                detail_names = ['seq','singleTitle','singleLink','mplayLink','singleIntroduction','swfUrl','resourceTime','singleThumbnails','captionAvailable','vip']
                for name in detail_names:
                    v = self.extract_text(detail, name)
                    if v:
                        D_detail[name] = v
                v = self.extract_text(detail, 'length')
                if v:
                    D_detail['timeOfSingle'] = v
                v = self.extract_text(item, 'singleIntroduction')
                if v:
                    D_detail['singleIntroduction'] = v
                # url of the doc
                D_detail['__url__'] = D_detail['singleLink']
                D_detail['__collection__'] = 'teleplay_open'
                items.append(D_detail)
        dt = (time.time() - t0) * 1000
        self.logger.info('#parse: %s %s dt=%.3f ms' % (response.url, len(items), dt))
        return items

