#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2019-02-13 13:29:53
####################################################

echo -e "Usage:\n\t$(basename $0) arg1 arg2 etc.\n"

echo "Listing args with \"\$*\":"
index=1
for arg in "$*"
do
	echo "Arg #$index = $arg"
	let "index+=1"
done
echo

echo "Listing args with \"\$@\":"
index=1
for arg in "$@"
do
	echo "Arg #$index = $arg"
	let "index+=1"
done
echo


echo 'Listing args with $* (unquoted):'
index=1
for arg in $*
do
	echo "Arg #$index = $arg"
	let "index+=1"
done
exit 0

