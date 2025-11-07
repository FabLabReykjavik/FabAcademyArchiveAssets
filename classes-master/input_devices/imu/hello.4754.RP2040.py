#
# hello.4754.RP2040.py
#    Adafruit 4754, BNO085 IMU, XIAO RP2040 hello-world
#
# Neil Gershenfeld 10/21/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
# IMU
#    https://learn.adafruit.com/adafruit-9-dof-orientation-imu-fusion-breakout-bno085/
#    jumper PS0 for UART RVC mode
# install MicroPython:
#    https://wiki.seeedstudio.com/XIAO-RP2040-with-MicroPython/
#    save as main.py to run at boot
#
import os,sys,time
#
# open IMU
#
import machine
from machine import UART,Pin
IMU = UART(0,baudrate=115200,timeout=1000)
#
# read IMU
#
def read_IMU():
    while (True):
        #
        # look for framing
        #
        c1 = 0
        while (True):
            c0 = c1
            c1 = IMU.read(1)
            if ((c0 == b'\xaa') and (c1 == b'\xaa')):
                break
        #
        # read data
        #
        index = int.from_bytes(IMU.read(1),"big")
        yaw0 = int.from_bytes(IMU.read(1),"big")
        yaw1 = int.from_bytes(IMU.read(1),"big")
        pitch0 = int.from_bytes(IMU.read(1),"big")
        pitch1 = int.from_bytes(IMU.read(1),"big")
        roll0 = int.from_bytes(IMU.read(1),"big")
        roll1 = int.from_bytes(IMU.read(1),"big")
        x0 = int.from_bytes(IMU.read(1),"big")
        x1 = int.from_bytes(IMU.read(1),"big")
        y0 = int.from_bytes(IMU.read(1),"big")
        y1 = int.from_bytes(IMU.read(1),"big")
        z0 = int.from_bytes(IMU.read(1),"big")
        z1 = int.from_bytes(IMU.read(1),"big")
        #
        # combine high and low bytes
        #
        yaw = yaw0+256*yaw1
        pitch = pitch0+256*pitch1
        roll = roll0+256*roll1
        ax = x0+256*x1
        ay = y0+256*y1
        az = z0+256*z1
        #
        # two's complement sign
        #
        if (yaw > 32767): yaw = yaw-65536
        if (pitch > 32767): pitch = pitch-65536
        if (roll > 32767): roll = roll-65535
        if (ax > 32767): ax = ax-65536
        if (ay > 32767): ay = ay-65536
        if (az > 32767): az = az-65536
        #
        # convert to degrees
        #
        yaw = yaw/100
        pitch = pitch/100
        roll = roll/100
        #
        # check for valid data (don't know why this is needed)
        #
        if ((yaw <= 180) and (yaw >= -180) or
           (pitch <= 90) and (pitch >= -90) or
           (roll <= 180) and (roll >= -180) or
           (ax <= 5000) and (ax >= -5000) or
           (ay <= 5000) and (ay >= -5000) or
           (az <= 5000) and (az >= -5000)):
            break
    #
    # return data
    #
    return f"{yaw},{pitch},{roll},{ax},{ay},{az}"
#
# main
#
while True:
   print(read_IMU())
