#
# hello.fade.C3.py
#
# Seeed XIAO ESP32C3 PWM LED fade hello-world
#
# Neil Gershenfeld 10/30/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# load MicroPython
#    https://micropython.org/download/ESP32_GENERIC_C3/
#    esptool.py --chip esp32c3 --port /dev/ttyACM0 erase_flash
#    esptool.py --chip esp32c3 --port /dev/ttyACM0 --baud 460800 write_flash -z 0x0 ESP32_GENERIC_C3-20231005-v1.21.0.bin
#

from machine import Pin,PWM
from time import sleep_us

led_pin = 20
frequency = 1000
delay_us = 10

led = PWM(Pin(led_pin),frequency)

while True:
  for duty_cycle in range(0,65536):
    led.duty_u16(duty_cycle)
    sleep_us(delay_us)
  for duty_cycle in range(65535,0,-1):
    led.duty_u16(duty_cycle)
    sleep_us(delay_us)
