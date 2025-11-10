#!/usr/bin/env python
#
# udp_slip.py
#
#    send and receive UDP SLIP packets
#    command line: python udp_slip.py serial_port serial_speed
#
# Neil Gershenfeld 11/22/09
#
# (c) Massachusetts Institute of Technology 2013
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

from Tkinter import *
from string import *
import serial, time, signal

TIMEOUT = 1
END = 192
ESC = 219
ESC_END = 220
ESC_ESC = 221

def handler(signum, frame):
   #
   # timeout handler
   #
   raise IOError, "timeout"

def high(number):
   #
   # return high byte
   #
   return (number >> 8)

def low(number):
   #
   # return low byte
   #
   return (number & 255)

def idle(parent):
   #
   # idle routine
   #
   if (ser.inWaiting() != 0):
      #
      # serial data waiting, read packet
      #
      # set timout alarm
      #
      signal.signal(signal.SIGALRM, handler)
      signal.alarm(TIMEOUT)
      #
      # try to read packet, otherwise set error message
      #
      try:
         get_packet()
      except:
         sindata.set("timeout")
      #
      # turn off alarm
      #
      signal.alarm(0)
   #
   # sleep to reduce OS load
   #
   time.sleep(0.001)
   parent.after_idle(idle,parent)

def output(byte):
   #
   # output a byte
   #
   ser.write(chr(byte))
   time.sleep(.001) # byte spacing (if needed)

def put_packet():
   #
   # send packet
   #
   ip_header_size = 20
   udp_header_size = 8
   outgoing_data_size = 10
   #
   # IP
   #
   packet = []
   packet.append(0x45) # version = 4, header length = 5 32-bit words
   packet.append(0) # type of service
   packet.append(high(ip_header_size + udp_header_size + outgoing_data_size)) # packet length
   packet.append(low(ip_header_size + udp_header_size + outgoing_data_size)) # packet length
   packet.append(0) # identification high byte
   packet.append(0) # identification low byte
   packet.append(0) # flag, fragment offset high byte
   packet.append(0) # flag, fragment offset low byte
   packet.append(255) # time to live
   packet.append(17) # protocol = 17 for UDP
   packet.append(0) # header checksum high byte
   packet.append(0) # header checksum low byte
   packet.append(int(soutsource1.get())) # source address byte 1
   packet.append(int(soutsource2.get())) # source address byte 2
   packet.append(int(soutsource3.get())) # source address byte 3
   packet.append(int(soutsource4.get())) # source address byte 4
   packet.append(int(soutdest1.get())) # destination address byte 1
   packet.append(int(soutdest2.get())) # destination address byte 2
   packet.append(int(soutdest3.get())) # destination address byte 3
   packet.append(int(soutdest4.get())) # destination address byte 4
   #
   # UDP
   #
   packet.append(high(int(soutsourceport.get()))) # source port
   packet.append(low(int(soutsourceport.get()))) # source port
   packet.append(high(int(soutdestport.get()))) # destination port
   packet.append(low(int(soutdestport.get()))) # destination port
   packet.append(high(udp_header_size + outgoing_data_size)) # payload length
   packet.append(low(udp_header_size + outgoing_data_size)) # payload length
   packet.append(0) # payload checksum (not used)
   packet.append(0) # payload checksum (not used)
   #
   # data
   #
   data = soutdata.get()
   for i in range(len(data)):
      packet.append(ord(data[i]))
   #
   # calculate checksum
   #
   """
   todo:
   ldi zh, high(outgoing_ip_start)
   ldi zl, low(outgoing_ip_start)
   ldi count, ip_header_size
   clr check_lo
   clr check_hi
   clr check_carry
   ip_checksum_loop:
      adiw zl, 1
      ld temp, z
      dec count
      sbiw zl, 1
      ld temp1, z
      add check_lo, temp
      adc check_hi, temp1
      adc check_carry, zero
      adiw zl, 2
      dec count
      brne ip_checksum_loop
   add check_lo, check_carry
   adc check_hi, zero
   com check_lo
   com check_hi
   sts outgoing_ip_checksum, check_hi
   sts outgoing_ip_checksum+1, check_lo
   """
   #
   # send the packet with SLIP mapping and framing
   #
   output(END)
   for byte in range(len(packet)):
      if (packet[byte] == END):
         output(ESC)
         output(ESC_END)
      elif (packet[byte] == ESC):
         output(ESC)
         output(ESC_ESC)
      else:
         output(packet[byte])
   output(END)

