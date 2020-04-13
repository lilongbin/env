#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2020-04-10 14:29:24
####################################################

import os
import configparser

class Configeration(object):
    def __init__(self, confpath, encode='utf8'):
        self.filepath = confpath
        self.fileencode = encode
        self.parser = configparser.ConfigParser()
        self.read()

    def read(self):
        try:
            self.parser.read(self.filepath, self.fileencode)
        except Exception as reason:
            print("read(%s) error: %s" % (self.filepath, reason))

    def write(self):
        try:
            self.parser.write(open(self.filepath, 'w'))
        except Exception as reason:
            print("write(%s) error: %s" % (self.filepath, reason))

    def getValue(self, section, option):
        value = None
        parser = self.parser

        if not parser.has_section(section):
            return value
        if not parser.has_option(section, option):
            return value
        try:
            # parser.read(self.filepath, seld.fileencode)
            value = parser.get(section, option)
        except Exception as reason:
            print("getValue(%s,%s) error: %s" % (section, option, reason))
        finally:
            return value

    def setValue(self, section, option, value):
        parser = self.parser
        if not parser.has_section(section):
            parser.add_section(section)
        try:
            parser.set(section, option, value)
        except Exception as reason:
            print("setValue(%s,%s,%s) error: %s" % (section, item, value, reason))

    def addSection(self, section):
        parser = self.parser
        try:
            parser.add_section(section)
        except Exception as reason:
            print("addSection(%s) error: %s" % (section, reason))

    def removeSection(self, section):
        parser = self.parser
        try:
            parser.remove_section(section)
        except Exception as reason:
            print("removeSection(%s) error: %s" % (section, reason))

    def removeOption(self, section, option):
        parser = self.parser
        if not parser.has_section(section):
            return
        if not parser.has_option(section, option):
            return
        try:
            parser.remove_option(section, option)
        except Exception as reason:
            print("removeSection(%s) error: %s" % (section, reason))

    def getSections(self):
        sections = None
        parser = self.parser
        try:
            sections = parser.sections()
        except Exception as reason:
            print("getSections error: %s" % (reason))
        finally:
           return sections

    def getOptions(self, section):
        options = None
        parser = self.parser
        try:
            options = parser.options(section)
        except Exception as reason:
            print("getOptions(%s) error: %s" % (section, reason))
        finally:
            return options

    def getItems(self, section):
        items = None
        parser = self.parser
        try:
            items = parser.items(section)
        except Exception as reason:
            print("getItems(%s) error: %s" % (section, reason))
        finally:
            return items

def test():
    confile = "config.ini"
    config = Configeration(confile)
    print("getValue(rule, path): %s" % config.getValue("rule", "path"))
    config.setValue("rule2", "path", "test.rule")
    config.setValue("rule2", "path2", "test2.rule")
    config.setValue("rule", "path", "test.rule")
    config.setValue("rule", "path2", "test2.rule")
    print("getValue(rule, path): %s" % config.getValue("rule", "path"))
    for sec in config.getSections():
        for opt in config.getOptions(sec):
            print("sec=%s, opt=%s, value=%s" % (sec, opt, config.getValue(sec, opt)))
    print("sec=%s, items=%s" % ("rule2", config.getItems(sec)))
    # config.write()

if __name__ == "__main__":
    test()

