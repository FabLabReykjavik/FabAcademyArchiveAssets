#
# hello.MPU-6050.py
#
# MPU-6050 IMU hello-world
#
# Neil Gershenfeld 10/20/24
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

from tkinter import *
import serial,sys

WINDOW = 1000 # window size

def idle(parent,canvas):
   roll = pitch= yaw = ax = ay = az = 0
   #
   # read data
   #
   s = ser.readline().decode('utf-8')
   try:
      yaw = float(s.split(',')[0])
      pitch = float(s.split(',')[1])
      roll = float(s.split(',')[2])
      ax = float(s.split(',')[3])
      ay = float(s.split(',')[4])
      az = float(s.split(',')[5])
   except:
      pass
   #
   #
   # plot
   #
   canvas.itemconfigure("roll",text=f"roll: {roll:.1f}")
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(0.5+0.5*roll/180))
   canvas.coords('roll1',.3*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('roll2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   canvas.itemconfigure("pitch",text=f"pitch: {pitch:.1f}")
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(0.5+0.5*pitch/180))
   canvas.coords('pitch1',.3*WINDOW,.25*WINDOW,x,.4*WINDOW)
   canvas.coords('pitch2',x,.25*WINDOW,.9*WINDOW,.4*WINDOW)
   canvas.itemconfigure("yaw",text=f"yaw: {yaw:.1f}")
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(0.5+0.5*yaw/180))
   canvas.coords('yaw1',.3*WINDOW,.45*WINDOW,x,.6*WINDOW)
   canvas.coords('yaw2',x,.45*WINDOW,.9*WINDOW,.6*WINDOW)
   canvas.itemconfigure("ax",text=f"ax: {ax:.0f}")
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(0.5+0.5*ax/12000))
   canvas.coords('ax1',.3*WINDOW,.65*WINDOW,x,.8*WINDOW)
   canvas.coords('ax2',x,.65*WINDOW,.9*WINDOW,.8*WINDOW)
   canvas.itemconfigure("ay",text=f"ay: {ay:.0f}")
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(0.5+0.5*ay/12000))
   canvas.coords('ay1',.3*WINDOW,.85*WINDOW,x,1*WINDOW)
   canvas.coords('ay2',x,.85*WINDOW,.9*WINDOW,1*WINDOW)
   canvas.itemconfigure("az",text=f"az: {az:.0f}")
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(0.5+0.5*az/12000))
   canvas.coords('az1',.3*WINDOW,1.05*WINDOW,x,1.2*WINDOW)
   canvas.coords('az2',x,1.05*WINDOW,.9*WINDOW,1.2*WINDOW)
   #
   # update
   #
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.MPU-6050.py serial_port")
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
ser = serial.Serial(port,115200)
#
# set up GUI
#
root = Tk()
root.title('hello.MPU-6050.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root,width=WINDOW,height=1.25*WINDOW,background='white')
#
canvas.create_text(.025*WINDOW,.125*WINDOW,text="1",font=("Helvetica",28),tags="roll",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.4*WINDOW,.2*WINDOW,tags='roll1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW,tags='roll2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.325*WINDOW,text="1",font=("Helvetica",28),tags="pitch",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.25*WINDOW,.4*WINDOW,.4*WINDOW,tags='pitch1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.25*WINDOW,.9*WINDOW,.4*WINDOW,tags='pitch2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.525*WINDOW,text="1",font=("Helvetica",28),tags="yaw",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.45*WINDOW,.4*WINDOW,.6*WINDOW,tags='yaw1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.45*WINDOW,.9*WINDOW,.6*WINDOW,tags='yaw2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.725*WINDOW,text="1",font=("Helvetica",28),tags="ax",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.65*WINDOW,.4*WINDOW,.8*WINDOW,tags='ax1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.65*WINDOW,.9*WINDOW,.8*WINDOW,tags='ax2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,.925*WINDOW,text="1",font=("Helvetica",28),tags="ay",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.85*WINDOW,.4*WINDOW,1*WINDOW,tags='ay1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.85*WINDOW,.9*WINDOW,1*WINDOW,tags='ay2', fill='#0000b0')
#
canvas.create_text(.025*WINDOW,1.125*WINDOW,text="1",font=("Helvetica",28),tags="az",fill="#0000b0",anchor=SW)
canvas.create_rectangle(.3*WINDOW,1.05*WINDOW,.4*WINDOW,1.2*WINDOW,tags='az1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,1.05*WINDOW,.9*WINDOW,1.2*WINDOW,tags='az2', fill='#0000b0')
#
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
