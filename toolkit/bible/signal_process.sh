#! /bin/bash
clear

function process_signal(){
	echo "generating a background child process"
	for ((i=9; i>=0; i--))
	do
		## 接收HUP信号,并进行处理
		trap 'echo -e "\nchild process catched HUP signal and exit"; exit 0' HUP
		sleep 1
		echo -e "\rsleeping ${i}s ...\c"
	done &
	## 创建后台进程
	## 获取最近创建的后台进程的PID
	BG_PID=$!
	
	sleep 3
	## 向后台进程发HUP信号
	echo -e "\nkill HUP signal to child process ${BG_PID}"
	(kill -HUP ${BG_PID})

	VAR="hello good morning"
	echo ${VAR}
	echo "function finished."
}

process_signal
echo ${VAR}
echo ${VAR}
echo ${VAR}
echo "sleeping 3s .. "
sleep 3
