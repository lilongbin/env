#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-03-25 13:46:54
####################################################

try:
    input = raw_input
except:
    pass
def get_user_pass():
    import getpass
    username = input('username: ')
    password = getpass.getpass('password: ')
    return [username, password]

username, password = get_user_pass()
print('%s %s' % (username, password))

