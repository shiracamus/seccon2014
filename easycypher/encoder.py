#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 winter online qualifications
# Title : Easy cypher
# Genre : Crypto
# Points: 100
# Author: @shiracamus

import random

for c in open('message.txt').read():
    if c == '\n':
        print
        continue
    cypher = [str, oct, lambda code: bin(code)[2:]]
    if any(h in 'abcdef' for h in hex(ord(c))):
        cypher += [lambda code: hex(code)[2:]]
    print random.choice(cypher)(ord(c)),
