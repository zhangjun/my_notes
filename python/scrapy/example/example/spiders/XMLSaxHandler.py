import xml.sax
import logging,time
#import tools
from scrapy.http import Request

class MyGeneralHandler(xml.sax.handler.ContentHandler):
    def __init__(self,cate,sourceurl,nofilter):
        #final return results
        self.items = []
        self.item = {}          #public info for on item
        self.item_details = []  #tvshow teleplay cartoon has detail info, details for on item
        self.item_detail =  {}  #tvshow teleplay cartoon has detail info
        self.cate = cate
        self.nofilter = nofilter
        if self.cate == '5':
            self.op = 'del'#'add' or 'del', type of interface
        else:
            self.op = 'add'
        self.is_in_detail = False
        self.current_tag = ''
        self.current_content = ''
        self.is_sitemap = False
        self.sourceurl = sourceurl
    def startDocument(self):
        pass
    def endDocument(self):
        pass
    def startElement(self, tag, attrs):
        self.current_tag = tag
        self.current_content = ''
        if tag == 'sitemapindex':
            self.is_sitemap = True
            return
        #start for item
        if tag == 'url':
            self.item = {'__cate':self.cate,'__sourceurl':self.sourceurl,'__startTime':time.time(),'__nofilter':self.nofilter}
            self.item_details = []
        #start for item_detail 
        elif tag == 'detail':
            self.is_in_detail = True
            self.item_detail = {}

    def endElement(self, tag):
        #logging.info("[sax endElement] recevie tag:%s,content:%s,current_tag:%s,from:%s" % (tag,self.current_content,self.current_tag,self.sourceurl)) 
        if self.current_tag != tag:#not leafnode
            #end for item
            if tag == 'url':
                if self.item_details:
                    for item_detail in self.item_details:
                        _ = self.item.copy()
                        _.update(item_detail)
                        self.items.append(_) 
                else:
                    self.items.append(self.item)
                self.item = {}
            #end for item_detail
            elif tag == 'detail':
                self.is_in_detail = False
                self.item_details.append(self.item_detail)
                #print str(self.item_detail)
        else:#leaf node
            # is sitemap
            if self.is_sitemap:
                if self.current_tag == 'loc':
                    url = self.current_content 
                    logging.info("[sax endElement] send seeds:%s,from:%s" % (url,self.sourceurl)) 
                    self.items.append(Request(tools.add_timestamp(url),meta={'cate':self.cate,'type':'xml'}))
            # detail html
            else:
                if self.current_tag in ('singleIntroduction','introduction'):
                   self.current_content = self.current_content.replace('\r\n','') 
                   self.current_content = self.current_content.replace('\n','') 
                if self.is_in_detail:
                    self.item_detail[self.current_tag] = self.current_content 
                else:
                    #//allow muti tag, connect with ;
                    if (self.current_tag == 'name' or self.current_tag == 'role') and self.item.get(self.current_tag):
                        self.item[self.current_tag] = self.item[self.current_tag] + ";" + self.current_content    
                    else:
                        self.item[self.current_tag] = self.current_content

        # father tag not content anything
        self.current_tag = ''
        self.current_content = ''
            
    def characters(self, content):
        if self.current_tag == '':
            return
        content = content.strip()
        if content == '':
            return
        #logging.info("[sax characters] recevie content:%s,tag:%s,from:%s" % (content,self.current_tag,self.sourceurl)) 
        self.current_content = self.current_content + content
