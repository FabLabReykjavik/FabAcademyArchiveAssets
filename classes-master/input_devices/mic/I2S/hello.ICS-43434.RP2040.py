#
# hello.ICS-43434.RP2040.py
#
# plot ICS-43434 microphone audio
#    hello.ICS-43434.RP2040.py serial_port
#
# Neil Gershenfeld 11/5/24
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

from tkinter import *
import serial,sys

NX = 1000
NY = 1000
nsamples = 1000
scale = 5
offset = NY/2
path = []

def idle(parent,canvas):
   #
   # idle routine
   #
   global path 
   #
   # look for framing
   #
   result = ser.readline().decode("utf-8").rstrip()
   if (result != "frame"):
      parent.after_idle(idle,parent,canvas)
      return
   #
   # framing found, loop over points
   #
   path = []
   for i in range(nsamples):
      #
      # read samples
      #
      sample = int(ser.readline().decode("utf-8").rstrip())
      #
      # two's complement conversion
      #
      sample = sample >> 8
      if (sample > 0x7FFF):
         sample = sample-0xFFFF-1
      #
      # add to plot
      #
      value = offset+sample/scale
      path.append(i*NY/float(nsamples))
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
   print("command line: hello.ICS-43434.RP2040.py serial_port")
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
root.title('hello.ICS-43434.RP2040.py')
root.bind('q','exit')
canvas = Canvas(root, width=NX, height=NY, background='white')
canvas.pack()
root.after(100,idle,root,canvas)
root.mainloop()
