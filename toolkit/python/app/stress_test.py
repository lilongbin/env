#! /usr/bin/python
#Wayne, f/w update stress test python
import subprocess
import os
import sys, time

def stress_test():
	FNULL = open(os.devnull, 'w')

	dd_cmd="adb shell dd bs=1048576 if=test1.text of=test2.txt count=1024 "
	ck_cmd="adb shell ls test2.txt"

	subprocess.call(dd_cmd.split(), stdout=FNULL, stderr=subprocess.STDOUT)
	proc_res = subprocess.Popen(ck_cmd.split(), stdout=subprocess.PIPE)	
	result = proc_res.stdout.readline()
	if (result.find("No such file or directory") > -1):
		print("could not open file, test2.txt")
		return 1

	sPopen1 = subprocess.Popen("adb shell md5 test1.txt |awk '{print $1}'", shell=True, stdout=subprocess.PIPE)
	output1 = sPopen1.stdout.read()
	sPopen1.communicate()

	sPopen2 = subprocess.Popen("adb shell md5 test2.txt |awk '{print $1}'", shell=True, stdout=subprocess.PIPE)
	output2 = sPopen2.stdout.read()
	sPopen2.communicate()

	## delete /data/tmp/wayne_test.txt
	sub_rm = subprocess.Popen("adb shell rm test2.txt", shell=True, stdout=subprocess.PIPE)
	rm_res = sub_rm.stdout.readline()
	sub_rm.communicate()
	if (rm_res.find("rm failed") != -1):
		print("rm failed")
		return 1

    # print("output1 : %s" % (output1))
    # print("output2 : %s" % (output2))
	if output1 == output2:
		return 0
	else:
		return 1

print("Start testing ...")
count, var, success, failure = 0, 1, 0, 0
while var==1:
	count += 1
	if stress_test() == 0:
		success += 1
	else:
		failure += 1
	print("\rTest circle : %s result <success/failure>: %s/%s ...", count, success, failure)
	sys.stdout.flush()
    # time.sleep(10)
	time.sleep(2)
print("\n Exit while, done for test")

