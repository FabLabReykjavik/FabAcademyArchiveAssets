#
# hello.hop-LED.py
#    hop-count LED hello-world
#
# Neil Gershenfeld 12/23/23
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
ser.write(bytearray([ord('a'),0]))
print(f"found {1+int.from_bytes(ser.read(),'big')} nodes")
while True:
   ser.write(bytearray([ord('l'),0,0b111]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),0,0b110]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),0,0b100]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),0,0b000]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b111]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b110]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b100]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),1,0b000]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b111]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b110]))
   time.sleep(0.1)
   ser.write(bytearray([ord('l'),2,0b100]))
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
