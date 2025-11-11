#
# hello.TB67H451.RP2040.sample.py
#    XIAO RP2040 TB67H451 H-bridge audio samples hello-world
#
# Neil Gershenfeld 12/3/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# install MicroPython
#    https://micropython.org/download/RPI_PICO/
# save hello.py to device
#

from rp2 import PIO,StateMachine,asm_pio
import time,math,machine,struct,hello

machine.freq(250_000_000)

pwm_freq = 250_000_000
pwm_range = (1<<10)-1
nsamples = len(hello.samples)/4 # downsample 2x
sample_frequency = 44100/2 # downsample 2x
sample_period_us = 1e6/sample_frequency

@asm_pio(sideset_init=PIO.OUT_LOW)
def pwm_prog():
    pull(noblock).side(0)
    mov(x,osr)
    mov(y,isr)
    label("loop")
    jmp(x_not_y,"skip")
    nop().side(1)
    label("skip")
    jmp(y_dec,"loop")

class pwm_pio:
    def __init__(self,sm_id,pin,max_count,count_freq):
        self._sm = StateMachine(sm_id,pwm_prog,freq=count_freq,sideset_base=machine.Pin(pin))
        self._sm.put(max_count)
        self._sm.exec("pull()")
        self._sm.exec("mov(isr,osr)")
        self._sm.active(1)
        self._max_count = max_count
        self.put = self._sm.put

pwm0 = pwm_pio(0,0,max_count=pwm_range,count_freq=pwm_freq)
pwm1 = pwm_pio(1,1,max_count=pwm_range,count_freq=pwm_freq)

@micropython.native
def play():
    t = t0 = 0
    for i in range(nsamples):
        while ((t-t0) < sample_period_us):
            t = time.ticks_us()
        t0 = t;
        sample = struct.unpack("<h",hello.samples[4*i:4*i+2])[0] # downsample 2x
        if (sample > 0):
            pwm0.put(sample)
            pwm1.put(-1)
        else:
            pwm0.put(-1)
            pwm1.put(-sample)

while True:
    play()
    time.sleep(0.2)

