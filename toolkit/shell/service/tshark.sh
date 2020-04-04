#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-07-19 23:12:42
####################################################

# sudo yum install wireshark
# sudo apt-get install tshark

function pcap_catch()
{
	if [ "-h" == "${1}" -o "--help" == "${1}" ] ;then
		echo "generate config file or load default config to catch data"
		echo -e "Usage:\n\tpcap_catch [init]"
		return
	fi

	local CONFIG=pcap.ini
	if [ "init" == "${1}" ] ;then
		touch ${CONFIG}
		echo 'PCAP_FILE=capture_${date +%F_%H%M%S}.pcap' > ${CONFIG}
		echo "CAP_PORT_COND='port 10111'" >> ${CONFIG}
		echo "CAP_IF_COND='-i eth0'" >> ${CONFIG}
		echo "CAP_AUTOSTOP_TSHARK_COND='-a duration:15'" >> ${CONFIG}
		echo "CAP_AUTOSTOP_TCPDUMP_COND='-c 20'" >> ${CONFIG}
		echo "Init OK, please config the file ${CONFIG}"
		return
	fi
	local TIME_NOW=$(date +%F_%H%M%S)
	local PCAP_FILE=capture_${TIME_NOW}.pcap
	# local CAP_HOST_COND="host 192.168.126.128"
	local CAP_PORT_COND='port 10111'
	local CAP_INTERFACE=eth1
	local CAP_IF_COND='-i ${CAP_INTERFACE:=lo}'
	local CAP_AUTOSTOP_TSHARK_COND='-a duration:15' # seconds
	local CAP_AUTOSTOP_TCPDUMP_COND='-c 20'
	local USER=${UID}

	[ -f ${CONFIG} ] && source ${CONFIG} && echo "load ${CONFIG} OK."
	local capture_cmd="sudo tcpdump ${CAP_IF_COND} ${CAP_HOST_COND} ${CAP_PORT_COND} ${CAP_AUTOSTOP_TCPDUMP_COND} -w /tmp/${PCAP_FILE}"
	# local capture_cmd="sudo tshark ${CAP_IF_COND} ${CAP_HOST_COND} ${CAP_PORT_COND} ${CAP_AUTOSTOP_TSHARK_COND} -w /tmp/${PCAP_FILE}"

	echo "The capture auto stop condition is ${CAP_AUTOSTOP_COND} for tshark."
	echo "Start to capture ..."
	echo "press <Ctrl-c> to stop capture"

	echo -e "\n${capture_cmd}\n"
	${capture_cmd}
	if [ "$?" != "0" ] ;then
		return
	fi

	sudo mv /tmp/${PCAP_FILE} ./
	if [ -f ${PCAP_FILE} ] ;then
		sudo chown ${USER} ${PCAP_FILE}
		ls -lh ${PCAP_FILE}
		echo -e "Capture completed, and the capture data saved to ${PCAP_FILE}\n"
	else
		echo "ERROR: error occurred."
	fi
}

function pcap_parsep()
{
	local pcap_file=$1
	if [ "-h" == "${1}" -o "--help" == "${1}" ] ;then
		echo "parse the specified file"
		echo -e "Usage:\n\tpcap_parsep file" 
		return
	fi
	if [ "" == "${pcap_file}" ] ;then
		echo -e "ERROR: parameter is empty."
		echo -e "Usage:\n\tpcap_parsep file"
		return
	fi
	if [ ! -f ./${pcap_file} ] ;then
		echo -e "ERROR: ${pcap_file}: No such file."
		return
	fi
	local SEPARATOR="##########################################################"
	local PARSE_SEP_COND="-S ${SEPARATOR}"

	echo "$(date +%F\ %T)"
	echo "The capture summary:"
	echo "${SEPARATOR}"
	tshark -r ${pcap_file} -P
	echo "${SEPARATOR}"
	echo ""
	echo "The packet details:"
	echo "${SEPARATOR}"
	tshark -r ${pcap_file} ${PARSE_SEP_COND} -V
	echo "${SEPARATOR}"
}

function pcap_parse()
{
	local PCAP_FILE=${1}
	if [ "-h" == "${PCAP_FILE}" -o "--help" == "${PCAP_FILE}" ] ;then
		echo "parse the specified file or all files with pcap suffix"
		echo -e "Usage:\n\tpcap_parse [file]"
		return
	fi
	if [ "" == "${PCAP_FILE}" ] ;then
		PCAP_FILE=$(ls *.pcap 2>/dev/null)
	fi
	if [ "" == "${PCAP_FILE}" ] ;then
		echo -e "Notify: No capture raw file to parse."
		return
	fi
	for pcap_file in ${PCAP_FILE}
	do
		echo "Begin to parse the capture file: ${pcap_file}"
		pcap_parsep ${pcap_file} 2>/dev/null > ${pcap_file}.txt
		echo "The parse result has saved to ${pcap_file}.txt"
	done
}

function __complete_pcap_parse()
{
	local curw=${COMP_WORDS[COMP_CWORD]}
	local wordlist=$(find . -maxdepth 1 -name \*.pcap -printf "%f\n" )
	# 定义双击TAB后显示的结果;compgen -W基于curw提供可能的补全
	COMPREPLY=( $(compgen -W '${wordlist[@]}' -- "${curw}" ) )
}
complete -F __complete_pcap_parse pcap_parse pcap_parsep

# pcap_catch
# pcap_parse

