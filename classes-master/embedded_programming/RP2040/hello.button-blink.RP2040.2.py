#
# hello.button-blink.RP2040.2.py
#
# Seeed XIAO RP2040 button, blink, echo hello-world, two threads
#
# Neil Gershenfeld 12/28/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# load RP2040 MicroPython
#    https://micropython.org/download/RPI_PICO/
#    https://thonny.org/    
#
from machine import Pin
import sys,select,time,asyncio
#
# set up pins
#
led_pin = 1
button_pin = 0
led = Pin(led_pin,Pin.OUT)
button = Pin(button_pin,Pin.IN,Pin.PULL_UP)
button_up = True
#
# define thread 0
#
async def thread0():
   while True:
      ready = select.select([sys.stdin],[],[],0)[0]
      if (ready != []):
        line = ready[0].readline().rstrip()
        if (line != ''):
            led.value(1)
            print('you typed:',line)
            time.sleep(0.1)
            led.value(0)
      await asyncio.sleep(0) # pause for cooperative mutlitasking

#
# define thread 1
#
async def thread1():
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
      await asyncio.sleep(0) # pasue for cooperative mutlitasking
#
# define thread tasks in main
#
async def main():
    thread0_task = asyncio.create_task(thread0())
    thread1_task = asyncio.create_task(thread1())
    await asyncio.gather(thread0_task,thread1_task)
#
# run main
#
asyncio.run(main())
