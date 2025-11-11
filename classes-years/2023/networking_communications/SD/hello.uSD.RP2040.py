#
# hello.uSD.RP2040.py
#    micro-SD XIAO RP2040 -bridge hello-world
#
# Neil Gershenfeld 11/11/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# install MicroPython
#    https://micropython.org/download/RPI_PICO/
# save sdcard.py:
#    https://raw.githubusercontent.com/micropython/micropython-lib/master/micropython/drivers/storage/sdcard/sdcard.py
#
import sdcard
import os
from machine import SPI,Pin
#
# mount SD card
#
spi=SPI(0,baudrate=40000000,sck=Pin(2),mosi=Pin(3),miso=Pin(4))
sd=sdcard.SDCard(spi,Pin(1))
vfs=os.VfsFat(sd)
os.mount(sd,'/SD')
#
# write to file on card
#
print("write file.txt\n")
with open("/SD/file.txt",'w',encoding="utf-8") as file:
    for i in range(1,11):
        file.write(f"hello world: line {i}\n")
#
# list files
#
print("list root: ",os.listdir("/"))
print("list SD: ",os.listdir("/SD"))
#
# read file
#
print("\nread file.txt:\n")
with open("/SD/file.txt",'r',encoding="utf-8") as file:
    for line in file:
        print(line.rstrip())

