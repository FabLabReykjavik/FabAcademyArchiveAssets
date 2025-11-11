#
# hello.LSM6DS33.py
#
# LSM6DS33 time of flight hello-world
#    hello.LSM6DS33.py serial_port
#
# Neil Gershenfeld 11/01/20
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

from tkinter import *
import serial

WINDOW = 600 # window size
eps = 0.005 # filter fraction
ax = ay = az = 0
gx = gy = gz = 0

def update(value,lo,hi,eps):
   if (hi < 128):
      return((1-eps)*value+eps*(256*hi+lo))
   else:
      return((1-eps)*value+eps*((256*hi+lo)-(256*255+255)))

def idle(parent,canvas):
   global filt,eps,ax,ay,az,gx,gy,gz
   #
   # idle routine
   #
   byte2 = 0
   byte3 = 0
   byte4 = 0
   ser.flush()
   #
   # find framing 
   #
   while 1:
      byte1 = byte2
      byte2 = byte3
      byte3 = byte4
      byte4 = ord(ser.read())
      if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)):
         break
   #
   # read
   #
   ax_low = ord(ser.read())
   ax_high = ord(ser.read())
   ay_low = ord(ser.read())
   ay_high = ord(ser.read())
   az_low = ord(ser.read())
   az_high = ord(ser.read())
   gx_low = ord(ser.read())
   gx_high = ord(ser.read())
   gy_low = ord(ser.read())
   gy_high = ord(ser.read())
   gz_low = ord(ser.read())
   gz_high = ord(ser.read())
   #
   # update
   #
   ax = update(ax,ax_low,ax_high,eps)
   ay = update(ay,ay_low,ay_high,eps)
   az = update(az,az_low,az_high,eps)
   gx = update(gx,gx_low,gx_high,eps)
   gy = update(gy,gy_low,gy_high,eps)
   gz = update(gz,gz_low,gz_high,eps)
   #
   #
   # plot
   #
   canvas.itemconfigure("tax",text="ax: %d"%ax)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(.5+0.5*ax/32768))
   canvas.coords('rax1',.3*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('rax2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   canvas.itemconfigure("tay",text="ay: %d"%ay)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(.5+0.5*ay/32768))
   canvas.coords('ray1',.3*WINDOW,.25*WINDOW,x,.4*WINDOW)
   canvas.coords('ray2',x,.25*WINDOW,.9*WINDOW,.4*WINDOW)
   canvas.itemconfigure("taz",text="az: %d"%az)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(.5+0.5*az/32768))
   canvas.coords('raz1',.3*WINDOW,.45*WINDOW,x,.6*WINDOW)
   canvas.coords('raz2',x,.45*WINDOW,.9*WINDOW,.6*WINDOW)
   canvas.itemconfigure("tgx",text="rx: %d"%gx)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(.5+0.5*gx/32768))
   canvas.coords('rgx1',.3*WINDOW,.65*WINDOW,x,.8*WINDOW)
   canvas.coords('rgx2',x,.65*WINDOW,.9*WINDOW,.8*WINDOW)
   canvas.itemconfigure("tgy",text="ry: %d"%gy)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(.5+0.5*gy/32768))
   canvas.coords('rgy1',.3*WINDOW,.85*WINDOW,x,1*WINDOW)
   canvas.coords('rgy2',x,.85*WINDOW,.9*WINDOW,1*WINDOW)
   canvas.itemconfigure("tgz",text="rz: %d"%gz)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(.5+0.5*gz/32768))
   canvas.coords('rgz1',.3*WINDOW,1.05*WINDOW,x,1.2*WINDOW)
   canvas.coords('rgz2',x,1.05*WINDOW,.9*WINDOW,1.2*WINDOW)
   #
   # update
   #
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.LSM6DS33.py serial_port")
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
root.title('hello.LSM6DS33.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=1.25*WINDOW, background='white')
#
canvas.create_text(.025*WINDOW,.125*WINDOW,text="1",font=("Helvetica", 24),tags="tax",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.4*WINDOW,.2*WINDOW, tags='rax1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='rax2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.325*WINDOW,text="1",font=("Helvetica", 24),tags="tay",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.25*WINDOW,.4*WINDOW,.4*WINDOW, tags='ray1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.25*WINDOW,.9*WINDOW,.4*WINDOW, tags='ray2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.525*WINDOW,text="1",font=("Helvetica", 24),tags="taz",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.45*WINDOW,.4*WINDOW,.6*WINDOW, tags='raz1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.45*WINDOW,.9*WINDOW,.6*WINDOW, tags='raz2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.725*WINDOW,text="1",font=("Helvetica", 24),tags="tgx",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.65*WINDOW,.4*WINDOW,.8*WINDOW, tags='rgx1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.65*WINDOW,.9*WINDOW,.8*WINDOW, tags='rgx2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.925*WINDOW,text="1",font=("Helvetica", 24),tags="tgy",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.85*WINDOW,.4*WINDOW,1*WINDOW, tags='rgy1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.85*WINDOW,.9*WINDOW,1*WINDOW, tags='rgy2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,1.125*WINDOW,text="1",font=("Helvetica", 24),tags="tgz",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,1.05*WINDOW,.4*WINDOW,1.2*WINDOW, tags='rgz1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,1.05*WINDOW,.9*WINDOW,1.2*WINDOW, tags='rgz2', fill='#0000b0')
#
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
