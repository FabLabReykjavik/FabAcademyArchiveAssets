#
# hello.broadhop-LED.py
#    broad-hop LED hello-world
#
# Neil Gershenfeld 11/14/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
import serial,sys,time
#
port = sys.argv[1]
ser = serial.Serial(port,921600)
ser.flush()
#
ser.write(bytearray([ord('a')]))
print(f"found {1+int.from_bytes(ser.read(),'big')} nodes")
while True:
   ser.write(bytearray([ord('l'),0,0b111]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),0,0b011]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),0,0b001]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),0,0b000]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b111]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b011]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b001]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b000]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b111]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b011]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b001]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b000]))
   time.sleep(0.1)
   ser.write(bytearray([ord('L'),0b111]))
   time.sleep(0.2)
   ser.write(bytearray([ord('L'),0b000]))
   time.sleep(0.2)
   ser.write(bytearray([ord('L'),0b111]))
   time.sleep(0.2)
   ser.write(bytearray([ord('L'),0b000]))
   time.sleep(0.2)
   ser.write(bytearray([ord('L'),0b111]))
   time.sleep(0.2)