def get_packet():
   #
   # read and display packet
   #
   sindata.set("reading ...")
   root.update()
   packet_length_offset = 2
   source_address_offset = 12
   destination_address_offset = 16
   source_port_offset = 20
   destination_port_offset = 22
   data_offset = 28
   #
   # read until END
   #
   packet = []
   while 1:
      byte = ord(ser.read())
      #
      # check for end
      #
      if (byte == END):
         #
         # check if packet received
         #
         if (len(packet) > 0):
            #
            # packet received, update GUI
            #
            sinsource1.set(str(packet[source_address_offset+0]))
            sinsource2.set(str(packet[source_address_offset+1]))
            sinsource3.set(str(packet[source_address_offset+2]))
            sinsource4.set(str(packet[source_address_offset+3]))
            sindest1.set(str(packet[destination_address_offset+0]))
            sindest2.set(str(packet[destination_address_offset+1]))
            sindest3.set(str(packet[destination_address_offset+2]))
            sindest4.set(str(packet[destination_address_offset+3]))
            sinsourceport.set(str(256*packet[source_port_offset+0] \
               + packet[source_port_offset+1]))
            sindestport.set(str(256*packet[destination_port_offset+0] \
               + packet[destination_port_offset+1]))
            packet_length = 256*packet[packet_length_offset+0] \
               + packet[packet_length_offset+1]
            data = join(map(chr,packet[(data_offset):]),sep="")
            sindata.set(data)
            return
         else:
            #
            # no packet received, continue
            #
            continue
      #
      # not END, do SLIP mapping and save byte
      #
      if (byte == ESC):
         byte = ord(ser.read())
         if (byte == ESC_END):
            byte = END
         elif (byte == ESC_ESC):
            byte = ESC
         else:
            print "error: unknown ESC"
            break
      packet.append(byte)

def quit():
   #
   # clean up and quit
   #
   sys.exit()

#
# get command line arguments
#
if (len(sys.argv) != 3):
   print "command line: python udp_slip.py serial_port serial_speed"
   sys.exit()
