#
# hello.ATP.recv.RP2040.py
#
# ATP (Asynchronous Token Protocol) PIO receiver
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
    set_init=(rp2.PIO.OUT_LOW,rp2.PIO.OUT_LOW),
    fifo_join=rp2.PIO.JOIN_RX)
def atp_recv():
    set(pins,0b00) # clear output token and bit on set_base
    label('byte loop') # byte loop
    set(x,7) # load bit counter
    label('bit loop') # bit loop
    wait(1,pin,1) # wait for input token on in_base+1
    in_(pins,1) # read bit into isr from in_base
    set(pins,0b01) # set output occupied on set_base
    wait(0,pin,1) # wait for input empty on in_base+1
    set(pins,0b00) # set output empty on set_base
    jmp(x_dec,'bit loop') # bit loop
    push() # push isr to rx fifo
    jmp('byte loop') # byte loop

class atp_receiver:
    def __init__(self,sm_id):
        self._sm = rp2.StateMachine(sm_id,atp_recv,
            freq=50000000,
            in_base = BI,
            set_base = BO,
            in_shiftdir = rp2.PIO.SHIFT_RIGHT)
        self._sm.active(True)
        self.get = self._sm.get
        self.put = self._sm.put
        self.exec = self._sm.exec

freq(250000000)

BO = Pin(6,Pin.OUT,value=0) # bit out
TO = Pin(7,Pin.OUT,value=0) # token out
BI = Pin(28,Pin.IN) # bit in
TI = Pin(29,Pin.IN) # token in

print('start-up delay')
time.sleep(2) # start-up delay

state_machine = 0
receiver = atp_receiver(state_machine)

count = 0
while True:
    count += 1
    print(f'receive char {count}: {chr(receiver.get() >> 24)}')
