#
# hello.ws-blink.C3.py
#    Seeed XIAO ESP32C3 WebSocket button-blink hello-world
#
# Neil Gershenfeld 11/24/23
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
# save Microdot:
#    https://github.com/miguelgrinberg/microdot/blob/main/src/microdot.py
#    https://github.com/miguelgrinberg/microdot/blob/main/src/microdot_asyncio.py
#    https://github.com/miguelgrinberg/microdot/blob/main/src/microdot_websocket.py
#    https://github.com/miguelgrinberg/microdot/blob/main/src/microdot_asyncio_websocket.py
#
# imports
#
import network
from machine import Pin
import uasyncio as asyncio
from microdot_asyncio import Microdot
from microdot_asyncio_websocket import with_websocket
#
# set up LED and button
#
led_pin = 20
button_pin = 21
led = Pin(led_pin,Pin.OUT)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True
#
# connect to WiFi access point
#
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
while (wifi.active() == False):
   pass
wifi.connect("ssid","key")
while (wifi.isconnected() == False):
   pass
print("connected to AP at ",wifi.ifconfig())
#
# create WebSocket server
#
server = Microdot()
#
# server route
#
socket = []
@server.route('/')
@with_websocket
async def message(request,ws):
    global socket
    socket = ws
    while True:
        data = await ws.receive()
        if (data == 'on'):
            led.value(1)
            await ws.send("LED on")
        elif (data == 'off'):
            led.value(0)
            await ws.send("LED off")
#
# local task
#
async def local():
    global button_up,socket
    while True:
        if ((button.value() == 0) and button_up):
            led.value(1)
            await socket.send('button down')
            button_up = False
        elif ((button.value() == 1) and (not button_up)):
            led.value(0)
            await socket.send('button up')
            button_up = True
        await asyncio.sleep(0)
#
# main tasks
#
async def main():
    local_task = asyncio.create_task(local())
    server_task = asyncio.create_task(server.start_server(port=12345,debug=True))
    await asyncio.gather(local_task,server_task)
#
# run main tasks
#
asyncio.run(main())

