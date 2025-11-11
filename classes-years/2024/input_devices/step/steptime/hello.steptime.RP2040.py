#
# hello.steptime.RP2040.py
#
# measure loading step response with PIO
#
# Neil Gershenfeld 3/31/24
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

from machine import Pin,freq
import rp2

@rp2.asm_pio(set_init=rp2.PIO.OUT_LOW)
def steptimer():
    #
    # initialize
    #
    pull()
    mov(y,osr) # move loop to y
    pull() # move settle to osr
    mov(x,null) # move count to x
    #
    # main loop
    #
    label('main loop')
    #
    # charge up
    #
    set(pins,1)
    #
    # time up
    #
    label('up loop')
    jmp(pin,'up settle')
    jmp(x_dec,'up loop')
    #
    # settle up
    #
    label('up settle')
    mov(isr,x)
    mov(x,osr)
    label('up settle loop')
    jmp(x_dec,'up settle loop')
    mov(x,isr)
    #
    # charge down
    #
    set(pins,0)
    #
    # time down
    #
    label('down loop')
    jmp(pin,'down continue')
    jmp('down settle')
    label('down continue')
    jmp(x_dec,'down loop')
    #
    # settle down
    #
    label('down settle')
    mov(isr,x)
    mov(x,osr)
    label('down settle loop')
    jmp(x_dec,'down settle loop')
    mov(x,isr)
    #
    # loop
    #
    jmp(y_dec,'main loop')
    #
    # push count
    #
    mov(isr,x)
    push()
    
class steptime:
    def __init__(self,sm_id,pin_in,pin_out):
        self._sm = rp2.StateMachine(sm_id,steptimer,
            jmp_pin=Pin(pin_in,Pin.IN),set_base=Pin(pin_out))
        self._sm.active(True)
        self.get = self._sm.get
        self.put = self._sm.put
        self.exec = self._sm.exec

freq(250000000)

state_machine = 0
pin_in = 2
pin_out = 1
step = steptime(state_machine,pin_in,pin_out)

loop = 250
settle = 50000

while True:
    step.put(loop)
    step.put(settle)
    print(4294967296-step.get()) # +1 for init 0
