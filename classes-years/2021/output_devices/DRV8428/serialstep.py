#
# serialstep.py
#    serial step-and-direction, 1 port
#
# Neil Gershenfeld  5/29/21
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
import serial,sys,time,signal
if (len(sys.argv) != 4):
   print("command line: serialstep.1.py port speed delay")
   sys.exit()
device = sys.argv[1]
baud = int(sys.argv[2])
delay = float(sys.argv[3])
print('open '+device+' at '+str(baud)+' delay '+str(delay))
port = serial.Serial(device,baudrate=baud,timeout=0)
count = 0;
maxcount = 5000;
forward = b'f'
reverse = b'r'
#
# alarm handler
#
def handler(signum,stack):
   global count
   count += 1
   if (count < maxcount/2):
      port.write(forward);
   elif (count < maxcount):
      port.write(reverse);
   else:
      count = 0
#
# start alarm
#
signal.signal(signal.SIGALRM,handler)
signal.setitimer(signal.ITIMER_REAL,1,delay)
#
# do nothing
#
while (1):
   0  
