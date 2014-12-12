# For the SECCON CTF 2014 summer online qualifications
# Title : Let's disassemble
# Genre : Programming
# Points: 300
# Author: @shiracamus

all: amida

amida: amida.c amida.hash
	cc -Wall -O2 -Wl,-z,now,-z,relro -static -o amida amida.c -lcrypto
	strip amida

amida.data:
	python mkamida.py > amida.data

amida.hash: amida.data
	python mkhash.py > amida.hash
