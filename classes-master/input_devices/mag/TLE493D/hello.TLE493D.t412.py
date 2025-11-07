#!/usr/bin/env python3
#
# hello.TLE493D.t412.py
#    TLE493D vector magnetometer tiny412 hello-world
#
# Neil Gershenfeld 12/22/21
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#
from tkinter import *
import serial,sys
#
window = 600 # window size
xf = yf = zf = 0 # filtered values
eps = 0.1 # filter fraction
xyscale = 200 # XY drawing scale
zscale = 500 # Z drawing scale
#
# circle drawing limits
#
def limits(x,y,r):
   xmin = x*window-r*window
   ymin = (1-y)*window-r*window
   xmax = x*window+r*window
   ymax = (1-y)*window+r*window
   return(xmin,ymin,xmax,ymax)
#
# idle routine
#
def idle(parent,canvas):
   global xf,yf,zf
   byte2 = 0
   byte3 = 0
   byte4 = 0
   ser.flush()
   while 1:
      #
      # find framing 
      #
      byte1 = byte2
      byte2 = byte3
      byte3 = byte4
      byte4 = ord(ser.read())
      if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)):
         break
   #
   # read data
   #
   v0 = ord(ser.read())
   v1 = ord(ser.read())
   v2 = ord(ser.read())
   v3 = ord(ser.read())
   v4 = ord(ser.read())
   v5 = ord(ser.read())
   x = (v0 << 4)+((v4 & 0xF0) >> 4)
   y = (v1 << 4)+(v4 & 0x0F)
   z = (v2 << 4)+(v5 & 0x0F)
   if (x > 2047):
      x = x-4096
   if (y > 2047):
      y = y-4096
   if (z > 2047):
      z = z-4096
   xf = (1-eps)*xf+eps*x
   yf = (1-eps)*yf+eps*y
   zf = (1-eps)*zf+eps*z
   #
   # update canvas
   #
   canvas.coords('circle',limits(0.5+xf/xyscale,0.5+yf/xyscale,zf/zscale))
   canvas.coords('center',limits(0.5+xf/xyscale,0.5+yf/xyscale,0.01))
   canvas.update()
   #
   # call idle
   #
   parent.after_idle(idle,parent,canvas)
#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.TLE493D.t412.py serial_port")
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
ser = serial.Serial(port,115200)
ser.setDTR()
#
# set up GUI
#
root = Tk()
root.title('hello.TLE493D.t412.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root,width=window,height=window,background='white')
#
canvas.create_oval(limits(0.5,0.5,0.5),fill='blue',tags='circle')
canvas.create_oval(limits(0.5,0.5,0.01),fill='red',tags='center')
canvas.create_oval(limits(0.5,0.5,0.01),fill='black',tags='origin')
#
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
