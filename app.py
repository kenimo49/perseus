# -*- coding: utf-8 -*-
import subprocess
import socket
import string
import os
import random
import numpy as np
from numpy.random import *
import time

import jtalk

host = "localhost"
port = 10500

p = subprocess.Popen(["./julius_start.sh"], stdout=subprocess.PIPE, shell=True)
pid = str(p.stdout.read().decode('utf-8'))  # juliusのプロセスIDを取得
time.sleep(5)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((host, port))

data = ""
killword = ""

print("<<<please speak>>>")
while True:
    strTemp = ""
    for line in data.split('\n'):
        data = data + sock.recv(1024).decode('utf-8')
        index = line.find('WORD="')
        if index != -1:
            line = line[index + 6:line.find('"', index + 6)]
            if line != "[s]":
                strTemp = strTemp + line

        if strTemp == 'おはよう':
            if killword != 'おはよう':
                print(f"Result: {strTemp}")
                jtalk.jtalk('おはよう')
                print("<<<please speak>>>")
                killword = "おはよう"
        elif strTemp != '':
            print(f'strTemp: {strTemp}')
    else:
        data += str(sock.recv(1024).decode('utf-8'))
