#
# hello.ALS31313.t412.py
#    vector magnetometer tiny412 hello-world
#       hello.ALS31313.t412.py serial_port
#
# Neil Gershenfeld 12/19/21
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
nsamples = 100.0 # number of samples accumulated
xf = yf = zf = 0 # filtered values
eps = 0.01 # filter fraction
scale = 200 # bar graph scale

def idle(parent,canvas):
   global xf,yf,zf
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
   y0 = ord(ser.read())
   z0 = ord(ser.read())
   x1 = ord(ser.read())
   y1 = ord(ser.read())
   z1 = ord(ser.read())
   x = (x0 << 4) + (x1 & 0b1111)
   y = (y0 << 4) + (y1 & 0b1111)
   z = (z0 << 4) + (z1 & 0b1111)
   if (x > 2047):
      x = x-4096
   if (y > 2047):
      y = y-4096
   if (z > 2047):
      z = z-4096
   xf = (1-eps)*xf+eps*x
   yf = (1-eps)*yf+eps*y
   zf = (1-eps)*zf+eps*z
   canvas.itemconfigure("xtext",text="%.1f"%xf)
   canvas.itemconfigure("ytext",text="%.1f"%yf)
   canvas.itemconfigure("ztext",text="%.1f"%zf)
   xp = (0.2+(0.9-0.2)/2.0)*WINDOW+((0.9-0.2)/2)*WINDOW*xf/scale
   yp = (0.2+(0.9-0.2)/2.0)*WINDOW+((0.9-0.2)/2)*WINDOW*yf/scale
   zp = (0.2+(0.9-0.2)/2.0)*WINDOW+((0.9-0.2)/2)*WINDOW*zf/scale
   canvas.coords('xrect1',.2*WINDOW,.05*WINDOW,xp,.2*WINDOW)
   canvas.coords('xrect2',xp,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   canvas.coords('yrect1',.2*WINDOW,.25*WINDOW,yp,.4*WINDOW)
   canvas.coords('yrect2',yp,.25*WINDOW,.9*WINDOW,.4*WINDOW)
   canvas.coords('zrect1',.2*WINDOW,.45*WINDOW,zp,.6*WINDOW)
   canvas.coords('zrect2',zp,.45*WINDOW,.9*WINDOW,.6*WINDOW)
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.ALS31313.t412.py serial_port")
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
root.title('hello.ALS31313.t412.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.65*WINDOW, background='white')
#
canvas.create_text(.1*WINDOW,.125*WINDOW,text="",font=("Helvetica", 24),tags="xtext",fill="#0000b0")
canvas.create_text(.95*WINDOW,.125*WINDOW,text="x",font=("Helvetica", 24),fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.05*WINDOW,.3*WINDOW,.2*WINDOW, tags='xrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='xrect2', fill='#0000b0')
#
canvas.create_text(.1*WINDOW,.325*WINDOW,text="",font=("Helvetica", 24),tags="ytext",fill="#0000b0")
canvas.create_text(.95*WINDOW,.325*WINDOW,text="y",font=("Helvetica", 24),fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.25*WINDOW,.3*WINDOW,.4*WINDOW, tags='yrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.25*WINDOW,.9*WINDOW,.4*WINDOW, tags='yrect2', fill='#0000b0')
#
canvas.create_text(.1*WINDOW,.525*WINDOW,text="",font=("Helvetica", 24),tags="ztext",fill="#0000b0")
canvas.create_text(.95*WINDOW,.525*WINDOW,text="z",font=("Helvetica", 24),fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.45*WINDOW,.3*WINDOW,.6*WINDOW, tags='zrect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.45*WINDOW,.9*WINDOW,.6*WINDOW, tags='zrect2', fill='#0000b0')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
