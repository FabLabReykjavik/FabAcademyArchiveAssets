#
# hello.CMM-4030D-261-I2S-TR.py
#
# plot CMM-4030D-261-I2S-TR microphone audio
#    hello.CMM-4030D-261-I2S-TR serial_port
#
# Neil Gershenfeld 12/27/20
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

from tkinter import *
import serial

NX = 500 
NY = 500
nloop = 200
scale = 500
offset = NY/2
path = []

def idle(parent,canvas):
   global path, baseline
   #
   # idle routine
   #
   # look for framing
   #
   byte2 = 0
   byte3 = 0
   byte4 = 0
   while 1:
      byte1 = byte2
      byte2 = byte3
      byte3 = byte4
      byte4 = ord(ser.read())
      if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)):
         break
   path = []
   #
   # framing found, loop over points
   #
   for i in range(nloop):
      #
      # read bytes
      #
      lobyte = ord(ser.read())
      midbyte = ord(ser.read())
      hibyte = ord(ser.read())
      #
      # two's complement conversion
      #
      reading = 256*256*hibyte+256*midbyte+lobyte
      if (reading >= pow(2,23)):
         reading = reading-pow(2,24)
      #
      # add to plot
      #
      value = offset+reading/scale
      path.append(i*NY/float(nloop))
      path.append(value)
   #
   # udpate canvas
   #
   canvas.delete("path")
   canvas.create_line(path,tag="path",width=3,fill="#00b000")
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.CMM-4030D-261-I2S-TR.py serial_port")
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
ser = serial.Serial(port,115200)
#
# start plotting
#
root = Tk()
root.title('hello.CMM-4030D-261-I2S-TR.py')
root.bind('q','exit')
canvas = Canvas(root, width=NX, height=NY, background='white')
canvas.pack()
root.after(100,idle,root,canvas)
root.mainloop()
