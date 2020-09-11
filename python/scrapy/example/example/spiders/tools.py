import re,time

def add_timestamp(url):
    if '?' in url:
        return url
    else:
        return url+"?"+str(int(time.time()))
def del_timestamp(url):
    if '?' in url:
        try:
            return re.match(r"(.*)\?\d+",url).group(1) 
        except:
            return url
    else:
        return url

