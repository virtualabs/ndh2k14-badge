# 16k bootloader addr = 0x3800
# 8k bootloader addr = 0x1800
# 32k bootloader addr for 4k bootloader = 0x7000
# 32k bootloader addr for 2k bootloader = 0x7800

F_CPU = 12000000
DEVICE = atmega328p
BOOTLOADER_ADDRESS = 0x7000
AVRDUDE = avrdude -c usbasp -p $(DEVICE) # edit this line for your programmer
FUSE_L  = 0xFF
FUSE_H  = 0xD8
FUSE_E  = 0xFF
LOCK    = 0xCF

#Fuses
#atmega8: -U lfuse:w:0x7F:m -U hfuse:w:0xD8:m -U lock:w:0xCF:m
#atmega88: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega88p: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega168: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega168p: -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xF8:m -U lock:w:0xCF:m
#atmega328p (4k bootloader): -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0xFF:m -U lock:w:0xCF:m
#atmega328p (2k bootloader): -U lfuse:w:0xFF:m -U hfuse:w:0xDA:m -U efuse:w:0xFF:m -U lock:w:0xCF:m

CC = avr-gcc

# Options:
DEFINES = #-DDEBUG_LEVEL=2
CFLAGS = -Wall -Os -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) $(DEFINES)
LDFLAGS =

OBJECTS =  WORLDcodes.o main.o

# symbolic targets:
all: main.hex

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(CC) $(CFLAGS) -S $< -o $@

# rule for uploading firmware:
flash: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U lfuse:w:$(FUSE_L):m -U hfuse:w:$(FUSE_H):m -U efuse:w:$(FUSE_E):m


clean:
	rm -f main.hex main.bin *.o main.s

main.bin:	$(OBJECTS)
	$(CC) $(CFLAGS) -o main.bin $(OBJECTS) $(LDFLAGS)

main.hex:	main.bin
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex
	avr-size main.hex

disasm:	main.bin
	avr-objdump -d main.bin

cpp:
	$(CC) $(CFLAGS) -E main.c

