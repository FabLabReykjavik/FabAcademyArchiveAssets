#
# hello.steptime.RP2040.plot.py
#
# receive and display loading step response
#    hello.steptime.RP2040.plot.py serial_port
#
# Neil Gershenfeld 3/31/24
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
eps = 0.1 # smoothing filter
plot_min = 0
plot_max = 0
filt = 0

def idle(parent,canvas):
   #
   # read and plot
   #
   global filt
   str = ser.readline().decode('utf-8').strip()
   if (str != ''):
      reading = float(str)
      filt = (1-eps)*filt+eps*reading
      print("%.0f"%filt)
      canvas.itemconfigure("text",text="%.0f"%filt)
      if ((filt > plot_min) and (filt < plot_max)):
         x = int(.2*WINDOW + (.9-.2)*WINDOW*(filt-plot_min)/(plot_max-plot_min))
      elif (filt > plot_max):
         x = int(.2*WINDOW + (.9-.2)*WINDOW)
      else:
         x = int(.2*WINDOW)
      canvas.coords('rect1',.2*WINDOW,.05*WINDOW,x,.2*WINDOW)
      canvas.coords('rect2',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
      canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 4):
   print("command line: hello.steptime.RP2040.py serial_port plot_min plot_max")
   sys.exit()
port = sys.argv[1]
plot_min = float(sys.argv[2])
filt = plot_min
plot_max = float(sys.argv[3])
#
# open serial port
#
ser = serial.Serial(port,115200)
ser.setDTR()
#
# set up GUI
#
root = Tk()
root.title('hello.steptime.RP2040.plot.py (q to exit)')
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
