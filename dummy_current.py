# Echo client program
import socket
from time import sleep
HOST = '192.168.0.18'    # The remote host
PORT = 3000
i=0
# The same port as used by the server
#while(1):
#    sleep(1)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        hello="CURRENT\n"
        hello=hello.encode();
        s.send(hello)
        i=i+1
        print("sent"+str(i))
        #data = s.recv(1024)
        #print('Received', repr(data))

