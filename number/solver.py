#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 winter online qualifications
# Title : Choose the number
# Genre : Programming
# Points: 100
# Author: @shiracamus

import socket
import sys
import string

HOST, PORT = 'number.quals.seccon.jp', 31337

def recv(s):
    data = ''
    while True:
        line = s.recv(80)
        if not line:
            exit()
        sys.stdout.write(line)
        data += line
        if 'SECCON{' in line:
            exit()
        if '?' in line or 'SECCON{' in line:
            return data

def solve(data):
    data, choice = map(string.strip, data.split('\n'))
    return min(eval(data)) if 'minimum' in choice else max(eval(data))

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
        s.send('%d\n' % ans)
