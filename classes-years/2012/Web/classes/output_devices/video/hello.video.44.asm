;
; hello.video.44.asm
; 
; NTSC/RS170 video hello-world
;
; set fuse low byte to 0x76 for 20 MHz resonator
;
; Neil Gershenfeld
; CBA MIT 11/18/07
;
; (c) Massachusetts Institute of Technology 2007
; Permission granted for experimental and personal use;
; license for commercial sale available from MIT.
;
.include "tn44def.inc"
;
; definitions
;
.equ pin_499 = PA7 ; 499 ohm DAC pin
.equ pin_1000 = PA6 ; 1000 ohm DAC pin
;
; registers
;
.def zero = R0 ; 0V
.def black = R1 ; .306V for 499,1k DAC
.def gray = R2 ; .613V for 499,1k DAC
.def white = R3 ; .919V for 499,1k DAC
.def temp = R16 ; temp variable
.def temp1 = R17 ; temp variable
.def row_counter = R18 ; row counter
.def col_counter = R19 ; col counter
; R30, 31 used for z pointer
;
; macros
;
.macro count
   ;
   ; delay counter
   ;
   ldi temp, @0 ; set match count
   ldi temp1, @1 ;
   out OCR1AH, temp ;
   out OCR1AL, temp1 ;
   clr temp ; clear counter
   out TCNT1H, temp ;
   out TCNT1L, temp ;
   sbi TIFR1, OCF1A ; clear count match flag
   count_loop:
      sbis TIFR1, OCF1A
      rjmp count_loop
   .endmacro
.macro 0V
   ;
   ; output 0V
   ;
   out PORTA, zero
   .endmacro
.macro black
   ;
   ; output black level
   ;
   out PORTA, black
   .endmacro
.macro horizontal_sync
   ;
   ; 4.7us horizontal sync
   ;
   count 0,82
   .endmacro
.macro front_porch
   ;
   ; off-screen beginning of scan line
   ;
   count 0,130
   .endmacro
.macro scan_line
   ;
   ; 58.8us complete scan line
   ;
   count 4,142
   .endmacro
.macro back_porch
   ;
   ; off-screen end of scan line
   ;
   count 0,70
   .endmacro
.macro blank_line
   ;
   ; blank horizontal scan line
   ;
   0V
   horizontal_sync
   black
   scan_line
   .endmacro
.macro sync_line
   ;
   ; vertical sync line
   ;
   ; inverted horizontal sync
   ;
   black
   horizontal_sync
   ;
   ; inverted scan line
   ;
   0V
   scan_line
   .endmacro
;
; code segment
;
.cseg
.org 0
rjmp reset
;
; main program
;
reset:
   ;
   ; set clock divider to /1
   ;
   ldi temp, (1 << CLKPCE)
   ldi temp1, (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0)
   out CLKPR, temp
   out CLKPR, temp1
   ; 
   ; set counter prescaler to /1
   ;
   ldi temp, (1 << CS10)
   out TCCR1B, temp
   ;
   ; set stack pointer to top of RAM
   ;
   ldi temp, high(RAMEND)
   out SPH, temp
   ldi temp, low(RAMEND)
   out SPL, temp
   ;
   ; init DAC pins
   ;
   cbi PORTA, pin_499
   sbi DDRA, pin_499
   cbi PORTA, pin_1000
   sbi DDRA, pin_1000
   ;
   ; set DAC value registers
   ;
   ldi temp, 0
   mov zero, temp
   ldi temp, (1 << pin_1000)
   mov black, temp
   ldi temp, (1 << pin_499)
   mov gray, temp
   ldi temp, ((1 << pin_499) | (1 << pin_1000))
   mov white, temp
   ;
   ; store test pattern in SRAM
   ;
   ldi zl, low(SRAM_START)
   ldi zh, high(SRAM_START)
   ldi col_counter, 40
   store_loop:
      st z+, black
      st z+, gray
      st z+, black
      st z+, white
      dec col_counter
      brne store_loop
   ;
   ; start main loop
   ;
   main_loop:
      ;
      ; 242 63.5us horizontal scan lines
      ;
      ldi row_counter, 242
      row_loop:
         0V
	 horizontal_sync
	 black
	 front_porch
	 ;
	 ; set z to point to test pattern
	 ;
         ldi zl, low(SRAM_START)
         ldi zh, high(SRAM_START)
	 ;
	 ;  column loop
	 ;
	 ldi col_counter, 160
	 col_loop:
	    ld temp, z+
	    out PORTA, temp
	    dec col_counter
	    brne col_loop
 	 black
	 back_porch
         dec row_counter
         breq row_loop_continue
	    rjmp row_loop
      row_loop_continue:
      ;
      ; 5 black lines
      ;
      ldi row_counter, 5
      black_loop:
         0V
	 horizontal_sync
         black
	 scan_line
         dec row_counter
         brne black_loop
      ;
      ; 3 vertical sync lines
      ;
      ldi row_counter, 3
      sync_loop:
         sync_line
         dec row_counter
         brne sync_loop
      ;
      ; 12 black lines
      ;
      ldi row_counter, 12
      black_loop1:
         0V
	 horizontal_sync
         black
	 scan_line
         dec row_counter
         brne black_loop1
      ;
      ; 60Hz main loop
      ;
      rjmp main_loop

