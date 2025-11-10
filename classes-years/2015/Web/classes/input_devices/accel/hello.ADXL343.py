#
# hello.ADXL343.py
#
# receive and display acceleration
#    hello.ADXL343.py serial_port
#
# Neil Gershenfeld 11/8/15
# (c) Massachusetts Institute of Technology 2015
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

from Tkinter import *
import serial

WINDOW = 600 # window size
EPS = 0.1 # filter
xfilt = yfilt = zfilt = 0

def idle(parent,canvas):
   global xfilt,yfilt,zfilt
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
   x0 = ord(ser.read())
   x1 = ord(ser.read())
   y0 = ord(ser.read())
   y1 = ord(ser.read())
   z0 = ord(ser.read())
   z1 = ord(ser.read())
   x = x0+255*x1
   if (0x8000 & x): 
      x = -(0x10000-x)
   xfilt = (1-EPS)*xfilt+EPS*x
   canvas.itemconfigure("xtext",text="X %.0f"%xfilt)
   xdisp = int(.2*WINDOW + WINDOW*(.35+.35*xfilt/1024.0))
   canvas.coords('xrect1',.2*WINDOW,.05*WINDOW,xdisp,.2*WINDOW)
   canvas.coords('xrect2',xdisp,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   y = y0+255*y1
   if (0x8000 & y): 
      y = -(0x10000-y)
   yfilt = (1-EPS)*yfilt+EPS*y
   canvas.itemconfigure("ytext",text="Y %.0f"%yfilt)
   ydisp = int(.2*WINDOW + WINDOW*(.35+.35*yfilt/1024.0))
   canvas.coords('yrect1',.2*WINDOW,.3*WINDOW,ydisp,.45*WINDOW)
   canvas.coords('yrect2',ydisp,.3*WINDOW,.9*WINDOW,.45*WINDOW)
   z = z0+255*z1
   if (0x8000 & z): 
      z = -(0x10000-z)
   zfilt = (1-EPS)*zfilt+EPS*z
   canvas.itemconfigure("ztext",text="Z %.0f"%zfilt)
   zdisp = int(.2*WINDOW + WINDOW*(.35+.35*zfilt/1024.0))
   canvas.coords('zrect1',.2*WINDOW,.55*WINDOW,zdisp,.7*WINDOW)
   canvas.coords('zrect2',zdisp,.55*WINDOW,.9*WINDOW,.7*WINDOW)
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print "command line: hello.accel.45.py serial_port"
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
root.title('hello.ADXL343.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.75*WINDOW, background='white')
canvas.create_text(.1*WINDOW,.125*WINDOW,text=".33",font=("Helvetica", 24),tags="xtext",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.05*WINDOW,.3*WINDOW,.2*WINDOW, tags='xrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='xrect2', fill='#0000b0')
canvas.create_text(.1*WINDOW,.375*WINDOW,text=".33",font=("Helvetica", 24),tags="ytext",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.3*WINDOW,.3*WINDOW,.45*WINDOW, tags='yrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.3*WINDOW,.9*WINDOW,.45*WINDOW, tags='yrect2', fill='#0000b0')
canvas.create_text(.1*WINDOW,.625*WINDOW,text=".33",font=("Helvetica", 24),tags="ztext",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.55*WINDOW,.3*WINDOW,.7*WINDOW, tags='zrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.55*WINDOW,.9*WINDOW,.7*WINDOW, tags='zrect2', fill='#0000b0')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
