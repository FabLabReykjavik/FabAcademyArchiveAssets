#
# convert_bytearray.py
#    save a WAV file to a byte array
#
# Neil Gershenfeld 12/4/23
#
from scipy.io import wavfile
import numpy as np
from struct import *
samplerate,data = wavfile.read("hello.wav")
max = np.max(data)
scale = 1023/max
amp = 0.7
b = bytearray()
for i in range(len(data)):
   b.extend(pack("<h",int(amp*scale*data[i])))
print(b)
