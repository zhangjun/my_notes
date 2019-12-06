
import sys
import json

fd = open('json_data')
content = fd.read()

data = json.loads(content)

count = 0
for i in range(len(data["trans_frag"])):
    count += len(data["trans_frag"][i]["text"])

print count
