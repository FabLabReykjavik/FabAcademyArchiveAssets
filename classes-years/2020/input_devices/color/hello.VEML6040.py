#
# hello.VEML6040.py
#
# VEML6040 color sensor hello-world
#    hello.VEML6040.py serial_port
#
# Neil Gershenfeld 11/22/20
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

def idle(parent,canvas):
   red = green = blue = white = 0
   #
   # read
   #
   ser.flush()
   while (1):
      reading = ser.readline()
      if (reading[0:6] == b'  Red:'): break
   red = int(reading[7:-2])
   reading = ser.readline()
   green = int(reading[7:-2])
   reading = ser.readline()
   blue = int(reading[7:-2])
   reading = ser.readline()
   white = int(reading[7:-2])
   #
   # plot
   #
   sum = red+green+blue
   canvas.itemconfigure("red",text="red: %d"%red)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(red/sum))
   canvas.coords('red1',.3*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('red2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   canvas.itemconfigure("green",text="green: %d"%green)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(green/sum))
   canvas.coords('green1',.3*WINDOW,.25*WINDOW,x,.4*WINDOW)
   canvas.coords('green2',x,.25*WINDOW,.9*WINDOW,.4*WINDOW)
   canvas.itemconfigure("blue",text="blue: %d"%blue)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(blue/sum))
   canvas.coords('blue1',.3*WINDOW,.45*WINDOW,x,.6*WINDOW)
   canvas.coords('blue2',x,.45*WINDOW,.9*WINDOW,.6*WINDOW)
   canvas.itemconfigure("white",text="white: %d"%white)
   x = int(.3*WINDOW + (.9-.3)*WINDOW*(white/5000))
   canvas.coords('white1',.3*WINDOW,.65*WINDOW,x,.8*WINDOW)
   canvas.coords('white2',x,.65*WINDOW,.9*WINDOW,.8*WINDOW)
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
root.title('hello.VEML6040.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.85*WINDOW, background='white')
#
canvas.create_text(.025*WINDOW,.15*WINDOW,text="1",font=("Helvetica", 22),tags="red",fill="#000000",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.4*WINDOW,.2*WINDOW, tags='red1', fill='#b00000')
canvas.create_rectangle(.4*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='red2', fill='#000000')
#
canvas.create_text(.025*WINDOW,.35*WINDOW,text="1",font=("Helvetica", 22),tags="green",fill="#000000",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.25*WINDOW,.4*WINDOW,.4*WINDOW, tags='green1', fill='#00b000')
canvas.create_rectangle(.4*WINDOW,.25*WINDOW,.9*WINDOW,.4*WINDOW, tags='green2', fill='#000000')
#
canvas.create_text(.025*WINDOW,.55*WINDOW,text="1",font=("Helvetica", 22),tags="blue",fill="#000000",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.45*WINDOW,.4*WINDOW,.6*WINDOW, tags='blue1', fill='#0000b0')
canvas.create_rectangle(.4*WINDOW,.45*WINDOW,.9*WINDOW,.6*WINDOW, tags='blue2', fill='#000000')
#
canvas.create_text(.025*WINDOW,.75*WINDOW,text="1",font=("Helvetica", 22),tags="white",fill="#000000",anchor=SW)
canvas.create_rectangle(.3*WINDOW,.65*WINDOW,.4*WINDOW,.8*WINDOW, tags='white1', fill='#ffffff')
canvas.create_rectangle(.4*WINDOW,.65*WINDOW,.9*WINDOW,.8*WINDOW, tags='white2', fill='#000000')
#
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
