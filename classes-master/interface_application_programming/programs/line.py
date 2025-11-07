#
# line.py
# Neil Gershenfeld  2/3/11
# line plot example
#

import matplotlib.pyplot as plt
from numpy import *

l = 15.5
x = arange(-l,l,.2)
y = sin(x)/x
plt.plot(x,y)
plt.show()
