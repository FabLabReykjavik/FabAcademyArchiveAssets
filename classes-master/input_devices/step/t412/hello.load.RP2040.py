#
# hello.load.RP2040.py
#
# receive, filter, and forward step response
#
# Neil Gershenfeld 9/7/24
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# install MicroPython
#    https://micropython.org/download/RPI_PICO/
#

from machine import Pin
import rp2

baud = 115200
rx = Pin(3,Pin.IN,Pin.PULL_UP)

@rp2.asm_pio(
   autopush=True,
   push_thresh=8,
   in_shiftdir=rp2.PIO.SHIFT_RIGHT,
   fifo_join=rp2.PIO.JOIN_RX)
def uart_rx():
   wait(0,pin,0) # wait for start bit
   set(x,7) [10] # load bit counter, delay into middle of first bit
   label("loop") # bit loop
   in_(pins,1) # read data
   jmp(x_dec,"loop") [6] # 8 cycles total

uart = rp2.StateMachine(
   0,uart_rx,freq=8*baud,
   in_base=rx, # wait, in
   jmp_pin=rx, # jmp
   )
uart.active(1)

filt = 1e6
filtmin = filt
eps = 0.5
result = ""
while True:
   c = chr(uart.get() >> 24)
   if (c == '\n'):
      filt = (1-eps)*filt+eps*int(result)
      if (filt < filtmin):
         filtmin = filt
      print(filt-filtmin)
      result = ""
   elif (c != '\r'):
      result += c
 
