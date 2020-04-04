#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-11-20 10:30:46
####################################################

import re
def gen_enum(filename, obj="java"):
    fd = open(filename)
    linenum = 1
    # replace xxxxxx, to xxxxxx(%d),
    re_pattern = "(\(\d*\))?,"
    if obj == "java": repl_format = "(%d),"
    else: repl_format = " = %d,"
    comment_format = "// " +obj+ " enum define %s"
    print("\n"+comment_format % "begin")
    for line in fd:
        if ',' in line:
            line = re.sub(re_pattern, repl_format % linenum, line)
            linenum += 1
        print("%s"%line.strip())
    print(comment_format % "end")


filename = "test.txt"
gen_enum(filename)
gen_enum(filename, "cpp")

