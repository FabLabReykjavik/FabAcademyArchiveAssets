#
# hello.TB67H451.RP2040.wave.py
#    XIAO RP2040 TB67H451 H-bridge audio waveform synthesis hello-world
#
# Neil Gershenfeld 12/3/23
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

from rp2 import PIO,StateMachine,asm_pio
import time,math,machine

machine.freq(250_000_000)

pwm_freq = 250_000_000
pwm_range = (1<<10)-1
cycle_count = 10
note_time_us = 150_000
note_amplitude = 0.5

# from J.S. Bach Invention No. 8
notes = [
  0,0,45,45,49,49,45,45,52,52,45,45,
  57,57,56,54,52,54,52,50,49,50,49,47,
  45,45,49,49,52,52,49,49,57,57,52,52,
  61,64,62,64,61,64,62,64,61,64,62,64,
  57,61,59,61,57,61,59,61,57,61,59,61,
  54,57,56,57,54,57,56,57,54,57,56,57,
  51,51,47,47,54,54,51,51,57,57,54,54,
  59,61,59,57,56,57,56,54,52,54,52,50,
  49,49,54,52,51,52,51,49,47,49,47,45,
  44,45,44,42,40,40,52,51,52,52,44,44,
  45,45,52,52,44,44,52,52,42,42,51,51,
  52,52,52,52,0,0
  ]
#    C#    Eb       F#    Ab    Bb       C#    Eb       F#    Ab    Bb
# C4    D4    E4 F4    G4    A4    B4 C5    D5    E5 F5    G5    A5    B5 C6
# 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64
pitches = [1e-6,27.50000,29.13524,30.86771,32.70320,34.64783,36.70810,38.89087,
    41.20344,43.65353,46.24930,48.99943,51.91309,55.00000,58.27047,61.73541,
    65.40639,69.29566,73.41619,77.78175,82.40689,87.30706,92.49861,97.99886,
    103.8262,110.0000,116.5409,123.4708,130.8128,138.5913,146.8324,155.5635,
    164.8138,174.6141,184.9972,195.9977,207.6523,220.0000,233.0819,246.9417,
    261.6256,277.1826,293.6648,311.1270,329.6276,349.2282,369.9944,391.9954,
    415.3047,440.0000,466.1638,493.8833,523.2511,554.3653,587.3295,622.2540,
    659.2551,698.4565,739.9888,783.9909,830.6094,880.0000,932.3275,987.7666,
    1046.502,1108.731,1174.659,1244.508,1318.510,1396.913,1479.978,1567.982,
    1661.219,1760.000,1864.655,1975.533,2093.005,2217.461,2349.318,2489.016,
    2637.020,2793.826,2959.955,3135.963,3322.438,3520.000,3729.310,3951.066,
    4186.009]

@asm_pio(sideset_init=PIO.OUT_LOW)
def pwm_prog():
    pull(noblock).side(0)
    mov(x,osr)
    mov(y,isr)
    label("loop")
    jmp(x_not_y,"skip")
    nop().side(1)
    label("skip")
    jmp(y_dec,"loop")

class pwm_pio:
    def __init__(self,sm_id,pin,max_count,count_freq):
        self._sm = StateMachine(sm_id,pwm_prog,freq=count_freq,sideset_base=machine.Pin(pin))
        self._sm.put(max_count)
        self._sm.exec("pull()")
        self._sm.exec("mov(isr,osr)")
        self._sm.active(1)
        self._max_count = max_count
        self.put = self._sm.put

pwm0 = pwm_pio(0,0,max_count=pwm_range,count_freq=pwm_freq)
pwm1 = pwm_pio(1,1,max_count=pwm_range,count_freq=pwm_freq)

@micropython.native
def play():
    t0 = sample = 0
    t = time.ticks_us()
    for note in range(len(notes)):
        pitch = pitches[notes[note]]
        tend = t+note_time_us
        sample_us = int(1e6/(cycle_count*pitch))
        while (t < tend):
            t = time.ticks_us()
            if ((t-t0) > sample_us):
                t0 = t
                duty_cycle = samples[sample]
                if (duty_cycle > 0):
                    pwm0.put(duty_cycle)
                    pwm1.put(-1)
                else:
                    pwm0.put(-1)
                    pwm1.put(-duty_cycle)
                sample += 1
                if (sample == cycle_count):
                    sample = 0

samples = []
for i in range(cycle_count):
    samples.append(int(note_amplitude*pwm_range*math.sin(2*math.pi*i/(cycle_count-1))))

while True:
    play()
