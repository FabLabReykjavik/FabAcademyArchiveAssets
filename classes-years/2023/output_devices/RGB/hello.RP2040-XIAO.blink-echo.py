#
# hello.RP204-XIAO.blink-echo.py
#
# Seeed XIAO RP2040 blink and echo hello-world
#
# Neil Gershenfeld 2/12/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
import sys
import utime
import machine
from ws2812 import WS2812
#
# globals
#
numpixels = 1
pixelpower = 11
pixelpin = 12
string = ''
#
# steup
#
powerpin = machine.Pin(pixelpower,machine.Pin.OUT)
powerpin.value(1)
led = WS2812(pixelpin,numpixels)
#
# main loop
#
while True:
    #
    # read, save, and send char
    #
    char = sys.stdin.read(1)
    string += char
    sys.stdout.write("hello.RP2040-XIAO.blink-echo.py: you typed "+string+'\r'+'\n');
    #
    # blink LED red green blue white black
    #
    led.pixels_fill((255,0,0))
    led.pixels_show()
    utime.sleep(0.1)
    #
    led.pixels_fill((0,255,0))
    led.pixels_show()
    utime.sleep(0.1)
    #
    led.pixels_fill((0,0,255))
    led.pixels_show()
    utime.sleep(0.1)
    #
    led.pixels_fill((255,255,255))
    led.pixels_show()
    utime.sleep(0.1)
    #
    led.pixels_fill((0,0,0))
    led.pixels_show()
