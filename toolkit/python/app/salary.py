#! /usr/bin/env python
# coding=utf-8

try:
    input = raw_input
except:
    pass

class Salary(object):
    def __init__(self):
        self.raw_salary = 0.0
        self.hourly_salary = 0.0
        self.overtime_fee = 0.0
        self.dinner_subsidy = 0.0
        self.social_security = 0.0
        self.personal_leave = 0.0
        self.ill_leave = 0.0
        self.total_salary = 0.0
        self.total_tax = 0.0
        self.final_salary = 0.0
        #self.get_salary_sum()
    def get_dataf(self, msg=''):
        dataf = input(msg).strip()
        if not dataf:
            dataf = 0.0
        dataf = float(dataf)
        return dataf
    def get_salary_sum(self):
        msg = 'Raw salary: '
        self.raw_salary = self.get_dataf(msg)
        self.hourly_salary = self.raw_salary/174.0
    def get_overtime_fee(self):
        msg = 'Over time(h): '
        overtime = self.get_dataf(msg)
        self.overtime_fee = self.hourly_salary*2*overtime
    def get_dinner_subsidy(self):
        msg = 'Dinner subsidy times: '
        ds_times = self.get_dataf(msg)
        self.dinner_subsidy = ds_times * 10.0
    def get_social_security(self):
        self.social_security = self.raw_salary * 0.185
    def get_personal_leave(self):
        msg = 'Personal leave days: '
        plhours = self.get_dataf(msg) * 8
        self.personal_leave = self.hourly_salary * plhours
    def get_ill_leave(self):
        msg = 'Ill leave days: '
        ilhours = self.get_dataf(msg)
        self.ill_leave = self.hourly_salary*ilhours*0.6
    def get_total_salary(self):
        self.get_salary_sum()
        self.get_overtime_fee()
        self.get_dinner_subsidy()
        self.get_social_security()
        self.get_personal_leave()
        self.get_ill_leave()
        total = self.raw_salary
        total += self.overtime_fee
        total += self.dinner_subsidy
        total -= self.personal_leave
        total -= self.ill_leave
        total -= self.social_security
        self.total_salary = total
    def get_total_tax(self):
        sum_total = self.total_salary
        new_sal = True
        if not new_sal:
            sal_stage = [3500,1500, 4500, 9000,35000,55000,80000]
            tax_rate  = [0.00,0.03, 0.10, 0.20, 0.25, 0.30, 0.35, 0.45]
            quick_op  = [   0,   0,  105,  555, 1005, 2755, 5505,13505]
        else:
            sal_stage = [5000,3000,12000,25000,35000,55000,80000]
            tax_rate  = [0.00,0.03, 0.10, 0.20, 0.25, 0.30, 0.35, 0.45]
            quick_op  = [   0,   0,  210, 1410, 2660, 4410, 7160,15160]
        
        tax_stage = [0,] * len(tax_rate)
        for i,x in enumerate(sal_stage):
            for j in range(i+1):
                tax_stage[i]+=int(sal_stage[j])
        index_num = 0
        
        if sum_total <= tax_stage[0]:
            return
        
        while index_num<len(sal_stage) and sum_total>tax_stage[index_num]:
            index_num += 1
        # print(tax_stage)
        # print(index_num)
        total_tax = 0
        sum_total -= sal_stage[0]
        if not new_sal:
            total_tax = sum_total*tax_rate[index_num+1]-quick_op[index_num+1]
        else:
            total_tax = sum_total*tax_rate[index_num]-quick_op[index_num]
        self.total_tax = total_tax
    def get_final_salary(self):
        self.get_total_salary()
        self.get_total_tax()
        self.final_salary = self.total_salary - self.total_tax

def test():
    sal = Salary()
    fs = sal.get_final_salary()
    print('Social security: %s' % sal.social_security)
    print('Total tax:       %s' % sal.total_tax)
    print('Final salary:    %s' % sal.final_salary)
    input('press <Enter> to continue.')

if __name__ == '__main__':
    test()
    
        
    
