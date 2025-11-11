#
# hello.txrx.t1624.py
#
# receive and display transmit-receive step response
#    hello.step.t1624.py serial_port
#
# Neil Gershenfeld 11/14/21
#

from tkinter import *
import serial,sys

WINDOW = 600 # window size
max = 75000 # bargraph max
eps = 0.1 # filter fraction
filt = 0.0 # filtered value

def idle(parent,canvas):
   global filt, eps
   #
   # idle routine: read, filter, and plot
   #
   value = int(ser.readline().rstrip().decode())
   filt = (1-eps)*filt + eps*value
   x = int(.2*WINDOW + (.9-.2)*WINDOW*filt/max)
   canvas.itemconfigure("text",text="%.0f"%filt)
   canvas.coords('rect1',.2*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('rect2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print("command line: hello.txrx.t1624.py serial_port")
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
root.title('hello.txrx.t1624.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.25*WINDOW, background='white')
#
canvas.create_text(.1*WINDOW,.125*WINDOW,text="1",font=("Helvetica", 24),tags="text",fill="#0000b0")
canvas.create_rectangle(.2*WINDOW,.05*WINDOW,.3*WINDOW,.2*WINDOW, tags='rect1', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='rect2', fill='#0000b0')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
