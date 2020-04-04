#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-09-07 00:54:26
####################################################

import sys, re
import matplotlib.pyplot as plt

g_Guidelines_log_file="guidelines.points.txt"
Listcolors = ['red', 'blue', 'green', 'cyan', 'magenta', 'orange', 'darkred', 'black']
Markers = ['^', '|', 'o', 'v', 's', 'p', '*', '+', 'x']

Re_lineinfo = re.compile("a_draw_line_info\.line\[([0-9]+)\], number_of_points_for_curr_line=([0-9]+)")
# Re_point = re.compile("painting_guidelines data_points\[([0-9]+)\] u_point=([0-9.]+), v_point=([0-9.]+)")
Re_point = re.compile("\[([0-9]+)\] u_point=([0-9.]+), v_point=([0-9.]+)")

One_test_normal_case="""
[95851] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[0], number_of_points_for_curr_line=3
[95852] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.242969, v_point=0.754167
[95852] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.271094, v_point=0.669444
[95852] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[2] u_point=0.364844, v_point=0.672222
[95857] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[1], number_of_points_for_curr_line=3
[95857] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.709375, v_point=0.768056
[95857] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.682031, v_point=0.681944
[95858] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[2] u_point=0.588281, v_point=0.677778
[95862] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[2], number_of_points_for_curr_line=13
[95863] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.271094, v_point=0.669444
[95863] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.293750, v_point=0.604167
[95863] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[2] u_point=0.312500, v_point=0.550000
[95863] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[3] u_point=0.328125, v_point=0.506944
[95864] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[4] u_point=0.341406, v_point=0.470833
[95864] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[5] u_point=0.353125, v_point=0.440278
[95864] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[6] u_point=0.362500, v_point=0.415278
[95864] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[7] u_point=0.378125, v_point=0.375000
[95865] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[8] u_point=0.390625, v_point=0.344444
[95865] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[9] u_point=0.400000, v_point=0.322222
[95865] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[10] u_point=0.407813, v_point=0.302778
[95865] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[11] u_point=0.414063, v_point=0.287500
[95866] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[12] u_point=0.419531, v_point=0.275000
[95874] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[3], number_of_points_for_curr_line=13
[95875] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.682031, v_point=0.681944
[95875] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.660156, v_point=0.613889
[95875] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[2] u_point=0.641406, v_point=0.559722
[95876] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[3] u_point=0.625781, v_point=0.515278
[95876] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[4] u_point=0.613281, v_point=0.477778
[95876] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[5] u_point=0.601563, v_point=0.447222
[95876] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[6] u_point=0.592188, v_point=0.422222
[95877] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[7] u_point=0.576563, v_point=0.380556
[95877] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[8] u_point=0.564844, v_point=0.350000
[95877] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[9] u_point=0.555469, v_point=0.326389
[95878] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[10] u_point=0.547656, v_point=0.306944
[95878] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[11] u_point=0.541406, v_point=0.291667
[95878] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[12] u_point=0.535937, v_point=0.277778
[95887] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[4], number_of_points_for_curr_line=2
[95887] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.312500, v_point=0.550000
[95887] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.641406, v_point=0.559722
[95897] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[5], number_of_points_for_curr_line=2
[95898] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.362500, v_point=0.415278
[95898] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.592188, v_point=0.422222
[95905] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[6], number_of_points_for_curr_line=2
[95905] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.390625, v_point=0.344444
[95906] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.564844, v_point=0.350000
[95911] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[7], number_of_points_for_curr_line=2
[95912] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.407813, v_point=0.302778
[95912] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.547656, v_point=0.306944
[95916] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[8], number_of_points_for_curr_line=2
[95916] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.419531, v_point=0.275000
[95917] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.535937, v_point=0.277778
[95920] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[9], number_of_points_for_curr_line=15
[95921] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.254687, v_point=0.741667
[95921] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.299219, v_point=0.652778
[95921] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[2] u_point=0.299219, v_point=0.652778
[95921] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[3] u_point=0.337500, v_point=0.583333
[95922] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[4] u_point=0.370313, v_point=0.529167
[95922] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[5] u_point=0.399219, v_point=0.486111
[95922] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[6] u_point=0.425781, v_point=0.451389
[95923] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[7] u_point=0.449219, v_point=0.423611
[95923] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[8] u_point=0.470313, v_point=0.401389
[95923] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[9] u_point=0.508594, v_point=0.366667
[95923] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[10] u_point=0.542188, v_point=0.343056
[95924] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[11] u_point=0.572656, v_point=0.327778
[95924] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[12] u_point=0.600781, v_point=0.316667
[95924] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[13] u_point=0.628125, v_point=0.309722
[95924] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[14] u_point=0.653906, v_point=0.306944
[95939] 1:41; draw_guidelines.c:866; NOTIFY - painting_guidelines a_draw_line_info.line[10], number_of_points_for_curr_line=15
[95939] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[0] u_point=0.697656, v_point=0.755556
[95940] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[1] u_point=0.653906, v_point=0.663889
[95940] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[2] u_point=0.653906, v_point=0.663889
[95940] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[3] u_point=0.616406, v_point=0.591667
[95940] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[4] u_point=0.583594, v_point=0.536111
[95941] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[5] u_point=0.554688, v_point=0.490278
[95941] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[6] u_point=0.529688, v_point=0.454167
[95941] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[7] u_point=0.506250, v_point=0.425000
[95942] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[8] u_point=0.485156, v_point=0.401389
[95942] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[9] u_point=0.446875, v_point=0.365278
[95942] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[10] u_point=0.413281, v_point=0.340278
[95943] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[11] u_point=0.382813, v_point=0.322222
[95943] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[12] u_point=0.354688, v_point=0.311111
[95943] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[13] u_point=0.327344, v_point=0.302778
[95943] 1:41; draw_guidelines.c:878; NOTIFY - painting_guidelines data_points[14] u_point=0.301562, v_point=0.297222
"""

