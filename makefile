#
# Makefile for programming AVR MCUs with Adafruits USBtinyISP
# Specifically the atmega328p
#
# For using this programmer with Linux without root access
# put the following commands in a .rules file 
#
# SUBSYSTEM=="usb",ATTRS{idVendor}=="1781",ATTRS{idProduct}=="0c9f",GROUP="users",MODE="0666"
# 
# put this file in /etc/udev/rules.d/ or similar folder.
#

CC=avr-gcc
UPLOADER = avrdude
CONVERTER = avr-objcopy

FILENAME = avrtest
MCU = atmega328p # ATmega328 (Arduino)
PROGRAMMER = usptiny

all:
	@echo "Cross compiling..."
	$(CC) -mmcu=$(MCU) -Os -D F_CPU=16000000 $ $(FILENAME).c -o $(FILENAME).out
	@echo "Done compiling"
	@echo "Converting to hex..."
	$(CONVERTER) -j .text -j .data -O ihex $(FILENAME).out $(FILENAME).hex
	@echo "Done converting"

upload: $(FILENAME).hex
	avrdude -c usbtiny -P usb -p m328p -U flash:w:$(FILENAME).hex:i

clean: *.out *.hex
	@echo "Cleaning project...."
	rm *.out *.hex 
