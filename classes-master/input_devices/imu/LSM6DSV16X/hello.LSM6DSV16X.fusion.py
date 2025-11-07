#
# hello.LSM6DSV16X.fusion.py
#    LSM6DSV16X IMU hello-world
#
# Neil Gershenfeld 5/19/25
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
from tkinter import *
import serial,sys,math
#
WIDTH = 750 # window width
NBARS = 12 # number of bar graphs
BARHEIGHT = 70 # bar graph height
HEIGHT = NBARS*BARHEIGHT # window height
BORDER = 10 # bar graph border
FONTSIZE = 28 # font size
EPS = 0.2 # smoothing filter
filter = {}
gx = gy = 0
gz = 1
barcount = 0
#
def idle(parent,canvas):
   global gx,gy,gz
   #
   # read a line
   #
   line = serialport.readline().decode('utf-8').rstrip()
   #
   # parse and plot
   #
   line = line.split(':')
   match line[0]:
      case "ax ay az": # acceleration
         try:
            line = line[1].split(',')
            line = list(map(int,line))
            ax = line[0]
            ay = line[1]
            az = line[2]
            ax *= 2/32768 # normalize 2g full scale
            ay *= 2/32768 # normalize 2g full scale
            az *= 2/32768 # normalize 2g full scale
            ax -= gx # subtract gravity
            ay -= gy # subtract gravity
            az -= gz # subtract gravity
            update_bar_graph("ax(g)",ax,-1,1)
            update_bar_graph("ay(g)",ay,-1,1)
            update_bar_graph("az(g)",az,-1,1)
         except:
            pass
      case "gx gy gz": # gravity
         try:
            line = line[1].split(',')
            line = list(map(int,line))
            gx = line[0]
            gy = line[1]
            gz = line[2]
            norm = math.sqrt(gx*gx+gy*gy+gz*gz)
            gx /= norm # normalize to 1g
            gy /= norm # normalize to 1g
            gz /= norm # normalize to 1g
            update_bar_graph("gx(g)",gx,-1,1)
            update_bar_graph("gy(g)",gy,-1,1)
            update_bar_graph("gz(g)",gz,-1,1)
         except:
            pass
      case "rx ry rz": # rotation
         try:
            line = line[1].split(',')
            line = list(map(int,line))
            rx = line[0]
            ry = line[1]
            rz = line[2]
            rx *= 1000/32768 # normalize to 1000 dps full scale
            ry *= 1000/32768 # normalize to 1000 dps full scale
            rz *= 1000/32768 # normalize to 2000 dps full scale
            update_bar_graph("rx(dps)",rx,-1000,1000)
            update_bar_graph("ry(dps)",ry,-1000,1000)
            update_bar_graph("rz(dps)",rz,-1000,1000)
         except:
            pass
      case "qw qx qy qz": # quaternions
         try:
            line = line[1].split(',')
            line = list(map(float,line))
            qw = line[0]
            qx = line[1]
            qy = line[2]
            qz = line[3]
            roll = math.atan2(
               2*qy*qw-2*qx*qz,
               1-2*qy*qy-2*qz*qz)
            yaw = math.asin(
               2*qx*qy+2*qz*qw)
            pitch = math.atan2(
               2*qx*qw-2*qy*qz,
               1-2*qx*qx-2*qz*qz)
            yaw = 180*yaw/math.pi
            pitch = 180*pitch/math.pi
            roll = 180*roll/math.pi
            update_bar_graph("yaw(deg)",yaw,-90,90)
            update_bar_graph("pitch(deg)",pitch,-180,180)
            update_bar_graph("roll(deg)",roll,-180,180)
         except:
            pass
   #
   # update canvas
   #
   canvas.update()
   parent.after_idle(idle,parent,canvas)
#
def update_bar_graph(label,value,vmin,vmax):
   filt = EPS*value+(1-EPS)*filter[label]
   filter[label] = filt
   canvas.itemconfigure(label,text=f"{label}: {filt:.1f}")
   tuple = canvas.coords(label+'r')
   tuple[2] = BORDER+(WIDTH-2*BORDER)*(filt-vmin)/(vmax-vmin)
   canvas.coords(label+'r',tuple)
#
def create_bar_graph(label):
   global barcount
   n = barcount
   barcount += 1
   canvas.create_rectangle(BORDER,n*BARHEIGHT+BORDER,WIDTH-BORDER,(n+1)*BARHEIGHT-BORDER,tags=label+'b', fill='#0000b0')
   canvas.create_rectangle(BORDER,n*BARHEIGHT+BORDER,BORDER,(n+1)*BARHEIGHT-BORDER,tags=label+'r', fill='#b00000')
   canvas.create_text(WIDTH/2,(n+0.5)*BARHEIGHT,text=label,font=("Helvetica",FONTSIZE),tags=label,fill="#FFFFFF",anchor=CENTER)
   filter[label] = 0
#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.LSM6DSV16X.fusion.py serial_port")
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
serialport = serial.Serial(port,115200)
#
# set up GUI
#
root = Tk()
root.title('hello.LSM6DSV16X.fusion.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root,width=WIDTH,height=HEIGHT,background='white')
#
create_bar_graph("ax(g)")
create_bar_graph("ay(g)")
create_bar_graph("az(g)")
#
create_bar_graph("gx(g)")
create_bar_graph("gy(g)")
create_bar_graph("gz(g)")
#
create_bar_graph("rx(dps)")
create_bar_graph("ry(dps)")
create_bar_graph("rz(dps)")
#
create_bar_graph("roll(deg)")
create_bar_graph("pitch(deg)")
create_bar_graph("yaw(deg)")
#
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()

