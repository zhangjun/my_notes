
now=`date +"%F_%H%M%S"`
nohup scrapy crawl open_delta -a xml=$seedsFile  &> log/crawl.${now}.log &
