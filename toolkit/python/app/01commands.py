#! /usr/bin/python
# class subprocess test of python language
# created by longbin <beangr@163.com>
# 2015-01-07

import commands
import os

# get output of shell commands
out = commands.getoutput("ls /home/")
print "\rcommands.getoutput out : ", out, "\n"

# get status and output of shell commands
(status, output) = commands.getstatusoutput("ls /home/")
print "\rstatus : ", status, "\noutput : ", output
print ""

# get return value of shell commands
res = os.system("echo \"hello python\"")
print "os.system res = ", res

# get output of shell commands
res = os.popen("echo \"hello python\"")
print "\nos.popen res.read() : ", res.read()

