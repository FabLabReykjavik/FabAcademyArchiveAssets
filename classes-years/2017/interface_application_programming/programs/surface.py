#
# surface.py
# Neil Gershenfeld  2/3/11
# surface plot example
#

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from pylab import *
from numpy import *

l = 15.5
fig = figure()
ax = Axes3D(fig)
x = arange(-l, l, 0.2)
y = arange(-l, l, 0.2)
(x,y) = meshgrid(x,y)
r = sqrt(x**2 + y**2)
z = sin(r)/r
ax.plot_surface(x,y,z,rstride=1,cstride=1,cmap=cm.gray)
show()
