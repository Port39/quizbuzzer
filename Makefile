CC=avr-gcc
GDB=avr-gdb
CXX=avr-g++
LD=avr-ld
AVRDUDE=avrdude

# CPU model
MCU?=atmega328p
# Frequency
F_CPU?=16000000

# Programmer and settings
PROGRAMMER=arduino
PORT=/dev/ttyACM0
BAUD=115200

common = -Os -DBAUD=$(BAUD) -D__AVR_$(MCU)__ -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Wall -Wextra -pedantic -Wno-unknown-pragmas -lc -lm
CFLAGS := $(CFLAGS) $(common) -std=gnu11
CPPFLAGS := $(CPPFLAGS) $(common) "-std=gnu++11"

LDFLAGS := $(LDFLAGS) -Wl,--print-memory-usage -mmcu=$(MCU) -flto

OUT=buzzer.hex

.PHONY: all

all: $(OUT)

flash: $(OUT)
	$(AVRDUDE) -v -V -p $(MCU) -c $(PROGRAMMER) "-P$(PORT)" -b$(BAUD) -D "-Uflash:w:$(OUT):i"

OBJS = main.o uart.o buzz.o 7segment.o
DEP = $(OBJS:%.o=%.d)

buzzer.elf: $(OBJS)
	$(CXX) $(LDFLAGS) -o buzzer.elf $^

-include $(DEP)

%.hex : %.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

%.o : %.c
	$(CC) -c $(CFLAGS) -MMD $< -o $@

%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) -MMD $< -o $@

clean:
	rm -f *.o
	rm -f *.d
	rm -f *.hex
	rm -f *.elf