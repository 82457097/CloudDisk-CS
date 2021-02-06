#!/usr/bin/python
import socket
import time
i=0
while(True):
    i = i + 1
    sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    status = sock.connect_ex(('127.0.0.1', 8888))
    if status == 0:
	print('the port is OK')
    else:
	print('this port is not OK')

    if i < 1000:
        time.sleep(0.1)
    else:
        break
