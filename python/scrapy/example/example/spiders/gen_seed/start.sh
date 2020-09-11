{
    flock -n 3
    if [ $? -eq 1 ] ;then
    { echo "[`date +"%F %H:%M:%S"`] open_delta start failed!"; exit; }
    fi

    if [ ! -d tmp ]
    then
        mkdir tmp
    fi
    if [ ! -d log/history ]
    then
        mkdir log/history
    fi
    rm -f tmp/*
    #caculate line number
    echo "[`date +"%F %H:%M:%S"`] open_delta start...";
    now=`date +"%F_%H%M%S"`
    fileNum=20
    seedsFilePrefix=tmp/xml.seeds.inc_
    indexFilePrefix=tmp/xml.seeds.index.inc_

    php parseIndex.php xml.seeds.index.inc xml.seeds.inc xml.seeds.inc.out &>log.parseIndex
    
    mv log/log* log/history/

    php split_file.php xml.seeds.inc.out $seedsFilePrefix $fileNum
    echo "[`date +"%F %H:%M:%S"`] php split_file.php xml.seeds.inc.out $seedsFilePrefix $fileNum"
    sed -i 's#https#http#g' `grep 'https' $seedsFilePrefix* -rl`
    #php split_file.php xml.seeds.index.inc $indexFilePrefix $fileNum
    #echo "[`date +"%F %H:%M:%S"`] php split_file.php xml.seeds.index.inc $indexFilePrefix $fileNum"

    #seedsLine=`wc -l xml.seeds.inc|awk -v fileNum=$fileNum '{print "scale=0;"$1"/"fileNum}'|bc`
    #indexLine=`wc -l xml.seeds.index.inc|awk -v fileNum=$fileNum '{print "scale=0;"$1"/"fileNum}'|bc`
    #split -l $seedsLine -d xml.seeds.inc $seedsFilePrefix
    #split -l $indexLine -d xml.seeds.index.inc $indexFilePrefix
    
    for num in `seq -w 0 99`
    do
        seedsFile=${seedsFilePrefix}${num}
        indexFile=${indexFilePrefix}${num}
        if [ -f $seedsFile -a -f $indexFile ]
        then
            nohup scrapy crawl open_delta -a xml=$seedsFile -a index=$indexFile &>log/log.open_delta_$num.$now &
            echo "[`date +"%F %H:%M:%S"`] scrapy crawl open_delta -a xml=$seedsFile -a index=$indexFile &>log/log.open_delta_$num.$now"
        elif [ -f $seedsFile ]
        then
            nohup scrapy crawl open_delta -a xml=$seedsFile  &>log/log.open_delta_$num.$now &
            echo "[`date +"%F %H:%M:%S"`] scrapy crawl open_delta -a xml=$seedsFile  &>log/log.open_delta_$num.$now"
        elif [ -f $indexFile ]
        then
            nohup scrapy crawl open_delta -a index=$indexFile &>log/log.open_delta_$num.$now & 
            echo "[`date +"%F %H:%M:%S"`] scrapy crawl open_delta -a index=$indexFile &>log/log.open_delta_$num.$now"
        else
            break
        fi
    done
    wait
    echo "[`date +"%F %H:%M:%S"`] open_delta finish ok!";
} 3<>/tmp/lockfile_open_delta
    
    #nohup scrapy crawl open_delta -a index=test.seeds.index.inc -a nofilter=true  &>log.add1 &
    #nohup scrapy crawl open_delta -a index=add.index   &>log.add &
    #nohup scrapy crawl open_delta -a xml=add.detail &>log.add &
    #nohup scrapy crawl open_delta -a xml=add.detail -a index=add.index -a nofilter=true &>log.add &

    #nohup scrapy crawl open_test -a xml=test.seeds.inc  &>log.test &
