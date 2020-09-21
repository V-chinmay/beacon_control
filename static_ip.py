from pyroute2 import IPRoute
import socket

def set_static_ip():
    ip_addr=(([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")] or [[(s.connect(("8.8.8.8", 53)), s.getsockname()[0], s.close()) for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) + ["no IP found"])[0]

    ip_seg=ip_addr.split('.')
    ip_seg[3]='200'
    ip_addr='.'.join(ip_seg)
    #os.environ["STATIC_BOT_IP"]=ip_addr

    #os.system("echo export STATIC_BOT_IP=\'{}\' >> //home//sirena//.bashrc".format(ip_addr))
    #print(os.environ.get("STATIC_BOT_IP"))

    print("ip that will be set::"+ip_addr)

    try:
        ip = IPRoute()
        index = ip.link_lookup(ifname='wlp2s0')[0]
        ip.addr('add', index, address=ip_addr, mask=24)
        ip.close()
        print("Successfully set the static ip")
        return(ip_addr)
    except Exception as e:
        print(e)
        print("failed to set static ip!!")
        return("127.0.1.1")


