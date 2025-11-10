;
; hello.ftdi.echo.44.asm
;
; 115200 baud FTDI character echo
;
; Neil Gershenfeld
; 10/17/10
;
; (c) Massachusetts Institute of Technology 2010
; Permission granted for experimental and personal use;
; license for commercial sale available from MIT.
;

.include "tn44def.inc"

.equ rxpin = PA0; serial input pin
.equ txpin = PA1; serial output pin
.equ comm_port = PORTA; comm port
.equ comm_dir = DDRA; comm direction
.equ comm_pins = PINA; comm pins
.def bitcnt = R16; bit counter
.def temp = R17; temporary storage
.def temp1 = R18; temporary storage
.def txbyte = R19; transmit byte
.def rxbyte = R20; receive byte

;
; print
; 
.macro print
   ldi zl,low(@0*2)
   ldi zh,high(@0*2)
   rcall print_db
   .endmacro

.cseg
.org 0
rjmp reset

;
; half_bit_delay
; serial half bit delay
;
half_bit_delay:
   ldi temp, 25; 115200 baud (20 MHz clock /1)
   half_bit_delay_loop:
      dec temp
      brne half_bit_delay_loop
   ret
;
; putchar
;    assumes line driver (inverts bits)
;
putchar:
   ldi bitcnt, 10; 1 start + 8 data + 1 stop bit
   com txbyte; invert everything
   sec; set start bit
   putchar0:
      brcc putchar1; if carry set
      cbi comm_port, txpin; send a '0'
      rjmp putchar2; else	
   putchar1:
      sbi comm_port, txpin	; send a '1'
      nop; even out timing
   putchar2:
      rcall half_bit_delay; bit delay
      rcall half_bit_delay; " 
      lsr txbyte; get next bit
      dec bitcnt; if not all bits sent
      brne putchar0; send next bit
   ret;
;
; getchar
;    assumes line driver (inverts bits)
;
getchar:
   ldi bitcnt, 9; 8 data + 1 stop bit
   getchar1:
      sbic comm_pins, rxpin; wait for start bit
      rjmp getchar1
   rcall half_bit_delay; delay to middle of bit
   getchar2:
      rcall half_bit_delay; bit delay
      rcall half_bit_delay; "
      clc; clear carry
      sbic comm_pins, rxpin; if RX pin low skip
         sec; otherwise set carry
      dec bitcnt
      breq getchar3; return if all bits read
      ror rxbyte; otherwise shift bit into receive byte
      rjmp getchar2; go get next bit
   getchar3:
      ret
;
; print_db
; prints a null-terminated .db string
;
print_db:
   print_loop:
      lpm
      mov txbyte,R0
      cpi txbyte,0
      breq return
      rcall putchar
      adiw zl, 1
      rjmp print_loop
   return:
      ret
;
; main program
;
reset:
   ;
   ; set fuse low byte to 0x7E for 20 MHz resonator
   ;
   ; set clock divider to /1
   ;
   ldi temp, (1 << CLKPCE)
   ldi temp1, (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0)
   out CLKPR, temp
   out CLKPR, temp1
   ;
   ; set stack pointer to top of RAM
   ;
   ldi temp, high(RAMEND)
   out SPH, temp
   ldi temp, low(RAMEND)
   out SPL, temp
   ;
   ; init comm pin
   ;
   sbi comm_port, txpin
   sbi comm_dir, txpin
   ;
   ; start main loop
   ;
   loop:
      rcall getchar
      print message
         message: .db "hello.ftdi.44.echo.asm: you typed ",0
      mov txbyte, rxbyte
      rcall putchar
      ldi txbyte,10
      rcall putchar
      rjmp loop
