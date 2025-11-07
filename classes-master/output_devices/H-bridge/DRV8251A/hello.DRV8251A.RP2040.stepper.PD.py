#
# hello.DRV8251A.RP2040.stepper.PD.py
#    DRV8251A H-bridge stepper PD RP2040 hello-world
#
# Neil Gershenfeld 11/17/24
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
from rp2 import PIO,StateMachine,asm_pio
import machine,sys,time,math,_thread
from machine import Pin,ADC
#
# H-bridge settings
#
#machine.freq(250_000_000)
#pwm_freq = 200_000_000 # faster, for smoother stepping
#pwm_range = 500 # 200 kHz, 2 instructions
pwm_freq = 50_000_000 # slower, for reduced switching losses
pwm_range = 2500 # 10 kHz, 2 instructions
pwm_pinA0 = 27
pwm_pinA1 = 28
pwm_pinB0 = 7
pwm_pinB1 = 0
vrefA_pin = 26
vrefB_pin = 6
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
# allocate and turn off PWMs
#
pwmA0 = pwm_pio(0,pwm_pinA0,pwm_range,pwm_freq)
pwmA1 = pwm_pio(1,pwm_pinA1,pwm_range,pwm_freq)
pwmB0 = pwm_pio(2,pwm_pinB0,pwm_range,pwm_freq)
pwmB1 = pwm_pio(3,pwm_pinB1,pwm_range,pwm_freq)
pwmA0.put(0)
pwmA1.put(0)
pwmB0.put(0)
pwmB1.put(0)
#
# turn on VREFs
#
vrefA = Pin(vrefA_pin,Pin.OUT)
vrefB = Pin(vrefB_pin,Pin.OUT)
vrefA.value(1)
vrefB.value(1)
#
# set up ADC
#
adc = ADC(Pin(29))
#
# convert ADC to amps
#
def amps(count):
    return (3.3*count/65535)/(1575e-6*499)
#
# output PWMs and read ADC
#
def output(A,B,delay):
    if (A > 0):
        pwmA0.put(int(A*pwm_range))
        pwmA1.put(0)
    else:
        pwmA0.put(0)
        pwmA1.put(int(-A*pwm_range))
    if (B > 0):
        pwmB0.put(int(B*pwm_range))
        pwmB1.put(0)
    else:
        pwmB0.put(0)
        pwmB1.put(int(-B*pwm_range))
    count = adc.read_u16()
    time.sleep_us(delay)
    return count
#
# micro step forward
#
def micro_step_fwd(delay,on,micro_step_count):
    count = 0
    for i in range(micro_step_count):
        A = on*math.cos(2*math.pi*i/(micro_step_count-1))
        B = on*math.sin(2*math.pi*i/(micro_step_count-1))
        count += output(A,B,delay)
    return count/micro_step_count
#
# half step forward
#
def half_step_fwd(delay,on):
    count = 0
    count += output(on,on,delay)
    count += output(0,on,delay)
    count += output(-on,on,delay)
    count += output(-on,0,delay)
    count += output(-on,-on,delay)
    count += output(0,-on,delay)
    count += output(on,-on,delay)
    count += output(on,0,delay)
    return count/8#
#
# step thread
#
step_count = 50
step_delay_us = 1500
on = 0.5
micro_step_count = 100
adc_count = 0
def step_thread():
    global adc_count
    while True:
        count = 0
        for _ in range(step_count):
            count += half_step_fwd(step_delay_us,on)
            #count += micro_step_fwd(step_delay_us,on,micro_step_count)
        adc_count = count
_thread.start_new_thread(step_thread,())
#
# main loop
#
while True:
    if (adc_count != 0):
        print(f"step forward, PWM fraction {on}, amps/phase: {amps(adc_count/step_count):.2f}")
        adc_count = 0