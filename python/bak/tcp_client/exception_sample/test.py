#!/usr/bin/env python

from exceptions import MemcacheError

data = {}
data["id"] = 3

if data["name"] == "dd":
    print "dd"

raise MemcacheError("sample error")
