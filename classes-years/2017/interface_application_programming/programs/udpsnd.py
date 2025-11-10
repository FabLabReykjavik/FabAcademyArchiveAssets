#
# udpsnd.py host port data
# Neil Gershenfeld 11/25/05
#

from socket import *
import sys

sock = socket(AF_INET,SOCK_DGRAM)
host = sys.argv[1]
port = int(sys.argv[2])
data = sys.argv[3]
sock.sendto(data,(host,port))
sock.close()
