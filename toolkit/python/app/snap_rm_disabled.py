#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2020-05-21 13:35:05
####################################################

import os
import subprocess

def remove_snap_disabled():
    subcmd = "sudo snap list --all"
    param = "passwd"
    matrix = []
    try:
        p = subprocess.Popen(subcmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        out, err = p.communicate(input=param)
        if not err:
            # print(out)
            lines = out.split(os.linesep)
        for line in lines:
            # print(line.split())
            matrix.append(line.split())
        # print(matrix)
    except Exception as reason:
        print("Error: %s" % (reason))

    if not len(matrix):
        return

    if not len(matrix[0]):
        return

    if len(matrix[0]) == 6:
        snap_pack_idx = 0
        snap_rev_idx = 2
        snap_note_idx = 5

    for line in matrix:
        if len(line) != 6:
            continue
        snap_pack_name = line[snap_pack_idx]
        snap_rev_value = line[snap_rev_idx]
        snap_note_value = line[snap_note_idx]
        if snap_note_value == "disabled":
            print("%s --revision %s is disabled, will be removed." % (snap_pack_name, snap_rev_value))
            subcmd = "sudo snap remove %s --revision %s" % (snap_pack_name, snap_rev_value)
            try:
                p = subprocess.Popen(subcmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
                out, err = p.communicate(input=param)
                if not err:
                    print(out)
            except Exception as reason:
                print("Error: %s" % (reason))
    print("remove disabled snap complete.")

def test():
    remove_snap_disabled()

if __name__ == '__main__':
    test()

