#
# pixbyte8 infile outfile
#    pack 8xN one-bit image pixels into display page bytes
#
# Neil Gershenfeld 11/1/19
#
import sys
import json
#
infile = open(sys.argv[1],'r')
rows = 8
#
bits = []
while True:
   c = infile.read(1)
   if not c:
      break
   if (c == '0'):
      bits.append(0)
   elif (c == '1'):
      bits.append(1)
pts = len(bits)/rows
#
bytes = []
for i in range(pts):
   byte = (1 << 0)*bits[0*pts+i] \
      + (1 << 1)*bits[1*pts+i] \
      + (1 << 2)*bits[2*pts+i] \
      + (1 << 3)*bits[3*pts+i] \
      + (1 << 4)*bits[4*pts+i] \
      + (1 << 5)*bits[5*pts+i] \
      + (1 << 6)*bits[6*pts+i] \
      + (1 << 7)*bits[7*pts+i]
   bytes.append(byte)
#
for i in range(len(bytes)):
   print("{:08b}".format(bytes[i]))
#
outfile = open(sys.argv[2],'w')
json.dump(bytes,outfile)
