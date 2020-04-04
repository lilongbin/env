#! /bin/bash

####################################################
# Author      : longbin
# Created date: 2018-01-10 00:07:33
####################################################

USERNAME=root
PASSWORD=root
HOST_DIR=//10.171.59.99/projects
DST_DIR=/usr1/share

mount -t ext3 -o username=${USERNAME},password=${PASSWORD} ${HOST_DIR} ${DST_DIR}

# /etc/fstab
# ${HOST_DIR} ${DST_DIR} cifs username=${USERNAME},password=${PASSWORD} 0 0

