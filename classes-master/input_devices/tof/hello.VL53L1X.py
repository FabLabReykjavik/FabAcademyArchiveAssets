#
# hello.VL53L1X.py
#
# VL53L1X time of flight hello-world
#    hello.VL53L1X.py serial_port
#
# Neil Gershenfeld 10/31/20
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

from tkinter import *
import serial,sys

WINDOW = 600 # window size

def idle(parent,canvas):
   global filt,eps
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
   #
   # read data
   #
   status = ord(ser.read())
   low = ord(ser.read())
   high = ord(ser.read())
   value = (256*high+low)
   #
   # update display
   #
   if (status == 0):
      x = int(.3*WINDOW + (.9-.3)*WINDOW*value/3000)
      canvas.itemconfigure("text",text="%d mm"%value)
      canvas.coords('rect1',.3*WINDOW,.05*WINDOW,x,.2*WINDOW)
      canvas.coords('rect2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
      canvas.update()
   else:
      x = int(.3*WINDOW)
      canvas.itemconfigure("text",text="?")
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
root.title('hello.VL53L1X.py (q to exit)')
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
