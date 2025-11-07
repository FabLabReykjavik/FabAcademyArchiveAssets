#
# hello.TCP-serial-blink.C3.py
#    Seeed XIAO ESP32C3 TCP+serial button+blink hello-world
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
import socket,sys,select,network

host = "0.0.0.0"
port = 12345

led_pin = 20
button_pin = 21
led = Pin(led_pin,Pin.OUT)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True

wifi = network.WLAN(network.STA_IF)
wifi.active(True)
while (wifi.active() == False):
    pass
#wifi.connect("ssid","key")
wifi.connect("squeakyp","10101010")
while (wifi.isconnected() == False):
    pass
print("connected to AP at ",wifi.ifconfig())

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((host,port))
s.listen()
conn,addr = s.accept()
print("connection from ",addr)

while True:
    readable,writeable,exceptional = select.select([conn,sys.stdin],[],[],0)
    if (readable != []):
        if (readable[0] == conn):
            data = conn.recv(1024)
            if (data.decode().rstrip() == '1'):
                led.value(1)
                conn.sendall('LED on\r\n')
            else:
                led.value(0)
                conn.sendall('LED off\r\n')
        elif (readable[0] == sys.stdin):
            line = sys.stdin.readline().rstrip()
            if (line == '1'):
                led.value(1)
                print('received 1: LED on')
            else:
                led.value(0)
                print('received 0: LED off')
    if ((button.value() == 0) and button_up):
        led.value(1)
        conn.sendall('button down\r\n')
        print('button down')
        button_up = False
    elif ((button.value() == 1) and (not button_up)):
        led.value(0)
        conn.sendall('button up\r\n')
        print('button up')
        button_up = True
