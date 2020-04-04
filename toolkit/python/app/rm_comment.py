#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-03-09 23:51:24
####################################################

import os
import sys
import re

def rm_comment_styleS(filename, newfile=""):
    # // single line comment style
    if not os.path.isfile(filename):
        print("ERROR: %s: No such file.")
        return
    if not newfile:
        newfile = filename + ".tmp"
    # //[^\n]*
    sl_comment_style = re.compile(r"//[^\n]*")
    with open(newfile, "w") as nfd:
        with open(filename) as fd:
            for line in fd:
                line = sl_comment_style.sub("", line)
                nfd.write(line)
    print("{}->{}: remove so comment completed.".format(filename, newfile))

def rm_comment_styleM(filename, newfile=""):
    # /*  */ multiple line comment style
    if not os.path.isfile(filename):
        print("ERROR: %s: No such file.")
        return
    if not newfile:
        newfile = filename + ".tmp"
    # \/\*[^*]*\*+([^/*][^*]*\*+)*\/
    ml_comment_style = re.compile(r"\/\*[^*]*\*+([^/*][^*]*\*+)*\/")
    with open(newfile, "w") as nfd:
        with open(filename) as fd:
            lines = "".join(fd.readlines())
            lines = ml_comment_style.sub(' ', lines)
            nfd.write(lines)
    print("{}->{}: remove sm comment completed.".format(filename, newfile))

def rm_comment(filename="test.c"):
    rm_comment_styleS(filename)
    rm_comment_styleM(filename+".tmp", filename+".c")
    try:
        os.remove(filename+".tmp")
    except:
        pass

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit()
    rm_comment(sys.argv[1])


