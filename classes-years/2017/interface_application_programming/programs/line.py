#
# line.py
# Neil Gershenfeld  2/3/11
# line plot example
#

from numpy import *
from pylab import *

l = 15.5
x = arange(-l,l,.2)
y = sin(x)/x
plot(x,y)
axis([-l,l,-.3,1.1])
show()
