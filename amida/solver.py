#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 summer online qualifications
# Title : Let's disassemble
# Genre : Programming
# Points: 300
# Author: @shiracamus

from subprocess import Popen, PIPE

def flip_xy(amida):
    r = [l.replace('-','/').replace('|','-').replace('/','|') for l in amida]
    return map(''.join, zip(*r))

def invert_vertical(amida):
    return amida[::-1]

def invert_horizontal(amida):
    return [line[::-1] for line in amida]

def shrink(amida):
    keep = map(amida[0].index, '12345678')
    return [''.join(line[x:x+2] for x in keep) for line in amida]

def normalize(amida):
    if amida[0][1] == '-':
        amida = flip_xy(amida)
    if '*' in amida[0]:
        amida = invert_vertical(amida)
    if amida[0][0] != '1':
        amida = invert_horizontal(amida)
    return shrink(amida)

def solve(amida):
    x = amida[-1].index('*')
    for line in amida[-2:0:-1]:
        if x > 0 and line[x-1] == '-':
            x -= 2
        elif x+1 < len(line) and line[x+1] == '-':
            x += 2
    return amida[0][x]  # '1'..'8'

def parse(reader):
    while True:
        amida = []
        while True:
            line = reader.read(2)
            if line == '? ':
                yield amida[1:]  # remove the first line "No.____"
                break
            line += reader.readline().replace('\n', '')
            print line
            if 'FLAG' in line:
                return
            amida += [line]

command = Popen('./amida', shell=True, stdin=PIPE, stdout=PIPE, stderr=PIPE)
for amida in parse(command.stdout):
    answer = solve(normalize(amida))
    print '?', answer
    command.stdin.write(answer + '\n')
