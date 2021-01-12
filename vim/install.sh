#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2020-06-03 15:06:12
####################################################

cur_dir=$(pwd)

if [ -f .vimrc ] ;then
    rm -rfv ~/.vimrc
    rm -rfv ~/.viminfo
    rm -rfv ~/.vim
    cp -rfv ${cur_dir}/.vimrc   ~/
    cp -rfv ${cur_dir}/.viminfo ~/
    cp -rfv ${cur_dir}/.vim     ~/
fi

