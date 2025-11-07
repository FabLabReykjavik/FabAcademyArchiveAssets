#
# hello.steptime.RP2040.py
#
# internal resistor step-response loading measurement with RP2040 PIO
#
# Neil Gershenfeld 9/8/25
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

from machine import Pin,freq
import rp2

@rp2.asm_pio(set_init=rp2.PIO.OUT_HIGH)
def steptimer():
    #
    # initialize
    #
    pull()
    mov(y,osr) # move loop to y
    pull() # move settle to osr
    mov(x,null) # move count to x
    set(pins,0) # set pin low
    #
    # main loop
    #
    label('main loop')
    #
    # charge down
    #
    set(pindirs,1) # set pin to output
    #
    # settle down
    #
    mov(isr,x)
    mov(x,osr)
    label('down settle loop')
    jmp(x_dec,'down settle loop')
    mov(x,isr)
    #
    # charge through pull-up
    #
    set(pindirs,0) # set pin to input
    #
    # time up
    #
    label('up loop')
    jmp(pin,'up continue')
    jmp(x_dec,'up loop')
    #
    # loop
    #
    label('up continue')
    jmp(y_dec,'main loop')
    #
    # push count
    #
    mov(isr,x)
    push()
    
class steptime:
    def __init__(self,sm_id,pin):
        self._sm = rp2.StateMachine(sm_id,steptimer,
            jmp_pin=pin,set_base=pin)
        self._sm.active(True)
        self.get = self._sm.get
        self.put = self._sm.put

freq(250000000)

Pin(0,Pin.IN,Pin.PULL_UP) # turn on pull-ups
Pin(1,Pin.IN,Pin.PULL_UP)
Pin(2,Pin.IN,Pin.PULL_UP)
Pin(4,Pin.IN,Pin.PULL_UP)
Pin(3,Pin.IN,Pin.PULL_UP)

step0 = steptime(0,0)
step1 = steptime(1,1)
step2 = steptime(2,2)
step3 = steptime(3,4) # these are flipped in pinout
step4 = steptime(4,3) # "

loop = 200
settle = 20000

min0 = min1 = min2 = min3 = min4 = 1e6

while True:
    step0.put(loop)
    step0.put(settle)
    result0 = 4294967296-step0.get() # +1 for init 0
    if (result0 < min0): min0 = result0
    step1.put(loop)
    step1.put(settle)
    result1 = 4294967296-step1.get()
    if (result1 < min1): min1 = result1
    step2.put(loop)
    step2.put(settle)
    result2 = 4294967296-step2.get()
    if (result2 < min2): min2 = result2
    step3.put(loop)
    step3.put(settle)
    result3 = 4294967296-step3.get()
    if (result3 < min3): min3 = result3
    step4.put(loop)
    step4.put(settle)
    result4 = 4294967296-step4.get()
    if (result4 < min4): min4 = result4
    print(f"7500,{result0-min0},{result1-min1},{result2-min2},{result3-min3},{result4-min4}") # 7500 for scale
