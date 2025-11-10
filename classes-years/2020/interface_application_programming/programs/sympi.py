import sympy as sy
import numpy as np
from sympy.functions import atan
x = sy.Symbol('x')
f = 4*atan(x)
def factorial(n):
    if n <= 0:
        return 1
    else:
        return n*factorial(n-1)
def taylor(function,x0,n):
    i = 0
    p = 0
    while i <= n:
        p += (function.diff(x,i).subs(x,x0))/(factorial(i))*(x-x0)**i
        i += 1
    return p
func = taylor(f,1,10)
print(func)
