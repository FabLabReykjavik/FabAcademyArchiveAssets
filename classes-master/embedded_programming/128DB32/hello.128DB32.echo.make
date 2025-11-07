PROJECT=hello.128DB32.echo
SOURCES=$(PROJECT).c
DEVICE = avr128db32
MMCU=$(DEVICE)
F_CPU = 24000000
PACK = path to AVR-Dx_DFP atpack goes here
PORT = /dev/ttyUSB1
BAUD = 19200

CFLAGS=-mmcu=$(MMCU) -Wall -Os -DF_CPU=$(F_CPU)

$(PROJECT).hex: $(PROJECT).out
	avr-objcopy -O ihex $(PROJECT).out $(PROJECT).hex;\
	avr-size --mcu=$(MMCU) --format=avr $(PROJECT).out
 
$(PROJECT).out: $(SOURCES)
	avr-gcc $(CFLAGS) -I./ -I$(PACK)/include -B$(PACK)/gcc/dev/$(MMCU) -o $(PROJECT).out $(SOURCES)

pyupdi: $(PROJECT).hex
	pyupdi -d $(DEVICE) -c $(PORT) -b $(BAUD) -v -f $(PROJECT).hex

pymcuprog: $(PROJECT).hex
	pymcuprog -d avr128db32 -t uart -u $(PORT) -c $(BAUD) -v info erase
	pymcuprog -d avr128db32 -t uart -u $(PORT) -c $(BAUD) -v info write -f $(PROJECT).hex
