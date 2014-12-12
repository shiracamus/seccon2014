#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 summer online qualifications
# Title : Let's disassemble
# Genre : Programming
# Points: 300
# Author: @shiracamus

from random import randint

N = 1000
X = 8
Y = 19

def make():
    maze = [[0] * X for i in range(Y+1)]
    for y in range(1,len(maze)):
        for x in range(1, X):
            if maze[y-1][x] == 0 and maze[y][x-1] == 0:
                maze[y][x] = randint(0,1)
    goal = randint(1,X)

    print ' '.join(str(x+1) for x in range(X))
    for y in maze[1:]:
        m = int(''.join('01'[x] for x in y), 2)
        print '|'.join([' -'[x] for x in y])[1:]+'|'
    print ' '*(goal-1)*2+'*'
    print '? '

for i in range(N):
    make()
