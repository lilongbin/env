#! /bin/bash

sudo apt autoremove #remove unuseful package

sudo apt autoclean #remove package that has been uninstalled
sudo apt clean     #remove package that has been installed

#dpkg -l | grep ^rc | awk '{print $2}' | sudo xargs dpkg -P #remove unuseful config file

#dpkg --get-selections | grep linux

sudo rm -r /var/log/*

history -c


