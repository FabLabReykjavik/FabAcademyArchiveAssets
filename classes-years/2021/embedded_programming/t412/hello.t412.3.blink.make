PROJECT=hello.t412.3.blink
SOURCES=$(PROJECT).c
DEVICE = tiny412
PROGDEVICE = attiny412
MMCU=at$(DEVICE)
F_CPU = 20000000
PACK = path to ATtiny_DFP atpack goes here
PORT = /dev/ttyUSB0
BAUD = 57600

CFLAGS=-mmcu=$(MMCU) -Wall -Os -DF_CPU=$(F_CPU)

$(PROJECT).hex: $(PROJECT).out
	avr-objcopy -O ihex $(PROJECT).out $(PROJECT).hex;\
	avr-size --mcu=$(MMCU) --format=avr $(PROJECT).out
 
$(PROJECT).out: $(SOURCES)
	avr-gcc $(CFLAGS) -I./ -I$(PACK)/include -B$(PACK)/gcc/dev/$(MMCU) -o $(PROJECT).out $(SOURCES)

pyupdi: $(PROJECT).hex
	pyupdi -d $(DEVICE) -c $(PORT) -b $(BAUD) -v -f $(PROJECT).hex

pymcuprog: $(PROJECT).hex
	pymcuprog -d $(PROGDEVICE) -t uart -u $(PORT) -c $(BAUD) -v info erase
	pymcuprog -d $(PROGDEVICE) -t uart -u $(PORT) -c $(BAUD) -v info write -f $(PROJECT).hex