serial_port = sys.argv[1]
serial_speed = int(sys.argv[2])
#
# set up GUI
#
root = Tk()
root.title('udp_slip.py')
#
incomingframe = Frame(root)
Label(incomingframe,text="incoming packet: ").pack(side="left")
incomingframe.pack()
#
insourceframe = Frame(root)
Label(insourceframe,text="source address: ").pack(side="left")
sinsource1 = StringVar()
winsource1 = Entry(insourceframe, width=3, textvariable=sinsource1)
winsource1.pack(side="left")
Label(insourceframe,text=".").pack(side="left")
sinsource2 = StringVar()
winsource2 = Entry(insourceframe, width=3, textvariable=sinsource2)
winsource2.pack(side="left")
Label(insourceframe,text=".").pack(side="left")
sinsource3 = StringVar()
winsource3 = Entry(insourceframe, width=3, textvariable=sinsource3)
winsource3.pack(side="left")
Label(insourceframe,text=".").pack(side="left")
sinsource4 = StringVar()
winsource4 = Entry(insourceframe, width=3, textvariable=sinsource4)
winsource4.pack(side="left")
insourceframe.pack()
#
indestframe = Frame(root)
Label(indestframe,text="destination address: ").pack(side="left")
sindest1 = StringVar()
windest1 = Entry(indestframe, width=3, textvariable=sindest1)
windest1.pack(side="left")
Label(indestframe,text=".").pack(side="left")
sindest2 = StringVar()
windest2 = Entry(indestframe, width=3, textvariable=sindest2)
windest2.pack(side="left")
Label(indestframe,text=".").pack(side="left")
sindest3 = StringVar()
windest3 = Entry(indestframe, width=3, textvariable=sindest3)
windest3.pack(side="left")
Label(indestframe,text=".").pack(side="left")
sindest4 = StringVar()
windest4 = Entry(indestframe, width=3, textvariable=sindest4)
windest4.pack(side="left")
indestframe.pack()
#
inportframe = Frame(root)
Label(inportframe,text="source port: ").pack(side="left")
sinsourceport = StringVar()
winsourceport = Entry(inportframe, width=4, textvariable=sinsourceport)
winsourceport.pack(side="left")
Label(inportframe,text="  destination port: ").pack(side="left")
sindestport = StringVar()
windestport = Entry(inportframe, width=4, textvariable=sindestport)
windestport.pack(side="left")
inportframe.pack()
#
indataframe = Frame(root)
Label(indataframe,text="data: ").pack(side="left")
sindata = StringVar()
windata = Entry(indataframe, width=30, textvariable=sindata)
windata.pack(side="left")
indataframe.pack()
#
outgoingframe = Frame(root)
Label(outgoingframe,text="\noutgoing packet: ").pack(side="left")
outgoingframe.pack()
#
outsourceframe = Frame(root)
Label(outsourceframe,text="source address: ").pack(side="left")
soutsource1 = StringVar()
soutsource1.set("10")
woutsource1 = Entry(outsourceframe, width=3, textvariable=soutsource1)
woutsource1.pack(side="left")
Label(outsourceframe,text=".").pack(side="left")
soutsource2 = StringVar()
soutsource2.set("0")
woutsource2 = Entry(outsourceframe, width=3, textvariable=soutsource2)
woutsource2.pack(side="left")
Label(outsourceframe,text=".").pack(side="left")
soutsource3 = StringVar()
soutsource3.set("0")
woutsource3 = Entry(outsourceframe, width=3, textvariable=soutsource3)
woutsource3.pack(side="left")
Label(outsourceframe,text=".").pack(side="left")
soutsource4 = StringVar()
soutsource4.set("1")
woutsource4 = Entry(outsourceframe, width=3, textvariable=soutsource4)
woutsource4.pack(side="left")
outsourceframe.pack()
#
outdestframe = Frame(root)
Label(outdestframe,text="destination address: ").pack(side="left")
soutdest1 = StringVar()
soutdest1.set("10")
woutdest1 = Entry(outdestframe, width=3, textvariable=soutdest1)
woutdest1.pack(side="left")
Label(outdestframe,text=".").pack(side="left")
soutdest2 = StringVar()
soutdest2.set("0")
woutdest2 = Entry(outdestframe, width=3, textvariable=soutdest2)
woutdest2.pack(side="left")
Label(outdestframe,text=".").pack(side="left")
soutdest3 = StringVar()
soutdest3.set("0")
woutdest3 = Entry(outdestframe, width=3, textvariable=soutdest3)
woutdest3.pack(side="left")
Label(outdestframe,text=".").pack(side="left")
soutdest4 = StringVar()
soutdest4.set("2")
woutdest4 = Entry(outdestframe, width=3, textvariable=soutdest4)
woutdest4.pack(side="left")
outdestframe.pack()
#
outportframe = Frame(root)
Label(outportframe,text="source port: ").pack(side="left")
soutsourceport = StringVar()
soutsourceport.set("0")
woutsourceport = Entry(outportframe, width=4, textvariable=soutsourceport)
woutsourceport.pack(side="left")
Label(outportframe,text="  destination port: ").pack(side="left")
soutdestport = StringVar()
soutdestport.set("1234")
woutdestport = Entry(outportframe, width=4, textvariable=soutdestport)
woutdestport.pack(side="left")
outportframe.pack()
#
outdataframe = Frame(root)
Label(outdataframe,text="data: ").pack(side="left")
soutdata = StringVar()
woutdata = Entry(outdataframe, width=30, textvariable=soutdata)
woutdata.pack(side="left")
outdataframe.pack()
#
ioframe = Frame(root)
wbtn = Button(ioframe, text="send outgoing packet",command=put_packet)
wbtn.pack(side="left")
ioframe.pack()
#
spaceframe = Frame(root)
Label(spaceframe,text=" ").pack(side="left")
spaceframe.pack()
#
quitframe = Frame(root)
wquit = Button(quitframe, text="quit",command=quit)
wquit.pack(side="left")
quitframe.pack()
#
# open serial port
#
ser = serial.Serial(port=serial_port, baudrate=serial_speed)
ser.flushInput()
#
# start mainloop
#
root.after(100,idle,root)
root.mainloop()
