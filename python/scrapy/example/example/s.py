# -*- coding: utf-8 -*-

# Scrapy settings for opendata project
#
# For simplicity, this file contains only settings considered important or
# commonly used. You can find more settings consulting the documentation:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#     http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html
#     http://scrapy.readthedocs.org/en/latest/topics/spider-middleware.html

BOT_NAME = 'vsogou'

SPIDER_MODULES = ['opendata.spiders']
NEWSPIDER_MODULE = 'opendata.spiders'
# Enable and configure HTTP caching (disabled by default)
# See http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html#httpcache-middleware-settings
HTTPCACHE_ENABLED=False
HTTPCACHE_DIR='/search/odin/daemon/scrapy/cache/'
HTTPCACHE_POLICY='scrapy.extensions.httpcache.RFC2616Policy'
#HTTPCACHE_EXPIRATION_SECS=0
#HTTPCACHE_IGNORE_HTTP_CODES=[]
#HTTPCACHE_STORAGE='scrapy.extensions.httpcache.FilesystemCacheStorage'
#FEED_URI='file:///tmp/export.xml'
#FEED_FORMAT='xml'

ITEM_PIPELINES = {
    'opendata.pipelines.ItemFormatPipeline':1,
    'opendata.pipelines.ItemDupFilterPipeline':2,
    'opendata.pipelines.MysqlPipeline':3,
#    'opendata.pipelines.KafKaPipeline':4,
}
#KAFKA_HOSTS=['q01.video.zw.ted:9092','q02.video.zw.ted:9092','q03.video.zw.ted:9092']
LOG_LEVEL='INFO'
#LOG_LEVEL='DEBUG'
#MYSQL_CONF={'host':'10.141.48.126','user':'weiyuedu','passwd':'weiyuedu','db':'weiyuedu','charset':'utf8'}
MYSQL_CONF={'host':'mysql01.video.rds.sogou','user':'chanpinyunying','passwd':'1234m6i1m2a3','db':'video_film','charset':'utf8'}
MC_URI=['127.0.0.1:11211']


# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'vsogou (+http://v.sogou.com)'
USER_AGENT = 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.76 Mobile Safari/537.36'
CONCURRENT_REQUESTS = 100
COOKIES_ENABLED = False
RETRY_ENABLED = False
DOWNLOAD_TIMEOUT = 30
REACTOR_THREADPOOL_MAXSIZE = 20

# Configure maximum concurrent requests performed by Scrapy (default: 16)
#CONCURRENT_REQUESTS=32

# Configure a delay for requests for the same website (default: 0)
# See http://scrapy.readthedocs.org/en/latest/topics/settings.html#download-delay
# See also autothrottle settings and docs
DOWNLOAD_DELAY=0
# The download delay setting will honor only one of:
#CONCURRENT_REQUESTS_PER_DOMAIN=16
#CONCURRENT_REQUESTS_PER_IP=16

# Disable cookies (enabled by default)
#COOKIES_ENABLED=False

# Disable Telnet Console (enabled by default)
#TELNETCONSOLE_ENABLED=False

# Override the default request headers:
#DEFAULT_REQUEST_HEADERS = {
#   'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
#   'Accept-Language': 'en',
#}

# Enable or disable spider middlewares
# See http://scrapy.readthedocs.org/en/latest/topics/spider-middleware.html
SPIDER_MIDDLEWARES = {
#    'opendata.middlewares.MyCustomSpiderMiddleware': 543,
    'scrapy.spidermiddlewares.offsite.OffsiteMiddleware': None,
}

# Enable or disable downloader middlewares
# See http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html
#DOWNLOADER_MIDDLEWARES = {
#    'opendata.middlewares.MyCustomDownloaderMiddleware': 543,
#    'scrapy.downloadermiddlewares.httpcache.HttpCacheMiddleware' : 543,
#}


# Enable or disable extensions
# See http://scrapy.readthedocs.org/en/latest/topics/extensions.html
#EXTENSIONS = {
#    'scrapy.telnet.TelnetConsole': None,
#}

# Configure item pipelines
# See http://scrapy.readthedocs.org/en/latest/topics/item-pipeline.html
#ITEM_PIPELINES = {
#    'opendata.pipelines.SomePipeline': 300,
#}

# Enable and configure the AutoThrottle extension (disabled by default)
# See http://doc.scrapy.org/en/latest/topics/autothrottle.html
# NOTE: AutoThrottle will honour the standard settings for concurrency and delay
#AUTOTHROTTLE_ENABLED=True
# The initial download delay
#AUTOTHROTTLE_START_DELAY=5
# The maximum download delay to be set in case of high latencies
#AUTOTHROTTLE_MAX_DELAY=60
# Enable showing throttling stats for every response received:
#AUTOTHROTTLE_DEBUG=False

