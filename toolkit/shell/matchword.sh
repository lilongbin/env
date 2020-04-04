#! /bin/bash

####################################################
# Author      : longbin
# Created date: 2018-03-08 20:11:52
####################################################

FILENAME=test.txt

awk '
{start=match($0, "main");
	# 返回行号和当前行内容
	if (start) print NR,$0;
	# 返回查找到的字符串的行号,开始位置,长度,字符串
	if (start) print NR,RSTART,RLENGTH,substr($0,RSTART,RLENGTH);
}
' ${FILENAME}

# 返回xxx出现的行号
#grep "xxx" -n ${FILENAME}

