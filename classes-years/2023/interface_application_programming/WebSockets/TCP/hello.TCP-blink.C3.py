#
# hello.TCP-blink.C3.py
#    Seeed XIAO ESP32C3 TCP button-blink hello-world
#
# Neil Gershenfeld 10/23/23
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
import socket,sys,select

host = "127.0.0.1"
port = 12345

led_pin = 20
button_pin = 21
led = Pin(led_pin,Pin.OUT)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True

with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s:
   s.bind((host,port))
   s.listen()
   conn,addr = s.accept()
   while True:
      readable,writeable,exceptional = select.select([conn],[],[],0)
      for r in readable:
         data = s.recv(1024)
         if (line == '1'):
            led.value(1)
            s.sendall('LED on')
         else:
            led.value(0)
            s.sendall('LED off')
      if ((button.value() == 0) and button_up):
         led.value(1)
         s.sendall('button down')
         button_up = False
      elif ((button.value() == 1) and (not button_up)):
         led.value(0)
         s.sendall('button up')
         button_up = True
