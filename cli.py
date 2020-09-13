# Echo client program
import socket
from time import sleep
HOST = '192.168.0.200'    # The remote host
PORT = 3000             # The same port as used by the server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    sleep(1)
    while(1):
        hello=input("enter the command")+"\n";
        #hello="current\n"
        hello=hello.encode();
        s.send(hello)
        print("sent")
        data = s.recv(1024)
        print('Received', repr(data))

