;
; hello.speaker.45.wave.asm
;
; wave-table audio hello-world program
;
; Neil Gershenfeld
; CBA MIT 8/3/07
;
; (c) Massachusetts Institute of Technology 2007
; Permission granted for experimental and personal use;
; license for commercial sale available from MIT.
;
.include "tn45def.inc"
; 
; register definitions
;
.def temp = R16	; temporary storage
.def temp1 = R17 ; temporary storage
.def sample_count = R18 ; sample counter
.def sample_delay = R19 ; delay between samples
.def delay_count = R20 ; delay loop counter
.def cycle_count = R21 ; cycle counter
.def note_count = R23 ; note counter
.def note_lo = R24 ; note low byte
.def note_hi = R25 ; note high byte
;
; code segment
;
.cseg
.org 0
rjmp reset
;
; waveform
; first byte = number of samples
;
wave:
   .db 99,191,195,199,203,207,211,215,218,222
   .db 225,229,232,235,238,240,243,245,247,249
   .db 250,252,253,254,254,254,254,254,254,253
   .db 252,251,250,248,246,244,242,239,236,233
   .db 230,227,224,220,216,213,209,205,201,197
   .db 193,189,185,181,177,173,169,166,162,158
   .db 155,152,149,146,143,140,138,136,134,132
   .db 131,130,129,128,128,128,128,128,128,129
   .db 130,132,133,135,137,139,142,144,147,150
   .db 153,157,160,164,167,171,175,179,183,187
;
; notes
; first byte = number of notes
; following notes are specified by the delay between samples
;   and then the number of cycles
;
notes:
   .db 18,5,250,5,250,5,250,5,250,5,250,5,250,10,250,10,250,10,250,20,250,20,250,20,250,15,250,30,200,40,100,35,100,35,100,25,200
;
; main program
;
reset:	   
   ;
   ; initialization
   ;
   ldi temp, low(RAMEND)
   out SPL, temp ; set stack pointer to top of RAM
   ldi temp, (1<<CLKPCE)
   ldi temp1, ((0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0))
   out CLKPR, temp ; write CLKPCE to change clock prescaler
   out CLKPR, temp1 ; set clock to 9.6MHz/1
   ldi temp, ((1 << COM0B0) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00))
   out TCCR0A, temp ; set OC0B on compare match and set fast PWM mode, 0xFF TOP
   ldi temp, (1 << CS00)
   out TCCR0B, temp ; set timer 0 prescalar to 1
   sbi DDRB, PB1 ; enable OC0B output pin
   ldi sample_delay, 60
   ldi cycle_count, 50
   ;
   ; main loop
   ;
   main_loop:
      ;
      ; read number of notes and set note pointer
      ;
      ldi zl, low(notes*2)
      ldi zh, high(notes*2)
      lpm
      mov note_count, R0
      adiw zl, 1
      movw note_lo, zl
      ;
      ; loop over notes
      ;
      inc note_count
      note_loop:
         dec note_count
	     breq main_loop
         movw zl, note_lo
	 lpm
	 mov sample_delay, R0
	 adiw note_lo, 1
         movw zl, note_lo
	 lpm
	 mov cycle_count, R0
	 adiw note_lo, 1
	 ;
	 ; loop over cycles
	 ;
	 inc cycle_count
	 cycle_loop:
	    dec cycle_count
	       breq note_loop
            ;
            ; read number of samples
            ;
            ldi zl, low(wave*2)
            ldi zh, high(wave*2)
            lpm
            mov sample_count, R0
            ;
	         ; loop over samples
	         ;
            sample_loop:
               adiw zl, 1 ; move to next sample
               lpm ; read sample
               out OCR0B, R0 ; update PWM
               mov delay_count, sample_delay
               sample_delay_loop:
                  dec delay_count
                  brne sample_delay_loop
               dec sample_count ; decrease sample count
               brne sample_loop ; get next sample, ...
                  rjmp cycle_loop ; ... or repeat cycle if done

