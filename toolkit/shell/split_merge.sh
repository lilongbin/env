#! /bin/bash

function help()
{
	echo "Usage:"
	echo "split_file largefile.xxx smallprefix."
	echo "merge_file largefile.xxx smallprefix."
}

function split_file()
{
	file=$1
	prefix=$2
	split -b 5M -d ${file} ${prefix}
}
# split_file ve2.7.tar.gz ve2.7.

function merge_file()
{
	target=$1
	prefix=$2
	touch ${target}
	for file in $(ls ${prefix}*)
	do
		cat ${file} >> ${target}
	done
}
# merge_file ve2.7.tar.gz ve2.7.
