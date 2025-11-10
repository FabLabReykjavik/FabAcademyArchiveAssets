#
# matimage.py
# (c) Neil Gershenfeld  2/5/05
# plot sin(kr)/kr image with matplotlib
#

from pylab import *

NGRID = 50

xvec = arange(-1.0,1.0,2.0/NGRID)
x = outer(xvec,ones(xvec.shape))
y = transpose(x)
r = sqrt(x*x + y*y)
k = 15
z = sin(k*r)/(k*r)

ion()
m = imshow(z,cmap=cm.gray)
m.set_data(z)
dk = 1
while 1:
   k += dk
   z = sin(k*r)/(k*r)
   m.set_data(z)
   draw()
   if (k == NGRID):
      dk = -1
   elif (k == 0):
      dk = 1
