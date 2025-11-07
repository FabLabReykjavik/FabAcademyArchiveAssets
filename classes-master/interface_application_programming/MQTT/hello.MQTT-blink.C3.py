#
# hello.MQTT-blink.C3.py
#    Seeed XIAO ESP32C3 MQTT blink hello-world
#
# Neil Gershenfeld 11/17/23
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
#
import network
from umqtt.simple import MQTTClient
from machine import Pin
#
# MQTT server address and port
#
# server = your MQTT server IP address
# port = your MQTT server IP port
server = "192.168.1.146"
port = 1883
#
# set up pins
#
led_pin = 20
button_pin = 21
led = Pin(led_pin,Pin.OUT)
led.value(0)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True
#
# connect to Wifi access point
#
print("connect to access point ...")
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
while (wifi.active() == False):
   pass
wifi.connect("ssid","key")
while (wifi.isconnected() == False):
   pass
print("connected to access point at ",wifi.ifconfig())
#
# MQTT message handler
#
def handler(topic,message):
    print(f"received message {message} on topic {topic}")
    if (message == b"1"):
        led.value(1)
    else:
        led.value(0)    
#
# connect to MQTT broker
#
print("connect to MQTT broker ...")
client = MQTTClient("hello.MQTT-blink.C3.py",server,port)
client.set_callback(handler)
client.connect()
print(f"connected to MQTT broker at {server}:{port}")
#
# subscribe to the LED topic
#
client.subscribe("LED")
#
# start main loop
#
while True:
    #
    # check for a message
    #
    client.check_msg()
    #
    # publish button changes to the button topic
    #
    if ((button.value() == 0) and button_up):
        led.value(1)
        print("publish button down")
        client.publish("button","button down")
        button_up = False
    elif ((button.value() == 1) and (not button_up)):
        led.value(0)
        print("publish button up")
        client.publish("button","button up")
        button_up = True    
