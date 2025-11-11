#
# hello.VL53L0X.D11C.py
#
# VL53L0X D11C time of flight hello-world
#    hello.VL53L1X.D11C.py serial_port
#
# Neil Gershenfeld 10/31/22
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

from tkinter import *
import serial,sys

WINDOW = 1200 # window size

def idle(parent,canvas):
   #
   # read data
   #
   value = float(ser.readline().strip())
   #
   # update display
   #
   if (value < 8190):
      x = int(.3*WINDOW + (.9-.3)*WINDOW*value/1500)
      canvas.itemconfigure("text",text="%d mm"%value)
   else:
      x = 0.9*WINDOW
      canvas.itemconfigure("text",text="out of range")
   canvas.coords('rect1',.3*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('rect2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   canvas.update()
   #
   # call after idle
   #
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.VL53L1X.py serial_port")
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
root.title('hello.VL53L0X.D11C.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.25*WINDOW, background='white')
canvas.create_text(.15*WINDOW,.125*WINDOW,text=".33",font=("Helvetica", 24),tags="text",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.05*WINDOW,.3*WINDOW,.2*WINDOW, tags='rect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='rect2', fill='#0000b0')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
