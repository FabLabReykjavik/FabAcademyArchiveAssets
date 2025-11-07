#
# hello.touch.S3.py
#
# ESP32S3 XIAO touch hello-world
#
# Neil Gershenfeld 7/25/24
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# install MicroPython
#    https://micropython.org/download/RPI_PICO/
#

from machine import TouchPad,Pin
import time

t1 = TouchPad(Pin(1))
t3 = TouchPad(Pin(3))
t4 = TouchPad(Pin(4))
t5 = TouchPad(Pin(5))
t6 = TouchPad(Pin(6))

r1min = r3min = r4min = r5min = r6min = 1e6
r1filt = r3filt = r4filt = r5filt = r6filt = 0
eps = 0.01 # smoothing filter fraction
loop = 500 # smoothing filter iterations

while True:
    for i in range(loop):
        r1 = t1.read()
        if (r1 < r1min): r1min = r1
        r1filt = (1-eps)*r1filt+eps*(r1-r1min)
        r3 = t3.read()
        if (r3 < r3min): r3min = r3
        r3filt = (1-eps)*r3filt+eps*(r3-r3min)
        r4 = t4.read()
        if (r4 < r4min): r4min = r4
        r4filt = (1-eps)*r4filt+eps*(r4-r4min)
        r5 = t5.read()
        if (r5 < r5min): r5min = r5
        r5filt = (1-eps)*r5filt+eps*(r5-r5min)
        r6 = t6.read()
        if (r6 < r6min): r6min = r6
        r6filt = (1-eps)*r6filt+eps*(r6-r6min)
    print(f"20000,{r1filt},{r3filt},{r4filt},{r5filt},{r6filt}") # 20000 for scale

