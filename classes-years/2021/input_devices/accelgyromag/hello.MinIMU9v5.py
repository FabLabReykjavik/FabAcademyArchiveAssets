#
# hello.MinIMU9v5.py
#
# Pololu MinIMU-9 v5 hello-world
#    hello.MinIMU9v5.py serial_port
#
# Neil Gershenfeld 11/2/21
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

from tkinter import *
import serial

window = 500 # window size
accel_scale = 5000 # accelerometer scale
gyro_scale = 10000 # gyro scale
mag_scale = 32768 # magnetometer scale
eps = 0.05 # filter fraction
ax = ay = az = 0
gx = gy = gz = 0
mx = my = mz = 0

def update(var):
   global eps
   line = ser.readline().rstrip()
   return (1-eps)*var+eps*int(line)
   
def idle(parent,canvas):
   global ax,ay,az,gx,gy,gz,mx,my,mz
   #
   # find start
   #
   ser.flush();
   while (True):
      line = ser.readline().rstrip()
      if (line == b'start'):
         break
   #
   # read data
   #
   ax = update(ax)
   ay = update(ay)
   az = update(az)
   gx = update(gx)
   gy = update(gy)
   gz = update(gz)
   mx = update(mx)
   my = update(my)
   mz = update(mz)
   #
   # plot
   #
   canvas.itemconfigure("tax",text="ax: %d"%ax)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*ax/accel_scale))
   canvas.coords('rax1',.3*window,.05*window,x,.2*window)
   canvas.coords('rax2',x,.05*window,.9*window,.2*window)
   canvas.itemconfigure("tay",text="ay: %d"%ay)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*ay/accel_scale))
   canvas.coords('ray1',.3*window,.25*window,x,.4*window)
   canvas.coords('ray2',x,.25*window,.9*window,.4*window)
   canvas.itemconfigure("taz",text="az: %d"%az)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*az/accel_scale))
   canvas.coords('raz1',.3*window,.45*window,x,.6*window)
   canvas.coords('raz2',x,.45*window,.9*window,.6*window)
   canvas.itemconfigure("tgx",text="rx: %d"%gx)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*gx/gyro_scale))
   canvas.coords('rgx1',.3*window,.65*window,x,.8*window)
   canvas.coords('rgx2',x,.65*window,.9*window,.8*window)
   canvas.itemconfigure("tgy",text="ry: %d"%gy)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*gy/gyro_scale))
   canvas.coords('rgy1',.3*window,.85*window,x,1*window)
   canvas.coords('rgy2',x,.85*window,.9*window,1*window)
   canvas.itemconfigure("tgz",text="rz: %d"%gz)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*gz/gyro_scale))
   canvas.coords('rgz1',.3*window,1.05*window,x,1.2*window)
   canvas.coords('rgz2',x,1.05*window,.9*window,1.2*window)
   canvas.itemconfigure("tmx",text="mx: %d"%mx)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*mx/mag_scale))
   canvas.coords('rmx1',.3*window,1.25*window,x,1.4*window)
   canvas.coords('rmx2',x,1.25*window,.9*window,1.4*window)
   canvas.itemconfigure("tmy",text="my: %d"%my)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*my/mag_scale))
   canvas.coords('rmy1',.3*window,1.45*window,x,1.6*window)
   canvas.coords('rmy2',x,1.45*window,.9*window,1.6*window)
   canvas.itemconfigure("tmz",text="mz: %d"%mz)
   x = int(.3*window + (.9-.3)*window*(.5+0.5*mz/mag_scale))
   canvas.coords('rmz1',.3*window,1.65*window,x,1.8*window)
   canvas.coords('rmz2',x,1.65*window,.9*window,1.8*window)
   #
   # update
   #
   canvas.update()
   parent.after_idle(idle,parent,canvas)
   
#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.MinIMU9v5.py serial_port")
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
root.title('hello.MinIMU9v5.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=window, height=1.85*window, background='white')
font_size = 22
#
canvas.create_text(.025*window,.125*window,text="1",font=("Helvetica", font_size),tags="tax",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,.05*window,.4*window,.2*window, tags='rax1', fill='#b00000')
canvas.create_rectangle(.4*window,.05*window,.9*window,.2*window, tags='rax2', fill='#0000b0')
#
canvas.create_text(.025*window,.325*window,text="1",font=("Helvetica", font_size),tags="tay",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,.25*window,.4*window,.4*window, tags='ray1', fill='#b00000')
canvas.create_rectangle(.4*window,.25*window,.9*window,.4*window, tags='ray2', fill='#0000b0')
#
canvas.create_text(.025*window,.525*window,text="1",font=("Helvetica", font_size),tags="taz",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,.45*window,.4*window,.6*window, tags='raz1', fill='#b00000')
canvas.create_rectangle(.4*window,.45*window,.9*window,.6*window, tags='raz2', fill='#0000b0')
#
canvas.create_text(.025*window,.725*window,text="1",font=("Helvetica", font_size),tags="tgx",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,.65*window,.4*window,.8*window, tags='rgx1', fill='#b00000')
canvas.create_rectangle(.4*window,.65*window,.9*window,.8*window, tags='rgx2', fill='#0000b0')
#
canvas.create_text(.025*window,.925*window,text="1",font=("Helvetica", font_size),tags="tgy",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,.85*window,.4*window,1*window, tags='rgy1', fill='#b00000')
canvas.create_rectangle(.4*window,.85*window,.9*window,1*window, tags='rgy2', fill='#0000b0')
#
canvas.create_text(.025*window,1.125*window,text="1",font=("Helvetica", font_size),tags="tgz",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,1.05*window,.4*window,1.2*window, tags='rgz1', fill='#b00000')
canvas.create_rectangle(.4*window,1.05*window,.9*window,1.2*window, tags='rgz2', fill='#0000b0')
#
canvas.create_text(.025*window,1.325*window,text="1",font=("Helvetica", font_size),tags="tmx",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,1.25*window,.4*window,1.4*window, tags='rmx1', fill='#b00000')
canvas.create_rectangle(.4*window,1.25*window,.9*window,1.4*window, tags='rmx2', fill='#0000b0')
#
canvas.create_text(.025*window,1.525*window,text="1",font=("Helvetica", font_size),tags="tmy",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,1.45*window,.4*window,1.6*window, tags='rmy1', fill='#b00000')
canvas.create_rectangle(.4*window,1.45*window,.9*window,1.6*window, tags='rmy2', fill='#0000b0')
#
canvas.create_text(.025*window,1.725*window,text="1",font=("Helvetica", font_size),tags="tmz",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*window,1.65*window,.4*window,1.8*window, tags='rmz1', fill='#b00000')
canvas.create_rectangle(.4*window,1.65*window,.9*window,1.8*window, tags='rmz2', fill='#0000b0')
#
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
