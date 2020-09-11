# -*- coding: utf-8 -*-
import scrapy

class MaoyanSpider(scrapy.Spider):
    name = "maoyan"
    allowed_domains = ['maoyan.com']
    start_urls = ['http://maoyan.com/board/7/']

    def parse(self, response):
        print("url: ", self.start_urls)
        pass
