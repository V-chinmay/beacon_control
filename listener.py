# Echo client program
import socket
from time import sleep
import static_ip

ip_addr=static_ip.set_static_ip()
ip_addr += "\n"
print("will connect!!")
HOST = '127.0.1.1'    # The remote host
PORT = 2800             # The same port as used by the server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print("connectting")
    s.connect((HOST, PORT))
    sleep(1)
    hello=ip_addr.encode();
    s.send(hello)
    while(1):
        #hello=input("enter the command")+"\n";
        #        print("sent")
        print("waiting")
        data = s.recv(1024)
        print('Received', repr(data))

