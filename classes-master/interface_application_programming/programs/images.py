#
# images.py
# Neil Gershenfeld  4/21/20
# image animation example
#

import matplotlib.pyplot as plt
from matplotlib import cm
from numpy import *
import time

l = 15.5
x = arange(-l,l,0.2)
y = arange(-l,l,0.2)
(x,y) = meshgrid(x,y)
r = sqrt(x**2+y**2)
z = sin(r)/r
plt.ion()
fig,ax = plt.subplots()
im = ax.imshow(z,extent=[-l,l,-l,l],cmap=cm.gray)
plt.show()
nloop = 25
loop = 1
step = 1
while 1:
   time.sleep(0.01)
   z = sin(0.1*loop*r)/(0.1*loop*r)
   im.set_data(z)
   fig.canvas.draw()
   fig.canvas.flush_events()
   loop += step
   if ((loop == 1) | (loop == nloop)):
      step = -step
