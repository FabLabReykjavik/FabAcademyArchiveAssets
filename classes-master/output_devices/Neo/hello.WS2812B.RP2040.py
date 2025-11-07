#
# hello.WS2812B.RP2040.py
#    WS281B RGB LED RP2040 hello-world
#
# Neil Gershenfeld 10/26/24
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

import array,time
from machine import Pin
import rp2

dt = 1 # loop delay in ms
Max = 255 # max brightness

@rp2.asm_pio(sideset_init=rp2.PIO.OUT_LOW,
    out_shiftdir=rp2.PIO.SHIFT_LEFT,
    autopull=True,pull_thresh=24)
def WS2812B():
    label("loop")
    out(x,1).side(0) # 1 cycle (for 0 or 1) (differs from data sheet?)
    jmp(not_x,"zero").side(1)[2] # 3 cycles (for 0 or 1)
    nop()[3] # 4 cycles (for 1)
    jmp("loop").side(0)[3] # 4 cycles (for 1)
    label("zero")
    nop().side(0)[3] # 4 cycles (for 0)
    jmp("loop")[3] # 4 cycles (for 0)

sm = rp2.StateMachine(0,WS2812B,freq=10_000_000,sideset_base=Pin(3))
sm.active(1)

arr = array.array("I",[0 for _ in range(4)])

def RGB(r,g,b):
    return (g << 16 | r << 8 | b)

def show(arr):
    sm.put(arr,8) # shift 8 bits to use 24 bits on 32 bit int

while True:
    for i in range(Max):
        arr[0] = RGB(i,0,0) # fade in red
        arr[1] = RGB(0,0,0)
        arr[2] = RGB(0,0,0)
        arr[3] = RGB(0,0,0)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(Max-i,i,0) # fade in green
        arr[1] = RGB(i,0,0)
        arr[2] = RGB(0,0,0)
        arr[3] = RGB(0,0,0)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(0,Max-i,i) # fade in blue
        arr[1] = RGB(Max-i,i,0)
        arr[2] = RGB(i,0,0)
        arr[3] = RGB(0,0,0)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(i,i,Max) # fade in white
        arr[1] = RGB(0,Max-i,i)
        arr[2] = RGB(Max-i,i,0)
        arr[3] = RGB(i,0,0)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(Max,Max,Max) # fade in all white
        arr[1] = RGB(i,i,Max)
        arr[2] = RGB(i,Max,i)
        arr[3] = RGB(Max,i,i)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(0,0,0) # blink
        arr[1] = RGB(0,0,0)
        arr[2] = RGB(0,0,0)
        arr[3] = RGB(0,0,0)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(Max,Max,Max) # blink
        arr[1] = RGB(Max,Max,Max)
        arr[2] = RGB(Max,Max,Max)
        arr[3] = RGB(Max,Max,Max)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(0,0,0) # blink
        arr[1] = RGB(0,0,0)
        arr[2] = RGB(0,0,0)
        arr[3] = RGB(0,0,0)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max):
        arr[0] = RGB(Max,Max,Max) # blink
        arr[1] = RGB(Max,Max,Max)
        arr[2] = RGB(Max,Max,Max)
        arr[3] = RGB(Max,Max,Max)
        show(arr)
        time.sleep_ms(dt)
    for i in range(Max,0,-1):
        arr[0] = RGB(i,i,i) # fade out
        arr[1] = RGB(i,i,i)
        arr[2] = RGB(i,i,i)
        arr[3] = RGB(i,i,i)
        show(arr)
        time.sleep_ms(dt)
