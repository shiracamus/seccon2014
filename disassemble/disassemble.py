#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 winter online qualifications
# Title : Let's disassemble
# Genre : Binary
# Points: 200
# Author: @shiracamus

import SocketServer
import socket
import random
import sys
import re
from datetime import datetime
from time import sleep

HOST, PORT = '', 23168
KEY = 'The flag is SECCON{I love Z80. How about you?}'

def load_instructions():
    def split(line):
        code, mnemonic = line.split('  ', 1)
        return code.strip(), mnemonic.strip()
    with open('Z80.txt') as f:
        return [split(line) for line in f if line[0] != '#']

instructions = load_instructions()

log = open("disassemble.log", "a")

def logging(message):
    log.write(datetime.now().strftime('%Y/%m/%d %H:%M:%S\t') + message)
    log.flush()

re_hexx = re.compile('0[xX][0-9a-fA-F]+')
re_hexh = re.compile('[0-9a-fA-F]+h')
re_hexd = re.compile('\$[0-9a-fA-F]+')
re_hexa = re.compile('\&[0-9a-fA-F]+')

def hexx2int(m): return str(int(m.group(), 16))
def hexh2int(m): return str(int(m.group()[:-1], 16))
def hexd2int(m): return str(int(m.group()[1:], 16))
def hexa2int(m): return str(int(m.group()[1:], 16))

def hex2int(s):
    s = re_hexx.sub(hexx2int, s)
    s = re_hexh.sub(hexh2int, s)
    s = re_hexd.sub(hexd2int, s)
    s = re_hexa.sub(hexa2int, s)
    return s

def format(code, mnemonic):
    if 'nn' in mnemonic:
        nn = random.randint(0, 0xffff)
        code = code.replace('nn', '%02X %02X' % (nn & 0xff, nn >> 8))
        mnemonic = mnemonic.replace('nn', "0x%04X" % nn)
    if 'n' in mnemonic:
        n = random.randint(0, 0xff)
        code = code.replace('n', "%02X" % n)
        mnemonic = mnemonic.replace('n', "0x%02X" % n)
    if 'p' in mnemonic:
        p = random.randint(0, 0xff)
        code = code.replace('p', '%02X' % p)
        mnemonic = mnemonic.replace('p', '0x%02X' % p)
    if '+d' in mnemonic:
        d = random.randint(-128, 127)
        code = code.replace('d', '%02X' % (d & 0xff))
        mnemonic = mnemonic.replace('+d', "%+d" % d)
    return code, mnemonic 

def choose_instruction(i):
    while True:
        choice = random.randint(0, len(instructions)-1)
        code, asm = format(*instructions[choice])
        code0 = code.split()[0]
        if i > 50 or (code0 not in ( 'CB', 'DD', 'ED', 'FD' )):
            return code, asm

class DisassembleHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        client = self.request
        client.settimeout(10)
        addr = client.getpeername()[0]
        for i in range(1, 101):
            code, asm = choose_instruction(i)
            client.send('#%d : %s\n? ' % (i, code))
            try:
                answer = client.recv(80).strip()
                answer = ' '.join(answer.split())
                answer = answer.replace(', ', ',')
                answer = hex2int(answer.lower())
                asm = hex2int(asm.lower())
                if answer != asm:
                    client.send('Something wrong, bye.\n')
                    if i > 1 and answer:
                        logging('%s\tfailed\t#%d: %-12s "%s" != "%s"\n' % (addr, i, code, asm, answer))
                    return
            except socket.timeout as e:
                client.send('\nTimeout, bye.\n')
                return
            except socket.error:
                return
            sleep(1)
        client.send('Congratulations!\n')
        client.send('%s\n' % KEY)
        logging('%s\tsolved\n' % addr)

class DisassembleServer(SocketServer.ThreadingTCPServer):
    def server_bind(self):
	self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind(self.server_address)

if __name__ == '__main__':
    if len(sys.argv) > 1:
	PORT = int(sys.argv[1])
    server = DisassembleServer((HOST, PORT), DisassembleHandler)
    server.serve_forever()
