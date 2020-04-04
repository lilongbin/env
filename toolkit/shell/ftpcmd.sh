#! /bin/bash
host=192.168.1.20
port=21
username=1
password=1

filename=test.txt

function ftp_put()
{
local filename=$1
if [ ! -f ${filename} ] ;then
echo "ERROR: ${filename}: No such file."
fi

ftp -n ${host} ${port} << FTPEND
user ${username} ${password}

binary

put ${filename}

bye
FTPEND
}

ftp_put ${filename}
