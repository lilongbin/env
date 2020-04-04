#! /usr/bin/env python
# class subprocess test of python language
# created by longbin <beangr@163.com>
# 2015-01-07

import subprocess

def popen_test():
    # use class subprocess to get child's pid,
    # return value, stdin, stdout, stderr etc.
    sPopen = subprocess.Popen("ls ${HOME}", shell=True, stdout=subprocess.PIPE)
    sPopen.wait() # wait for sPopen's process end 
    
    print("sPopen.pid: ", sPopen.pid)
    print("sPopen.returncode: ", sPopen.returncode)
    print("sPopen.stdin: ", sPopen.stdin)
    print("sPopen.stdout.read(): ", sPopen.stdout.read())
    print("sPopen.stderr: ", sPopen.stderr)
    
    # if ... else statement
    if sPopen.pid == sPopen.pid:
    	print("sPopen.pid == sPopen.pid")
    else:
    	print("sPopen.pid != sPopen.pid")
    	
    # communicate can get stderr and stdout
    out = sPopen.communicate()
    print("sPopen.communicate out: ", out)
    print("sPopen.communicate out[0] = ", out[0], " out[1]", out[1], "\n")

    res = subprocess.Popen("echo \"hello python\"", shell=True, stdout=subprocess.PIPE)
    print("res.stdout.read(): ", res.stdout.read())
    print("res.returncode: ", res.returncode)

def call_test():
    # get return code of shell commands
    retcode = subprocess.call("ls ~/", shell=True, stdout=subprocess.PIPE)
    print("subprocess.call retcode: ", retcode)
    try:
    	res = subprocess.check_call(['ls', '-l'])
    except err:
    	print("subprocess.check_call res: ", res)

if __name__ == "__main__":
    popen_test()
    call_test()

