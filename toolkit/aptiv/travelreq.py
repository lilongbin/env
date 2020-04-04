#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-10-23 13:32:32
####################################################

import time
try: input = raw_input
except: pass

ReMark = """
a. Name: %(name)s
b. Purpose: travelling for %(project)s project %(job)s in %(destnation)s.
c. Destination: %(destnation)s
d. Duration: %(business_days)d day(s) (%(duration)s)
e. Estimated cost
   i. Transportation ￥500.00
   ii. Meal ￥%(est_cost_meal)s
   iii. Others (if applicable)
"""

class TravelReqTable(object):
    def __init__(self):
        self.days_begin = 1
        self.business_days = 1
        self.TravelReqDct = {
                "name"          : "Li Longbin",
                "project"       : "C1UB",
                "job"           : "joint debug",
                "destnation"    : "Shanghai PATAC",
                }

    def set_basic_info(self):
        for key in self.TravelReqDct:
            value = self.TravelReqDct[key]
            new_value = input("%s: [%s] ? " % (key, value))
            if new_value and new_value.strip():
                self.TravelReqDct[key] = new_value.strip()

    def set_date_begin(self):
        days_begin = input("which day to start travel? (0:today, 1:tomorrow, ...) ")
        if days_begin and days_begin.strip():
            self.days_begin = int(days_begin)

    def set_business_days(self):
        business_days = input("How long is the business trip? (%d days) " % self.business_days)
        if business_days and business_days.strip():
            self.business_days = int(business_days)
        self.TravelReqDct["business_days"] = self.business_days

    def set_time_duration(self):
        duration_days_begin = self.days_begin
        duration_days_end   = duration_days_begin + self.business_days - 1
        
        time_duration_begin = time.strftime("%Y-%m-%d 8:30",  time.localtime(time.time() + 3600*24*duration_days_begin))
        time_duration_end   = time.strftime("%Y-%m-%d 17:30", time.localtime(time.time() + 3600*24*duration_days_end))
        time_duration = time_duration_begin + " -- " + time_duration_end
        self.TravelReqDct["duration"] = time_duration
        
    def set_est_cost(self):
        self.est_cost_meal = 200.0 * self.business_days
        self.TravelReqDct["est_cost_meal"] = self.est_cost_meal

    def run(self):
        self.set_basic_info()
        self.set_date_begin()
        self.set_business_days()
        self.set_time_duration()
        self.set_est_cost()
        print("\nTravel Request Reason:")
        print(ReMark % self.TravelReqDct)

def test():
    travel = TravelReqTable()
    travel.run()

if __name__ == "__main__":
    test()

