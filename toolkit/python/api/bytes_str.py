#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-03-24 18:09:56
####################################################

import sys

#在Python3中，我们需要编写接受str或bytes，并总是返回str的方法：
def to_str(bytes_or_str):
    if isinstance(bytes_or_str, bytes):
        value = bytes_or_str.decode('utf-8')
    else:
        value = bytes_or_str
    # Instance of str
    return value

#另外，还需要编写接受str或bytes，并总是返回bytes的方法：
def to_bytes(bytes_or_str):
    if isinstance(bytes_or_str, str):
        value = bytes_or_str.encode('utf-8')
    else:
        value = bytes_or_str
    # Instance of bytes
    return value

def test3():
    bwords = b"hello bytes"
    print(to_str(bwords))
    bwords = to_bytes("hello world")
    print(bwords)
    print(to_str(bwords))

#在Python2中，需要编写接受str或unicode，并总是返回unicode的方法：
#python2
def to_unicode2(unicode_or_str):
    if isinstance(unicode_or_str, str):
        value = unicode_or_str.decode('utf-8')
    else:
        value = unicode_or_str
    # Instance of unicode
    return value

#另外，还需要编写接受str或unicode，并总是返回str的方法：
#Python2
def to_str2(unicode_or_str):
    if isinstance(unicode_or_str, unicode):
        value = unicode_or_str.encode('utf-8')
    else:
        value = unicode_or_str
    # Instance of str
    return value

def test2():
    uwords = u"hello bytes"
    print(to_str2(uwords))
    uwords = to_unicode2("hello world")
    print(uwords)
    print(to_str2(uwords))

if int(sys.version_info[0]) >= 3:
    test3()
else:
    test2()

