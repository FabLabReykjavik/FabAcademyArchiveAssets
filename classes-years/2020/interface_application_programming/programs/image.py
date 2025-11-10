#
# image.py
# Neil Gershenfeld  2/3/11
# image plot example
#

import matplotlib.pyplot as plt
from matplotlib import cm
from numpy import *

l = 15.5
x = arange(-l,l,0.2)
y = arange(-l,l,0.2)
(x,y) = meshgrid(x,y)
r = sqrt(x**2+y**2)
z = sin(r)/r
plt.imshow(z,extent=[-l,l,-l,l],cmap=cm.gray)
plt.show()
