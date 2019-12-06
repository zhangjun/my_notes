import sys
from datetime import datetime
import threading

#log format
log_lock = threading.Lock()
def sogou_log(log_str, reqid='', lock=False):
	if lock:
		log_lock.acquire()
	if reqid != '':
		print >>sys.stderr, '%s [%s] %s' % (datetime.now().strftime('[%Y%m%d %H:%M:%S]'), reqid, log_str)
	else:
		print >>sys.stderr, '%s %s' % (datetime.now().strftime('[%Y%m%d %H:%M:%S]'), log_str)
	if lock:
		log_lock.release()

def bin_out(buf):
    out_buf = []
    for i in buf:
        out_buf.append('%02x' % ord(i))
    sogou_log(" ".join(out_buf))

