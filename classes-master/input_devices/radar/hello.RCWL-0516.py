#
# hello.RCWL-0516.py
#
# RCWL-0516 Doppler radar motion detectorr hello-world
#    hello.RCWL-0516.py serial_port
#
# Neil Gershenfeld 10/27/19
# (c) Massachusetts Institute of Technology 2019
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

from tkinter import *
import serial

WINDOW = 200 # window size

def idle(parent,canvas):
   global filt,eps
   #
   # idle routine
   #
   char = ser.read()
   if (char == '1'):
      x = 1
      text = 'motion'
      canvas.itemconfigure("rect",fill="#b00000")
      canvas.itemconfigure("text",text="motion")
   else:
      x = 0
      text = 'off'
      canvas.itemconfigure("rect",fill="#0000b0")
      canvas.itemconfigure("text",text="off")
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.RCWL-0516.py serial_port")
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
ser = serial.Serial(port,9600)
ser.setDTR()
ser.flush()
#
# set up GUI
#
root = Tk()
root.title('hello.RCWL-0516.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=2*WINDOW, height=WINDOW, background='white')
canvas.create_text(.5*WINDOW,.5*WINDOW,text="read",font=("Helvetica", 24),tags="text",fill="#0000b0")
canvas.create_rectangle(WINDOW,0,2*WINDOW,WINDOW, tags='rect', fill='#b00000')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
