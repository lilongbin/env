#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-08-06 13:26:08
####################################################

function inotify_events()
{
	local dst_files=$*
	local inotifywait=$(which inotifywait 2>/dev/null)
	inotifywait=${inotifywait:=/usr/bin/inotifywait}
	if [ ! -x ${inotifywait} ] ;then
		echo "ERROR: ${inotifywait}: No such command."
		echo -e "Usage:\n\tinotify_events file(s)"
		return;
	fi
	if [ "" = "${dst_files}" ] ;then
		echo "ERROR: empty parameter, please specify the file(s) to be watched."
		echo -e "Usage:\n\tinotify_events file(s)"
		return;
	fi
	local format_opt='--timefmt %Y/%m/%d_%H:%M:%S --format [%T]%w%f[%e]'
	local event_opt='-e create,delete,modify,move,attrib'

	${inotifywait} -mrq ${format_opt} ${event_opt} ${dst_files} | while read watched
	do
		# echo "${watched}"
		local wtime=$(echo ${watched} | tr -t '[]' '  ' | awk '{print $1}')
		local wfile=$(echo ${watched} | tr -t '[]' '  ' | awk '{print $2}')
		local wevent=$(echo ${watched} | tr -t '[]' '  ' | awk '{print $3}')
		# echo "time: ${wtime}"
		# echo "file: ${wfile}"
		# echo "event: ${wevent}"
		echo "${wevent} ${wfile} @${wtime}"
	done
}

inotify_events $*

