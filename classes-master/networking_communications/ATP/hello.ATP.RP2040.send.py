#
# hello.ATP.send.RP2040.py
#
# ATP (Asynchronous Token Protocol) PIO sender
#
# Neil Gershenfeld 4/21/24
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#

import rp2,time
from machine import Pin,freq,mem32

@rp2.asm_pio(
    set_init=rp2.PIO.OUT_LOW,
    sideset_init=rp2.PIO.OUT_LOW,
    out_init=rp2.PIO.OUT_LOW,
    fifo_join=rp2.PIO.JOIN_TX)
def atp_send():
    label('byte loop') # byte loop
    pull() # blocking pull from tx fifo into osr
    set(x,7) # load bit counter
    label('bit loop') # bit loop
    wait(0,pin,1) # wait for no input token on in_base+1
    wait(0,pin,0) # wait for input empty on in_base
    out(pins,1).side(1) # send bit from osr to out_base, set token on sideset_base
    wait(1,pin,0) # wait for input occupied on in_base
    set(pins,0).side(0) # clear output token and bit
    jmp(x_dec,'bit loop') # bit loop
    jmp('byte loop') # byte loop

class atp_sender:
    def __init__(self,sm_id):
        self._sm = rp2.StateMachine(sm_id,atp_send,
            freq=50000000,
            in_base = BI,
            out_base = BO,
            set_base = BO,
            sideset_base = TO,
            out_shiftdir = rp2.PIO.SHIFT_RIGHT)
        self._sm.active(True)
        self.get = self._sm.get
        self.put = self._sm.put
        self.exec = self._sm.exec

freq(250000000)

BO = Pin(3,Pin.OUT,value=0) # bit out
TO = Pin(4,Pin.OUT,value=0) # token out
BI = Pin(1,Pin.IN) # bit in
TI = Pin(2,Pin.IN) # token in

print('start-up delay')
time.sleep(2) # start-up delay

state_machine = 0
sender = atp_sender(state_machine)

count = 0
while True:
    count += 1
    print(f'send hello world: {count}')
    sender.put('hello world ')
    time.sleep(1)
