#
# hello.Web-blink.C3.py
#
# Seeed XIAO ESP32C3 Web hello-world
#
# Neil Gershenfeld 11/9/23
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
#
from machine import Pin
import network
import uasyncio as asyncio
from microdot_asyncio import Microdot,Request,Response
#
led_pin = 20
button_pin = 21
led = Pin(led_pin,Pin.OUT)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True
#
# start Wifi access point
#
wifi = network.WLAN(network.AP_IF)
wifi.config(ssid='fab',key='class')
wifi.active(True) # 192.168.4.1
while (wifi.active() == False):
  pass
print("AP started at",wifi.ifconfig())
print("ssid: fab, key: class")
#
# connect to Wifi access point
#
'''
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
while (wifi.active() == False):
   pass
wifi.connect("ssid","key")
while (wifi.isconnected() == False):
   pass
print("connected to AP at ",wifi.ifconfig())
'''
#
# create Web server
#
server = Microdot()
#
# root Web page
#
def root_page():
    html = '''
<!DOCTYPE html>
<html lang="en">
<head>
<script>
function TurnOn() {
   fetch('on')
    .then(response => response.text())
    .then(data => {
         document.getElementById('message').innerHTML = data
         })
   }
function TurnOff() {
   fetch('off')
    .then(response => response.text())
    .then(data => {
         document.getElementById('message').innerHTML = data
         })
   }
</script>
</head>
<body>
<button onclick='TurnOn()'>turn LED on</button>&nbsp;
<button onclick='TurnOff()'>turn LED off</button>&nbsp;
<div id='message'></div>
</body>
</html>
    '''
    return html
#
# server routes
#
@server.get('/')
async def get_root(request):
    return root_page(),200,{'Content-Type':'text/html'}
@server.get('/on')
async def get_on(request):
    led.value(1)
    return "LED is on",200,{'Content-Type':'text/html'}
@server.get('/off')
async def get_off(request):
    led.value(0)
    return "LED is off",200,{'Content-Type':'text/html'}
#
# local task
#
async def local():
    global button_up
    while True:
        if ((button.value() == 0) and button_up):
            led.value(1)
            print('button down')
            button_up = False
        elif ((button.value() == 1) and (not button_up)):
            led.value(0)
            print('button up')
            button_up = True
        await asyncio.sleep(0)
#
# main tasks
#
async def main():
    local_task = asyncio.create_task(local())
    server_task = asyncio.create_task(server.start_server(port=80,debug=True))
    await asyncio.gather(local_task,server_task)
#
# run main tasks
#
asyncio.run(main())
