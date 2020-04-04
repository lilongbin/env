#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-01-13 17:55:12
####################################################

import json

def json_load(fd):
    # load from json file
    pinfo = json.load(fd)
    # print(pinfo)
    return pinfo

def json_loads(jstr):
    # load from json file
    pinfo = json.loads(jstr)
    # print(pinfo)
    return pinfo

def json_dump(pinfo, fd):
    # dump to json file
    json.dump(pinfo, fd, indent=4)
    print("json.dump")

def json_dumps(pinfo, fd):
    # dump to json file
    fd.write(json.dumps(pinfo, indent=4))
    print("json.dumps")

def test():
    dct = {"name": "tom", "age": 23}
    filename = "test1.json"
    with open(filename, "w") as fd:
        json_dump(dct, fd)
    with open(filename, "r") as fd:
        pinfo = json_load(fd)
        print(pinfo)

    filename = "test2.json"
    with open(filename, "w") as fd:
        json_dumps(dct, fd)
    with open(filename, "r") as fd:
        pinfo = json_loads(fd.read())
        print(pinfo)

if __name__ == "__main__":
    test()


