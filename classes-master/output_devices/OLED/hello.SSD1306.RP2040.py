#
# hello.SSD1306.RP2040.py
#    XIAO RP2040 SSD1306 OLED hello-world
#
# Neil Gershenfeld 11/4/23
#
# install MicroPython:
#    https://micropython.org/download/RPI_PICO/
# copy ssd1306 to device:
#    https://github.com/stlehmann/micropython-ssd1306/blob/master/ssd1306.py
#
from machine import Pin,I2C
import ssd1306
#
width = 128
height = 64
#
i2c = I2C(1,sda=Pin(6),scl=Pin(7),freq=400000)
oled = ssd1306.SSD1306_I2C(width,height,i2c,addr=0x3c)
#
while True:
    for i in range(width):
        oled.fill(0)
        oled.text('Hello World',0,0,1)
        oled.text(f"width: {i}",0,15,1)
        oled.fill_rect(0,30,i,20,1)
        oled.show()