#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import sys 
reload(sys)
sys.setdefaultencoding('utf-8')
import base64
import math
import time
import os
import datetime

import smtplib 
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
 
from email.utils import COMMASPACE,formatdate
from email import encoders

date = ''
temp = '<table class="pv_uv_table" border="1"><tr><td>数据项</td><td>数据量大小</td><td>百分比(相对于total)</tr>%s</table>'

def send_mail(sender, receiver, subject, text): 
    assert type(receiver) == list 
 
    msg = MIMEMultipart('related') 
    msg['From'] = sender 
    msg['Subject'] = subject 
    msg['To'] = COMMASPACE.join(receiver) 
    print msg['To']
    msg.attach(MIMEText(text, 'html', 'utf-8')) 

    try: 
    	smtp = smtplib.SMTP()
    	smtp.connect('abc.com')
    	smtp.sendmail(sender, receiver, msg.as_string()) 
	print 'send mail secuessfully'
    except smtplib.SMTPException:
	print 'Error:send failed'
    smtp.quit()

#def join_html(vertical):
#    html = '' 
#    table = ''
#
#    for key in vertical:
#        table = ''
#        title = """<tr><td colspan = "2">%s</td></tr>""" % (key)
#        for err_key in vertical[key]:
#            row = """<tr><td>%s</td><td>%s</td></tr>""" % (err_key,vertical[key][err_key])
#            table = table + row
#        html = html + temp % (title,table) + '<br>'
#    return html

def fetch_data(filename):
	data = {}
	for line in open(filename):
		linedata = line.strip().split('\t')
		if len(linedata) == 3:
			keywords = linedata[0]
			data[keywords] = []
			data[keywords].append(linedata[1])
			data[keywords].append(linedata[2])

	return data 

def get_info(data, *args):
	html = ''
	table = ''
	if len(args) == 0:
		for key in data:
			row = """<tr><td>%s</td><td>%s</td><td>%s</td></tr>""" % (key, data[key][0], data[key][1])
			table = table + row
	elif len(args) == 1:
		keywords = args[0]
		for key in data:
			if key.startswith(keywords) and len(key.split(',')) == 1:
				row = """<tr><td>%s</td><td>%s</td><td>%s</td></tr>""" % (key, data[key][0], data[key][1])
				table = table + row
			if key.startswith(keywords) and len(key.split(',')) == 2:
				row = """<tr><td>%s</td><td>%s</td><td>%s</td></tr>""" % (key.split(',')[1], data[key][0], data[key][1])
				table = table + row
	
	html = temp % (table) + '<br>'	
	return html

if __name__=='__main__':
    sender = 'abc.com'
    receiver = []
    receiver.append('zhangjun@abc.com')
    subject = '数据量统计 ' + time.strftime('%Y-%m-%d')
    text = 'mail content'
    date = time.strftime("%Y_%m_%d", time.localtime()) 
    fd = open('template.html')
    html_content = fd.read()
    datafile = 'datastat.20170116'
    alldata = fetch_data(datafile)
    fd.close()
    html = ''
    
    total = get_info(alldata, 'total')
    html = html + '总数:<br>' + total

    qi_data = get_info(alldata, 'QI')
    html = html + '等级(QI):<br>' + qi_data

    filter_data = get_info(alldata, 'FILTER')
    html = html + '各bit数据分布<br>' + filter_data

    lack_data = get_info(alldata, '缺失')
    html = html + '数据缺失情况<br>' + lack_data

    remain_data = get_info(alldata)
    html = html + '其他数据信息<br>' + remain_data

    html_content = html_content % (html)
    print html_content

    msg = MIMEMultipart('related') 
    msg['From'] = sender 
    msg['Subject'] = subject 
    msg['To'] = COMMASPACE.join(receiver) 
    print msg['To']
    msg.attach(MIMEText(text, 'html', 'utf-8')) 

    try: 
    	smtp = smtplib.SMTP()
    	smtp.connect('abc.com')
    	smtp.sendmail(sender, receiver, msg.as_string()) 
	print 'send mail secuessfully'
    except smtplib.SMTPException:
	print 'Error:send failed'
    smtp.quit()

    #send_mail(sender, receiver, subject, html_content)
    print time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()),"send mail"
