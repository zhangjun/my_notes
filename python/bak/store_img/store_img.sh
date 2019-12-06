#!/usr/bin/bash

cat ml_detect_trans.err.log.2018-11-07_* | grep statis | grep -vE "zh-CHS|en" > lang

python stat.py | awk '{print "cp ../../data/service/img/"$1".jpg ../../data/service/img/langs/"$2""}' | sh
