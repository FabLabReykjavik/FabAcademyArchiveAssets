#
# hello.tcpserver.py
#    TCP server hello-world
#
# Neil Gershenfeld 11/23/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
import socket,select
host = "127.0.0.1"
port = 12345
with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s:
   s.bind((host,port))
   s.listen()
   conn,addr = s.accept()
   while True:
      readable,writeable,exceptional = select.select([conn],[],[],0)
      for r in readable:
         data = r.recv(1024)
         r.sendall(data)
