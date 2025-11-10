#
# image.py
# Neil Gershenfeld  2/3/11
# image plot example
#

from matplotlib import cm
from pylab import *
from numpy import *

l = 15.5
fig = figure()
x = arange(-l, l, 0.2)
y = arange(-l, l, 0.2)
(x,y) = meshgrid(x,y)
r = sqrt(x**2 + y**2)
z = sin(r)/r
imshow(z,extent=[-l,l,-l,l],cmap=cm.gray)
show()
