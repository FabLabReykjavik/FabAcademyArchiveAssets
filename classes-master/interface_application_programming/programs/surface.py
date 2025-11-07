#
# surface.py
# Neil Gershenfeld  2/3/11
# surface plot example
#
from pylab import *
from numpy import *
from mpl_toolkits import mplot3d
l = 15.5
x = arange(-l,l,0.2)
y = arange(-l,l,0.2)
(x,y) = meshgrid(x,y)
r = sqrt(x**2+y**2)
z = sin(r)/r
fig = plt.figure(figsize =(8,8))
ax = plt.axes(projection ='3d')
ax.plot_surface(x,y,z)
plt.show()
