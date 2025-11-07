PROJECT=hello.ALS31313.t412
SOURCES=$(PROJECT).c
MCU=attiny412
FREQ = 20000000
PACK = /home/neilg/exe/avr/packs/ATtiny_DFP/
PORT = /dev/ttyACM1
BAUD = 57600

CFLAGS=-mmcu=$(MCU) -Wall -Os -DF_CPU=$(FREQ)

$(PROJECT).hex: $(PROJECT).out
	avr-objcopy -O ihex $(PROJECT).out $(PROJECT).hex;\
	avr-size --mcu=$(MCU) --format=avr $(PROJECT).out
 
$(PROJECT).out: $(SOURCES)
	avr-gcc $(CFLAGS) -I./ -I$(PACK)/include -B$(PACK)/gcc/dev/$(MCU) -o $(PROJECT).out $(SOURCES)

pymcuprog: $(PROJECT).hex
	pymcuprog -d $(MCU) -t uart -u $(PORT) -c $(BAUD) -v info erase
	pymcuprog -d $(MCU) -t uart -u $(PORT) -c $(BAUD) -v info write -f $(PROJECT).hex
