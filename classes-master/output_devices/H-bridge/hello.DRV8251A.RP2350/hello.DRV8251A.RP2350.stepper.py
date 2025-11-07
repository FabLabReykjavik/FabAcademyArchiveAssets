#
# hello.DRV8251A.RP2350.stepper.py
#    DRV8251A H-bridge stepper RP2350 hello-world
#
# Neil Gershenfeld 6/18/25
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# install MicroPython
#    https://micropython.org/download/SEEED_XIAO_RP2350/
#
from rp2 import PIO,StateMachine,asm_pio
import machine,sys,time,math,array
from machine import Pin,ADC
#
# microstep settings
#

machine.freq(250_000_000)
pwm_freq = 250_000_000
pwm_range = 200
micro_steps = 100
step_count = 50000
step_delay_us = 100
amplitude = 0.8*pwm_range
import gc
gc.disable()
#
# define pins
#
A0_pin = 27
A1_pin = 28
B0_pin = 7
B1_pin = 0
VA_pin = 26
VB_pin = 6
#
# fill in trig tables
#
sin = array.array('f',range(micro_steps))
cos = array.array('f',range(micro_steps))
for i in range(micro_steps):
    sin[i] = math.sin(2*math.pi*i/micro_steps)
    cos[i] = math.cos(2*math.pi*i/micro_steps)
#
# PWM PIO program
#
@asm_pio(sideset_init=PIO.OUT_LOW)
def pwm_prog():
    pull(noblock).side(0) # empty FIFO moves X to OSR
    mov(x,osr) # OSR has PWM threshold
    mov(y,isr) # ISR has PWM range
    label("loop")
    jmp(x_not_y,"skip")
    nop().side(1) # toggle output when X matches Y
    label("skip")
    jmp(y_dec,"loop") # loop over Y
class pwm_pio:
    def __init__(self,sm_id,pwm_pin,pwm_range,pwm_freq):
        self._sm = StateMachine(sm_id,pwm_prog,freq=pwm_freq,sideset_base=machine.Pin(pwm_pin))
        self._sm.put(pwm_range)
        self._sm.exec("pull()")
        self._sm.exec("mov(isr,osr)")
        self._sm.active(1)
        self._max_count = pwm_range
        self.put = self._sm.put
#
# allocate and turn off H bridge control pins
#
A0 = Pin(A0_pin,Pin.OUT)
A1 = Pin(A1_pin,Pin.OUT)
B0 = Pin(B0_pin,Pin.OUT)
B1 = Pin(B1_pin,Pin.OUT)
A0.value(0)
A1.value(0)
B0.value(0)
B1.value(0)
#
# allocate and turn off VREF PWMs
#
VA = pwm_pio(0,VA_pin,pwm_range,pwm_freq)
VB = pwm_pio(1,VB_pin,pwm_range,pwm_freq)
VA.put(0)
VB.put(0)
#
# set microstep
#
def microstep(count,amplitude):
    step = count%micro_steps
    A = amplitude*sin[step]
    B = amplitude*cos[step]
    if (A > 0):
       A0.value(1)
       A1.value(0)
    else:
       A0.value(0)
       A1.value(1)
    VA.put(int(abs(A)))
    if (B > 0):
       B0.value(1)
       B1.value(0)
    else:
       B0.value(0)
       B1.value(1)
    VB.put(int(abs(B)))
#
# main loop
#
while True:
   for step in range(step_count):
      microstep(step,amplitude)
      time.sleep_us(step_delay_us)
   for step in range(step_count,0,-1):
      microstep(step,amplitude)
      time.sleep_us(step_delay_us)
