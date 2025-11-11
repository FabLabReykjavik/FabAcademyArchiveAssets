#
# hello.A4949.RP2040.py
#    A4949 BLDC RP2040 hello-world
#
# Neil Gershenfeld 10/27/24
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
import machine,sys,time

machine.freq(250_000_000)

pwm_freq = 100_000_000
pwm_range = 1000
pwm_pin = 0

@asm_pio(sideset_init=PIO.OUT_LOW)
def pwm_prog():
    pull(noblock).side(0) # empty FIFO moves X to OSR
    mov(x,osr) # OSR has PWM threshold
    mov(y,isr) # ISR has PWM range
    label("loop")
    jmp(x_not_y,"skip")
    nop().side(1) # toggle output when X matches Y
    label("skip")
    jmp(y_dec,"loop") # loop over Y

class pwm_pio:
    def __init__(self,sm_id,pwm_pin,pwm_range,pwm_freq):
        self._sm = StateMachine(sm_id,pwm_prog,freq=pwm_freq,sideset_base=machine.Pin(pwm_pin))
        self._sm.put(pwm_range)
        self._sm.exec("pull()")
        self._sm.exec("mov(isr,osr)")
        self._sm.active(1)
        self._max_count = pwm_range
        self.put = self._sm.put

pwm = pwm_pio(0,pwm_pin,pwm_range,pwm_freq)
pwm.put(0)

@asm_pio()
def cycle_timer_prog():
    label("start")
    #
    pull() # load and save timer init
    mov(x,osr)
    mov(y,x)
    #
    # wait for pin high
    #
    label("wait high")
    jmp(pin,"wait high continue")
    jmp(x_dec,"wait high")
    jmp("timeout")
    #
    label("wait high continue")
    #
    # wait for pin low
    #
    label("wait low")
    jmp(pin,"wait low skip")
    jmp("wait low continue")
    label("wait low skip")
    jmp(x_dec,"wait low")
    jmp("timeout")
    #
    label("wait low continue")
    mov(x,y) # reset x
    #
    # time low
    #
    label("time low")
    jmp(pin,"time low continue") # 1 clock cycle
    jmp(x_dec,"time low") # 1 clock cycle
    jmp("timeout")
    #
    label("time low continue")
    #
    # time high
    #
    label("time high")
    jmp(pin,"time high skip") # 1 clock cycle
    jmp("time high continue")
    label("time high skip")
    jmp(x_dec,"time high") # 1 clock cycle
    jmp("timeout")
    #
    label("time high continue")
    #
    # return
    #
    mov(isr,x)
    push()
    jmp("start")
    #
    # timeout
    #
    label("timeout")
    mov(isr,y)
    push()
    jmp("start")

class cycle_timer_pio:
    def __init__(self,sm_id,cycle_timer_pin,cycle_timer_freq):
        self._sm = rp2.StateMachine(sm_id,cycle_timer_prog,
            jmp_pin=machine.Pin(cycle_timer_pin),freq=cycle_timer_freq)
        self._sm.active(1)
        self.get = self._sm.get
        self.put = self._sm.put

cycle_timer_pin = 7
cycle_timer_freq = 10_000_000
cycle_timer_init = 1_000_000
cycle_timer = cycle_timer_pio(1,cycle_timer_pin,cycle_timer_freq)

def get_count():
    cycle_timer.put(cycle_timer_init)
    return cycle_timer_init-cycle_timer.get()

while True:
    percent = float(input("PWM percent (0-100)? "))
    count = int((percent/100)*pwm_range)
    pwm.put(count)
    count = get_count()
    lastcount = -1
    tolerance = 0.0001
    while True:
        lastcount = count
        count = get_count()
        if (count != 0):
           freq = int(cycle_timer_freq/(2*count)) # 2 clock cycles/count
           print(f"   {freq} Hz")
           if (abs(count-lastcount)/count < tolerance): break
        else:
            print(f"   0 Hz")
            if (count == lastcount): break
        time.sleep_ms(10)
