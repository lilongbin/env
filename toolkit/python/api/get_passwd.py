#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-03-25 13:48:26
####################################################

import sys
import tty
import termios

class PassWd(object):
    def __init__(self, maskchar="*"):
        self.maskchar = maskchar

    def getch(self):
        fd = sys.stdin.fileno() 
        old_settings = termios.tcgetattr(fd) 
        try:
            tty.setraw(fd) 
            ch = sys.stdin.read(1) 
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings) 
        return ch
    
    def getpass(self):
        password = ""
        masklen = len(self.maskchar)
        while True:
            ch = self.getch()
            if ch == "\r" or ch == "\n":
                print('')
                break
            elif ch == "\b" or ord(ch) == 127: 
                if len(password) > 0: 
                    sys.stdout.write("\b \b" * masklen) 
                    password = password[:-1] 
            else: 
                if self.maskchar: 
                    sys.stdout.write(self.maskchar) 
                password += ch
            sys.stdout.flush()
        return password

def test():
    passwd = PassWd("**")
    for x in range(3):
        sys.stdout.write("Enter Password: ")
        sys.stdout.flush()
        pw = passwd.getpass()
        print('you input: %s' % pw)

if __name__ == "__main__": 
    test()

