#
# hello.MXD6235MP.py
#
# receive and display acceleration
#    hello.MXD6235MP.py serial_port
#
# Neil Gershenfeld 11/3/14
# (c) Massachusetts Institute of Technology 2014
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

from tkinter import *
import serial

WINDOW = 600 # window size

def idle(parent,canvas):
   global filter, eps
   #
   # idle routine
   #
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
   xoffl = ord(ser.read())
   xoffh= ord(ser.read())
   xonl = ord(ser.read())
   xonh= ord(ser.read())
   yoffl = ord(ser.read())
   yoffh= ord(ser.read())
   yonl = ord(ser.read())
   yonh= ord(ser.read())
   xoff = 256*xoffh+ xoffl
   xon = 256*xonh+ xonl
   xvalue = xoff/float(xon+xoff)
   x = int(.2*WINDOW + (.9-.2)*WINDOW*xvalue)
   canvas.itemconfigure("xtext",text="X %.2f"%xvalue)
   canvas.coords('xrect1',.2*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('xrect2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   yoff = 256*yoffh+ yoffl
   yon = 256*yonh+ yonl
   yvalue = yoff/float(yon+yoff)
   y = int(.2*WINDOW + (.9-.2)*WINDOW*yvalue)
   canvas.itemconfigure("ytext",text="Y %.2f"%yvalue)
   canvas.coords('yrect1',.2*WINDOW,.3*WINDOW,y,.45*WINDOW)
   canvas.coords('yrect2',y,.3*WINDOW,.9*WINDOW,.45*WINDOW)
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.accel.45.py serial_port")
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
ser = serial.Serial(port,9600)
ser.setDTR()
#
# set up GUI
#
root = Tk()
root.title('hello.MXD6235MP.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.5*WINDOW, background='white')
canvas.create_text(.1*WINDOW,.125*WINDOW,text=".33",font=("Helvetica", 24),tags="xtext",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.05*WINDOW,.3*WINDOW,.2*WINDOW, tags='xrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='xrect2', fill='#0000b0')
canvas.create_text(.1*WINDOW,.375*WINDOW,text=".33",font=("Helvetica", 24),tags="ytext",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.3*WINDOW,.3*WINDOW,.45*WINDOW, tags='yrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.3*WINDOW,.9*WINDOW,.45*WINDOW, tags='yrect2', fill='#0000b0')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
