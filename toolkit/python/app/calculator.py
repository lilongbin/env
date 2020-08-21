#! /usr/bin/env python
# coding=utf-8
#================================================================
#   File name   : calculator.py
#   Author      : longbin
#   Created date: 2017-10-25 23:52:01
#================================================================

try:
    input = raw_input
except: pass

class Calculator(object):
    def ___init__(self):
        self.n16 = 0x0
        self.n10 = 0
        self.n8 = 0
        self.n2 = 0
        self.ip4 = "0.0.0.0"
        self.ip6 = "0::0"
    def run(self):
        nstr = input("raw number: ").strip().lower()
        if not nstr: return
        self.n10 = 0
        if ":" in nstr:
            # XX:XX XX::XX
            self.ip6 = nstr
            self.xip6()
        elif "." in nstr:
            # x.x.x.x
            self.ip4 = nstr
            self.xip4()
        elif len(nstr) >= 2 and "0x" == nstr[:2]:
            # 0x123456789ABCDEF
            self.n16 = nstr
            self.x16()
        elif len(nstr) >= 2 and "0b" == nstr[:2]:
            # 0bxxxx
            self.n2 = nstr
            self.x2()
        elif nstr[0] == '0':
            # 01234567
            self.n8 = nstr
            self.x8()
        elif "*" in nstr:
            # 12345 * 67890
            self.n10 = eval(nstr)
        else:
            self.n10 = int(nstr)
        self.update()

    def xip6(self):
        ip6 = self.ip6
        if "::" in ip6:
            if "::" == ip6[:2]:
                ip6 = "0"+ip6
            if "::" == ip6[-2:]:
                ip6 = ip6+"0"
            ip6 = ip6.split("::")[0]+":0"*(7-ip6.count(":")+1)+":"+ip6.split("::")[1]
            #print("ip6: %s" % ip6)
        ip6 = ip6.split(":")
        if len(ip6) == 8:
            for i in range(8):
                self.n10 += int(ip6[i], 16) * (2 ** ((7-i)*16))
    def xip4(self):
        ip4 = self.ip4.split(".")
        if len(ip4) == 4:
            for i in range(4):
                self.n10 += int(ip4[i]) * (2 ** ((3-i)*8))
    def x16(self):
        n16 = self.n16.replace(" ", "")
        self.n10 = int(n16, 16)
    def x8(self):
        n8 = self.n8.replace(" ", "")
        self.n10 = int(n8, 8)
    def x2(self):
        n2 = self.n2.replace(" ", "")
        self.n10 = int(n2, 2)
        return
        for i in range(2, len(n2)):
            self.n10 += int(n2[i]) * (2 ** (len(n2) - i - 1))
    def update(self):
        print("n10: %s" % self.n10)
        self.tn16()
        print("n16: %s" % self.n16)
        self.tip6()
        print("ip6: %s" % self.ip6)
        self.tip4()
        print("ip4: %s" % self.ip4)
        self.tn8()
        print("n08: %s" % self.n8)
        self.tn2()
        print("n2:  %s" % self.n2)
        print("prime: %s" % self.isprime(self.n10))
    def tip6(self):
        n10 = self.n10
        self.ip6 = ""
        for i in range(8):
            seg = str(hex((int(n10) >> (7-i)*16) & 0xffff))[2:]
            if seg[-1].lower() == "l":
                seg = seg[:-1]
            self.ip6 += seg
            if i != 7:
                self.ip6 += ":"
        #print("ip6:  %s" % self.ip6)
    def tip4(self):
        n10 = self.n10
        self.ip4 = ''
        for i in range(4):
            self.ip4 += str((int(n10) >> ((3 - i) * 8)) & 0xff)
            if i != 3:
                self.ip4 += '.'
    def tn16(self):
        n10 = self.n10
        self.n16 = 0x0
        if n10 >= 0:
            n16 = str(hex(n10))
        else:
            n16 = str(hex(((0 - n10)^0xffffff) + 0x1))
        if n16[-1].lower() == "l":
            n16 = n16[:-1]
        self.n16 = str(n16)
    def tn8(self):
        self.n8 = 0
        self.n8 = str(oct(int(self.n16, 16)))
    def tn2(self):
        self.n2 = '0b0'
        self.n2 = str(bin(int(self.n16, 16)))
    def isprime(self, n10):
        if n10 < 0: return False
        if not n10 % 2: return False
        import math
        for i in range(2, int(math.sqrt(n10))):
            if not n10 % i:
                return False
        return True

def main():
    cal = Calculator()
    cal.run()

if __name__ == "__main__":
    main()

