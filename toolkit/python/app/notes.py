#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-05-20 16:31:55
####################################################

import os

def getfiles(root):
    for dirpath, dirnames, filenames in os.walk(root):
        for dirname in dirnames[:]:
            if len(dirname)>1 and dirname[0] == ".":
                dirnames.remove(dirname)
        for filename in filenames:
            yield os.path.join(dirpath, filename)

def getdirs(root):
    for dirpath, dirnames, filenames in os.walk(root):
        for dirname in dirnames[:]:
            if len(dirname)>1 and dirname[0] == ".":
                dirnames.remove(dirname)
        for dirname in dirnames:
            yield os.path.join(dirpath, dirname)

def append2doc(dst, src):
    print("append %s to %s" % (src, dst))
    with open(src, "r") as rfd:
        with open(dst, 'a') as wfd:
            wfd.write(src + "\n")
            for line in rfd:
                wfd.write(line)
            wfd.write("\n" * 5)

def test():
    file_list = []
    for file in getfiles('.'):
        file_list.append(file)
    file_list.sort()
    # print(file_list)
    dst_file = "sum.txt"
    if os.path.isfile(dst_file):
        print("ERROR: %s already exists." % dst_file)
        return
    for file in file_list:
        append2doc(dst_file, file)

if __name__ == '__main__':
    test()

