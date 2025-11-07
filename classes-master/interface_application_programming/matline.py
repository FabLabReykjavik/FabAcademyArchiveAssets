#
# matline.py
# (c) Neil Gershenfeld  2/5/05
# plot sin(x)/x with matplotlib
#

from pylab import *
x = arange(-25.5,25.5,0.1)
plot(x,sin(x)/x)
show()
