#
# pixbyte16 infile outfile
#
# Neil Gershenfeld 11/1/19
#    pack 16xN one-bit image pixels into display page bytes
#
import sys
import json
#
infile = open(sys.argv[1],'r')
rows = 16
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
lobytes = []
hibytes = []
for i in range(pts):
   lobyte = (1 << 0)*bits[0*pts+i] \
      + (1 << 1)*bits[1*pts+i] \
      + (1 << 2)*bits[2*pts+i] \
      + (1 << 3)*bits[3*pts+i] \
      + (1 << 4)*bits[4*pts+i] \
      + (1 << 5)*bits[5*pts+i] \
      + (1 << 6)*bits[6*pts+i] \
      + (1 << 7)*bits[7*pts+i]
   lobytes.append(lobyte)
   hibyte = (1 << 0)*bits[8*pts+i] \
      + (1 << 1)*bits[9*pts+i] \
      + (1 << 2)*bits[10*pts+i] \
      + (1 << 3)*bits[11*pts+i] \
      + (1 << 4)*bits[12*pts+i] \
      + (1 << 5)*bits[13*pts+i] \
      + (1 << 6)*bits[14*pts+i] \
      + (1 << 7)*bits[15*pts+i]
   hibytes.append(hibyte)
#
outfile = open(sys.argv[2],'w')
json.dump(lobytes,outfile)
json.dump(hibytes,outfile)
