#
# hello.button-blink.C3.py
#
# Seeed XIAO ESP32C3 button, blink, and echo hello-world
#
# Neil Gershenfeld 10/8/23
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

from machine import Pin
import sys,select,time

led_pin = 20
button_pin = 21

led = Pin(led_pin,Pin.OUT)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)

button_up = True

while True:
    ready = select.select([sys.stdin],[],[],0)[0]
    if (ready != []):
        line = ready[0].readline().rstrip()
        if (line != ''):
            led.value(1)
            print('you typed:',line)
            time.sleep(0.1)
            led.value(0)
    if ((button.value() == 0) and button_up):
        led.value(1)
        print('button down')
        button_up = False
    elif ((button.value() == 1) and (not button_up)):
        led.value(0)
        print('button up')
        button_up = True
