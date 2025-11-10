#
# lines.py
# Neil Gershenfeld 2/4/17
# animated line plot example
#

import matplotlib.pyplot as plt
from numpy import *
import time

l = 15.5
x = arange(-l,l,.2)
y = sin(x)/x
plt.ion()
fig,ax = plt.subplots()
line, = ax.plot(x,y)
plt.show()
nloop = 25
loop = 1
step = 1
while 1:
   time.sleep(0.01)
   y = sin(0.1*loop*x)/(0.1*loop*x)
   line.set_ydata(y)
   fig.canvas.draw()
   loop += step
   if ((loop == 1) | (loop == nloop)):
      step = -step
