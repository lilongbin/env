#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2019-06-10 13:25:06
####################################################

function userhelp()
{
	echo "Usage:"
	echo "    txtconv <files>"
}

function convert()
{
	local FROMCODE=$1
	local TOCODE=$2
	local INPUTFILE=$3
	if [ $# -lt 3 ] ;then
		echo "ERROR: parameters too less."
		return
	fi
	if [ ! -f ${INPUTFILE} ] ;then
		echo "${INPUTFILE}: No such file."
		return
	fi
	local OUTPUTFILE=${INPUTFILE}.${FROMCODE}2${TOCODE}.txt
	iconv -f ${FROMCODE:=gb18030} -t ${TOCODE:=utf-8} -o ${OUTPUTFILE} ${INPUTFILE}
}

function txtconv()
{
	local args=$*
	if [ $# -eq 0 ] ;then
		userhelp
	fi
	if [ "${args,,}" = "--help" -o "${args,,}" = "-h" -o "${args,,}" = "?" ] ;then
		userhelp
		return
	fi
	for file in ${args}
	do
		convert gb18030 utf-8 ${file}
		convert utf-8 gb18030 ${file}
	done
}

txtconv $*

