#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-08-23 14:44:51
####################################################

class IntBitArry(object):
    def __init__(self, integer):
        bits = bin(integer)
        bits = bits.replace('b', '0')
        if len(bits) < 8:
            bits = '0' * (8-len(bits)) + bits
        self.integer = bits
    def __len__(self):
        return len(self.integer)
    def __getitem__(self, item):
        bits = self.integer[::-1][item][::-1]
        # print("%s[%s] : %s" % (int(self.integer,2),item,bits))
        return bits
    def __setitem__(self, key, value):
        self.integer = self.integer[::-1]
        try:
            self.integer[key] = value
        except Exception as reason:
            print("error: %s" % reason)
            pass
        finally:
            self.integer = self.integer[::-1]
    def __str__(self):
        return str(self.integer)

def test():
    bits = IntBitArry(0x51)
    print(bits)
    print("%s"% bits)
    for i in range(len(bits)):
        print("\t%s" % bits[i])
    print(bits[0])
    print(int(bits[0], 2))
    print(bits[4:7])
    print(int(bits[4:7], 2))
    # print(bits[16])
    bits[4] = 0
    print(bits)

if __name__ == "__main__":
    test()

