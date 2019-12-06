#!/usr/bin/env python2 


import logging

#logger = logging.getLogger(__name__)
logger = logging.getLogger("client_example")

LOG_FORMAT = "%(asctime)s [%(name)s][%(levelname)s] %(message)s"
DATE_FORMAT = "%Y%m%d %H:%M:%S"


logging.basicConfig(level = logging.DEBUG, filename='mylog.log', format=LOG_FORMAT, datefmt=DATE_FORMAT)

logging.debug("long example")
logging.debug("long example")
logging.debug("long example")
print "data"
