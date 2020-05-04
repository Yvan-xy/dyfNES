#/usr/bin/env python3
import os, re
f = open("./opcode.h", "r+")

contain = f.readlines()

con=""
for i in contain:
    con = con + i
# print(con)
# exit()
def parse(matched):
    value = str(matched.group())[:-2]
    # print(value)
    return value+'", '+value+","
    # return ""
conn = re.sub(r'\w{3}",', parse, con)
result=""
for i in conn: 
    result += i
    
f.write(result)
# print(result)