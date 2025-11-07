#
# hello.TB67H451.RP2040.py
#    XIAO RP2040 TB67H451 H-bridge hello-world
#
# Neil Gershenfeld 11/6/23
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
from time import sleep_ms

duty_min = int(0.65*65535)
duty_max = int(0.75*65535)
delay_ms = 500

pwm0 = PWM(Pin(0),freq=100000,duty_u16=0)
pwm1 = PWM(Pin(1),freq=100000,duty_u16=0)

while True:
    pwm0.duty_u16(0)
    pwm1.duty_u16(duty_min)
    sleep_ms(delay_ms)
    #
    pwm0.duty_u16(duty_min)
    pwm1.duty_u16(0)
    sleep_ms(delay_ms)
    #
    pwm0.duty_u16(0)
    pwm1.duty_u16(duty_max)
    sleep_ms(delay_ms)
    #
    pwm0.duty_u16(duty_max)
    pwm1.duty_u16(0)
    sleep_ms(delay_ms)
    #
