#
# hello.TB67H451.RP2040.audio.py
#    XIAO RP2040 TB67H451 H-bridge audio hello-world
#
# Neil Gershenfeld 11/7/23
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

from machine import Pin,PWM
from time import sleep_us

duty_min = int(0.1*65535)
duty_max = int(0.4*65535)
duration_us = 10000

pwm0 = PWM(Pin(0),freq=50000,duty_u16=0)
pwm1 = PWM(Pin(1),freq=50000,duty_u16=0)

while True:
    #
    # square wave soft ramp
    #
    for delay_us in range(5000,500,-50):
        cycles = duration_us/delay_us
        for i in range(cycles):
            pwm0.duty_u16(0)
            pwm1.duty_u16(duty_min)
            sleep_us(delay_us)
            #
            pwm0.duty_u16(duty_min)
            pwm1.duty_u16(0)
            sleep_us(delay_us)
    #
    # square wave loud ramp
    #
    for delay_us in range(5000,500,-50):
        cycles = duration_us/delay_us
        for i in range(cycles):
            pwm0.duty_u16(0)
            pwm1.duty_u16(duty_max)
            sleep_us(delay_us)
            #
            pwm0.duty_u16(duty_max)
            pwm1.duty_u16(0)
            sleep_us(delay_us)
            #