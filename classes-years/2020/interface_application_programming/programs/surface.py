#
# surface.py
# Neil Gershenfeld  2/3/11
# surface plot example
#

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from numpy import *

l = 15.5
fig = plt.figure()
ax = fig.gca(projection='3d')
x = arange(-l,l,0.2)
y = arange(-l,l,0.2)
(x,y) = meshgrid(x,y)
r = sqrt(x**2+y**2)
z = sin(r)/r
ax.plot_surface(x,y,z)
plt.show()
