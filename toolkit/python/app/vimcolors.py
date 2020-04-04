#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-08-06 17:54:05
####################################################

head="<html> <head> <body> <table>"
tail="</table> </body> </head> </html>"

def write_colors_to_html():
    wfd = open('colors.html', 'w')
    nu = 0
    with open('/usr/share/vim/vim74/rgb.txt', 'r') as fd:
        wfd.write(head + "\r\n")
        for line in fd:
            line = line.split()
            try:
                int(line[0])
            except:
                continue
            color = int(line[0]) << 16
            color += int(line[1]) << 8
            color += int(line[2])
            color = "#%06X" % color
            name = " ".join(line[3:])
            # print(color)
            nu += 1
            str = '<tr><td bgcolor="%s" height="30" width="30"></td><td>%3d %s %s</td></tr>' % (color, nu, color, name)
            wfd.write(str + "\r\n")
        wfd.write(tail + "\r\n")

write_colors_to_html()

