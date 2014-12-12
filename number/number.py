#!/usr/bin/env python
# -*- coding: utf-8 -*-

# For the SECCON CTF 2014 winter online qualifications
# Title : Choose the number
# Genre : Programming
# Points: 100
# Author: @shiracamus

import SocketServer
import socket
import random
from datetime import datetime
from time import sleep

HOST, PORT = '', 31337

FLAG = 'The flag is SECCON{Programming is so fun!}'

log = open('number.log', 'a')
no = 1

FUNCTIONS = ( ('min', 'The minimum number'),
              ('max', 'The maximum number') )

def make_numbers(n):
    return [ random.choice((1, -1)) *
             (random.randint(0, 0xffffffff) % (10 ** (n / 10 + 1)))
             for i in xrange(n) ]

class NumberHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        client = self.request
        client.settimeout(10)
        for i in range(1, 101):
            func = random.choice(FUNCTIONS)
            numbers = ', '.join(map(str, make_numbers(i + 1)))
            client.send('%s\n%s? ' % (numbers, func[1]))
            try:
                correct = str(eval(func[0] + '(' + numbers + ')'))
                answer = client.recv(1024).strip()
                if answer != correct:
                    client.send('Wrong, bye.\n')
                    return
            except socket.timeout as e:
                client.send('\nTimeout, bye.\n')
                return
            except socket.error:
                return
            except:
                return
            sleep(1)
        client.send('Congratulations!\n')
        client.send(FLAG + '\n')
	log.write( ('#%d ' % no)
                 + datetime.now().strftime('%Y/%m/%d %H:%M:%S\t')
                 + client.getpeername()[0]
		 + '\tsolved\n')
        log.flush()

class NumberServer(SocketServer.ThreadingTCPServer):
    def server_bind(self):
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind(self.server_address)

if __name__ == '__main__':
    server = NumberServer((HOST, PORT), NumberHandler)
    server.serve_forever()
