#
# convert.py
#    save a WAV file to text
#
# Neil Gershenfeld 11/19/23
#
from scipy.io import wavfile
import numpy as np
samplerate,data = wavfile.read("hello.wav")
max = np.max(data)
scale = 65535/max
for i in range(len(data)):
   print(f"{(scale*data[i]).astype(int)},",end='')
