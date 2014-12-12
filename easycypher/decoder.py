#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 winter online qualifications
# Title : Easy cypher
# Genre : Crypto
# Points: 100
# Author: @shiracamus

def decode(number):
    if number[0] == '0':
        return int(number, 8)
    if len(number) > 3:
        return int(number, 2)
    if any(c in 'abcdef' for c in number):
        return int(number, 16)
    return int(number)

for line in open('cypher.txt'):
    print ''.join(chr(decode(number)) for number in line.split())
