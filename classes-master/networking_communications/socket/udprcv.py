#
# udprcv.py port
# Neil Gershenfeld 11/25/05
#

from socket import *
import sys

sock = socket(AF_INET,SOCK_DGRAM)
port = int(sys.argv[1])
sock.bind(("",port))
bufsize = 1024
while 1:
   (data,addr) = sock.recvfrom(1024)
   print('received "%s" from %s'%(data,addr[0]))
sock.close()
