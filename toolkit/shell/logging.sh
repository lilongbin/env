#! /bin/bash
# source logging.sh

function __logging_init()
{
	LOG_TERM_ON=on
	LOG_FILE_ON=on
	LOG_FILE=log_${0}.log
	LOGC=1
	LOGE=2
	LOGW=3
	LOGI=4
	LOGD=5
	LOGV=6
	LOG_LEVEL=${LOG_LEVEL:=3}
}
__logging_init
unset __logging_init

function __log_backup()
{
	LOG_MAX_LINES=${LOG_MAX_LINES:=10000}
	if [ ! -f ${LOG_FILE} ] ;then
		return
	fi
	if [ $(wc -l ${LOG_FILE} 2>/dev/null | awk '{print $1}') -ge ${LOG_MAX_LINES} ] ;then
		echo "backup log file."
		NEW_LOG_FILE=${LOG_FILE%.log}-$(date +%F_%T).log
		mv ${LOG_FILE} ${NEW_LOG_FILE}
		zip -r ${NEW_LOG_FILE%.log}.zip ${NEW_LOG_FILE} && rm ${NEW_LOG_FILE}
	fi
}

function __log_func()
{
    if [ "${LOG_TERM_ON}" == "on" ] ;then
        echo -e $(date +%F\ %T) $*
    fi

    if [ "${LOG_FILE_ON}" != "on" ] ;then
        return
    fi
    if [ ! -d $(dirname ${LOG_FILE}) ] ;then
        mkdir -p $(dirname ${LOG_FILE}) || return
    fi

    touch ${LOG_FILE}
    echo -e $(date +%F\ %T) $* >> ${LOG_FILE}
	if [ "$(date +%S)" = "00" ] ;then
		__log_backup	
	fi
}

function logV()
{
    [ ${LOGV} -gt ${LOG_LEVEL} ] && return
    __log_func "[V] " $*
}

function logD()
{
    [ ${LOGD} -gt ${LOG_LEVEL} ] && return
    __log_func "[D] " $*
}

function logI()
{
    [ ${LOGI} -gt ${LOG_LEVEL} ] && return
    __log_func "[I] " $*
}

function logW()
{
    [ ${LOGW} -gt ${LOG_LEVEL} ] && return
    __log_func "[W] " $*
}

function logE()
{
    [ ${LOGE} -gt ${LOG_LEVEL} ] && return
    __log_func "[E] " $*
}

function logC()
{
    [ ${LOGC} -gt ${LOG_LEVEL} ] && return
    __log_func "[C] " $*
}

function log_test()
{
	logV "Verbose"
	logD "Debugging"
	logI "Info"
	logW "warning"
	logE "Error"
	logC "Critical"
}


