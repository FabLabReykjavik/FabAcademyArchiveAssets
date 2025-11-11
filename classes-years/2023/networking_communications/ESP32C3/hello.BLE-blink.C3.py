#
# hello.BLE-blink.C3.py
#    Seeed XIAO ESP32C3 BLE blink hello-world
#
# Neil Gershenfeld 11/13/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# load MicroPython:
#    https://micropython.org/download/ESP32_GENERIC_C3/
#    esptool.py --chip esp32c3 --port /dev/ttyACM0 erase_flash
#    esptool.py --chip esp32c3 --port /dev/ttyACM0 --baud 460800 write_flash -z 0x0 ESP32_GENERIC_C3-20231005-v1.21.0.bin
# save blueart.py
#
import blueart
from machine import Pin
#
led_pin = 20
button_pin = 21
#
led = Pin(led_pin,Pin.OUT)
led.value(0)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True
#
def rx():
    message = uart.read().decode().strip()
    print("received: "+message)
    if (message == "1"):
        led.value(1)
    elif (message == "0"):
        led.value(0)
#
uart = blueart.uart()
uart.irq(handler=rx)
#
try:
    while True:
        if ((button.value() == 0) and button_up):
            led.value(1)
            print("send: button down")
            uart.write('button down')
            button_up = False
        elif ((button.value() == 1) and (not button_up)):
            led.value(0)
            print("send: button up")
            uart.write('button up')
            button_up = True
except:
    uart.close()