def usage():
    helpinfo = """Usage:
    1. enable log output of Draw_Guidelines_painting_guidelines(), such as:
painting_guidelines a_draw_line_info.line[%d], number_of_points_for_curr_line=%d
painting_guidelines data_points[%d] u_point=%f, v_point=%f
"""+"""
    2. filter these log to %s
    3. run current script
    """ % g_Guidelines_log_file
    print("%s" % helpinfo)

class GuideLinesPLT(object):
    def __init__(self, datafile):
        self.GuideLinesInfoDict = {}
        self.GuideLinesPointFile = datafile if datafile else g_Guidelines_log_file
        self.GuideLinesImageFile = self.GuideLinesPointFile + ".png"

    def load_points(self):
        datafile = self.GuideLinesPointFile
        print("datafile: %s" % datafile)
        guidelines = {}
        try:
            fd = open(datafile)
        except Exception as reason:
            print("Error: %s" % reason)
            return
        ln_start = False
        guidelineid = 0
        currentlinepnum = 0
        pointnum = 0
        pointid = 0
        for ln in fd:
            ln = ln.strip()
            lineinfo = Re_lineinfo.findall(ln)
            if lineinfo:
                if pointnum != currentlinepnum:
                    print("Warning: number_of_points_for_curr_line:%d, get points num:%d" % (currentlinepnum, pointnum))
                # print(lineinfo[0])
                ln_start = True
                pointnum = 0
                guidelineid = int(lineinfo[0][0])
                currentlinepnum = int(lineinfo[0][1])
                # print("currentline:%d, number_of_points_for_curr_line:%d" % (guidelineid, currentlinepnum))
                guidelines[guidelineid] = [[],[]]
                continue
            if ln_start:
                pointinfo = Re_point.findall(ln)
                if pointinfo:
                    pointnum += 1
                    # print(pointinfo[0])
                    pointid = int(pointinfo[0][0])
                    u_point = float(pointinfo[0][1])
                    v_point = float(pointinfo[0][2])
                    guidelines[guidelineid][0].append(u_point)
                    guidelines[guidelineid][1].append(v_point)
        for key in guidelines:
            # print("%d: %s" % (key, guidelines[key]))
            pass
        self.GuideLinesInfoDict = guidelines

    def config(self):
        # config fig
        maxpoint = 0.0
        if self.GuideLinesInfoDict:
            for key in self.GuideLinesInfoDict:
                guidelinepoints = self.GuideLinesInfoDict[key]
                if maxpoint < max(guidelinepoints[0]): maxpoint = max(guidelinepoints[0])
                if maxpoint < max(guidelinepoints[1]): maxpoint = max(guidelinepoints[1])
        plt.figure(figsize=(11,6))
        if maxpoint < 1.0:
            plt.axis([0,1,0,1])
        plt.grid(True)
        plt.title("Guidelines")
        plt.xlabel("X axis", color='red')
        plt.ylabel("Y axis", color='blue')
        # plt.xlim(0, 1)
        # plt.ylim(0, 1)
        
        # invert x/y axis
        ax = plt.gca()
        ax.xaxis.set_ticks_position('top')
        # ax.invert_xaxis()
        ax.yaxis.set_ticks_position('left')
        ax.invert_yaxis()

    def draw(self):
        if not self.GuideLinesInfoDict:
            print("Error: get GuideLinesInfoDict failed")
            sys.exit()
        for key in self.GuideLinesInfoDict:
            guidelinepoints = self.GuideLinesInfoDict[key]
            mlabel = "line%d (%2d*points)" % (key, len(guidelinepoints[0]))
            mcolor = Listcolors[key % len(Listcolors)]
            mmarker = Markers[key % len(Markers)]
            u_points = guidelinepoints[0]
            v_points = guidelinepoints[1]
            plt.plot(u_points, v_points, label=mlabel, color=mcolor, marker=mmarker, linewidth=2)
        plt.legend()
        plt.savefig(self.GuideLinesImageFile)
        print("save fig to: %s" % self.GuideLinesImageFile)

    def show(self):
        plt.show()

    def run(self):
        self.load_points()
        self.config()
        self.draw()
        self.show()

    def draw_points_test(self):
        # for test some points
        u_points = [0.242969, 0.271094, 0.364844]
        v_points = [0.754167, 0.669444, 0.672222]
        print(u_points, v_points)
        plt.plot(u_points, v_points, label="points test", color='red', marker='o', linewidth=2)
        plt.legend()
        plt.show()

def gen_test_data():
    with open(g_Guidelines_log_file, 'w') as fd:
        fd.write("%s\n" % One_test_normal_case)

def main(datafile=''):
    # gen_test_data()
    glplt = GuideLinesPLT(datafile)
    glplt.run()

if __name__ == "__main__":
    datafile = ''
    if len(sys.argv) > 1:
        # print(sys.argv)
        if sys.argv[1].lower() in ["-h", "--help", "?" ]:
            usage()
            sys.exit()
        else:
            datafile = sys.argv[1]
    main(datafile)

