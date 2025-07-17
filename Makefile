CC=avr-gcc
GDB=avr-gdb
CXX=avr-g++
LD=avr-ld
AVRDUDE=avrdude

# CPU model
MCU?=atmega328p
# Frequency
#F_CPU?=16000000
F_CPU?=1000000

# Programmer and transmission settings

# If the Arduino bootload is in use:
#PROGRAMMER=arduino
#BAUD=9600
# If you use another Arduino as an ISP programmer:
PROGRAMMER?=stk500v1
BAUD?=19200

PORT?=/dev/ttyACM0

# Arguments shared by C and C++ compiles
common = -Os -DBAUD=$(BAUD) -D__AVR_$(MCU)__ -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Wall -Wextra -pedantic -Wno-unknown-pragmas -lc -lm
# C compiler arguments
CFLAGS := $(CFLAGS) $(common) -std=gnu11
# C++ compiler arg
CPPFLAGS := $(CPPFLAGS) $(common) "-std=gnu++11"
# Linker flags
LDFLAGS := $(LDFLAGS) -Wl,--print-memory-usage -mmcu=$(MCU) -flto

# Main target
OUT=buzzer.hex

.PHONY: all

all: $(OUT)

flash: $(OUT)
	$(AVRDUDE) -v -V -p $(MCU) -c $(PROGRAMMER) "-P$(PORT)" -b$(BAUD) "-Uflash:w:$(OUT):i"

OBJS = main.o uart.o buzz.o 7segment.o
DEP = $(OBJS:%.o=%.d)

buzzer.elf: $(OBJS)
	$(CXX) $(LDFLAGS) -o buzzer.elf $^

# For every existing
-include $(DEP)

%.hex : %.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

# The -MMD creates
%.o : %.c
	$(CC) -c $(CFLAGS) -MMD $< -o $@

%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) -MMD $< -o $@

clean:
	rm -f *.o
	rm -f *.d
	rm -f *.hex
	rm -f *.elf