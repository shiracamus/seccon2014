#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 summer online qualifications
# Title : Let's disassemble
# Genre : Programming
# Points: 300
# Author: @shiracamus

import sha

NUM = 1000

def solve(amida):
    x = amida[-2].index('*')
    for y in range(len(amida)-3, 0, -1):
        if x > 0 and amida[y][x-1] == '-':
            x -= 2
        elif amida[y][x+1] == '-':
            x += 2
    answer = x / 2 + 1

    data = ""
    for y in amida[1:-2]:
        data += chr(int("".join("01"[c=='-'] for c in y[1:16:2]), 2))
    data += chr(amida[-2].index('*') / 2 + 1) # add the goal

    return data, answer

def dump_amida(amida, key):
    xor = sha.new(key).digest()
    print "{",
    for a, b in zip(amida, xor):
        print "0x%02x," % (ord(a) ^ ord(b)),
    print "},"

def main():
    f = open("amida.data")
    data = [solve([f.readline() for y in xrange(22)]) for i in range(NUM)]
    f.close()
    data += [data[0]] # for check the last answer

    answers = ""
    for amida, answer in data:
        dump_amida(amida, answers)
        answers += str(answer)

if __name__ == "__main__":
    main()
