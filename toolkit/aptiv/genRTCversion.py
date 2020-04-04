#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-07-30 15:25:29
####################################################

import os
import re
try:
    input = raw_input
except:
    pass
# Delphi:cv.38.31.00;ODC:6476.19.31.07291416;RPC:20190424.0.3.9.0;HMI:31.81.1.1.190729
# AP Version:  cv.38.31.00
# VT_SOFTWARE_VERSION:   C1UX.MY20.ICI.DayL.38.31.00
# ODC version:  6476.19.31.07291416
# Tire1SdkVer:  201907291557
# AoShuo BackupHMI:  Version:20190729 0.7.0.0
# AoShuo TinyRPC:  Version:20190424 0.3.9.0
# AoShuo MPM:  Version:20190521 0.8.8.0
# AoShuo NaviARBService:  Version: 20180320 0.0.6.0
# AoShuo CarLife:  Version:20190723 2.5.2.0
# VR Version:  3.1.2.47-rc13
# android_version: c1ub_290.30.0.190729
# HMIVersion: 31.81.1.1.190729
# NaviVersion:amap_3.2.9.21678
# vrProxy_version:version:0.2.12
# Title : Harman Daily Release_2019-07-29_v31.81.1.1.190729
# Date : 2019-07-29
# Version : 31.81.1.1.190729
# appshop: 47.1.1.1.190321
# kaolafm: versionName:34.1.1.1.190723
# VRProxy: version:0.2.12
# Travelgroup: travelgroup_C1UB : 20190719-1.3.7.9
# Amapauto: NaviVersion:amap_3.2.9.21678
# Netmusic: version:GJBUC_KW_1.0.0_1.0.2_190723_Beta
# Android tag: c1ub_master_20190729_bn290  
# HMI commit(d2ub): 60124f2c9ef3e685624607c4fb7fd323fdce6fca
# FBL Version:  261829.01
# BSP commit(Global_A): 02c8f911a1e87b0033bc3b3d5e717f803d2024ea
# ODC commit(d2ub): 0ef8535a2b13aa3d8bc2c352535e02c0231c6a9c

class RTCVersion(object):
    def __init__(self):
        self.ap_ver = "AP Version"
        self.odc_ver = "ODC version"
        self.rpc_ver = "AoShuo TinyRPC"
        self.hmi_ver = "HMIVersion"
        # Delphi:XX.0.0.XX;ODC:0.0;RPC:00.00;HMI:0.0.0
        self.ver_info = {
                self.ap_ver  : "XX.0.0.XX", 
                self.odc_ver : "0.0",
                self.rpc_ver : "00.00",
                self.hmi_ver : "0.0.0"
                }
        self.ver_format_str = "Delphi:%s;ODC:%s;RPC:%s;HMI:%s" % (self.ap_ver, self.odc_ver, self.rpc_ver, self.hmi_ver)
    def version(self, key):
        value = ""
        if key in self.ver_info:
            value = self.ver_info[key]
        return value

    def getinput(self):
        print("Please input your info block: ")
        info = ""
        while True:
            var = input().strip()
            if not var: break
            info += var + os.linesep
        return info
    def parser(self, string):
        info = string.split(os.linesep)
        d = {}
        for var in info:
            var = var.split(":")
            if len(var) < 2:
                continue
            d[var[0]] = var[-1].strip().replace(" ", ".")
        # print(d)
        for key in self.ver_info:
            if key in d:
                self.ver_info[key] = d[key]
        # print(self.ver_info)
        self.ap_ver = self.version(self.ap_ver)
        self.odc_ver = self.version(self.odc_ver)
        self.rpc_ver = self.version(self.rpc_ver)
        self.hmi_ver = self.version(self.hmi_ver)
        self.ver_format_str = "Delphi:%s;ODC:%s;RPC:%s;HMI:%s" % (self.ap_ver, self.odc_ver, self.rpc_ver, self.hmi_ver)
        print("The RTC version: ")
        console_rows, console_columns = os.popen('stty size', 'r').read().split()
        console_rows = int(console_rows)
        console_columns = int(console_columns)
        print("#" * console_columns)
        print(self.ver_format_str)
        print("#" * console_columns)
    def genversion(self):
        info = self.getinput()
        # print(info)
        self.parser(info)

def test():
    rtc_ver = RTCVersion()
    rtc_ver.genversion()

if __name__ == "__main__":
    test()

