#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 winter online qualifications
# Title : Let's disassemble
# Genre : Binary
# Points: 200
# Author: @shiracamus

import socket
import sys

HOST, PORT = 'disassemble.quals.seccon.jp', 23168

def load_instructions():
    def split(line):
        code, mnemonic = line.split('  ', 1)
        return code.split(), mnemonic.strip()
    with open('Z80.txt') as f:
        return [split(line) for line in f if line[0] != '#']

instructions = load_instructions()

def solve(data):
    data = data.split('\n')[0].split(': ')[1].split()
    for code, mnimonic in instructions:
        size = len(code)
        if 'nn' in code:
            size += 1
        if len(data) != size:
            continue
        for i in range(len(code)):
            if code[i] == 'nn':
                mnimonic = mnimonic.replace('nn', data[i+1]+data[i]+'H')
            elif code[i] == 'n':
                mnimonic = mnimonic.replace('n', data[i]+'H')
            elif code[i] == 'p':
                mnimonic = mnimonic.replace('p', data[i]+'H')
            elif code[i] == 'd':
                d = int(data[i],16)
                if d > 0x7f:
                    d -= 0x100
                mnimonic = mnimonic.replace('+d', "%+d"%d)
            else:
                if code[i] != data[i]:
                    break
        else:
            return mnimonic
    return "unknown"

def recv(s):
    data = ''
    while True:
        d = s.recv(256)
        if not d:
            print 'abort:', data
            exit()
        data += d
        if 'SECCON{' in data:
            print data
            exit()
        if '? ' in d:
            print data,
            return data

if __name__ == '__main__':
    if len(sys.argv) >= 2:
        HOST = sys.argv[1]
    if len(sys.argv) >= 3:
        PORT = int(sys.argv[2])
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    while True:
        ans = solve(recv(s))
        print ans
        s.send(ans+'\n')
