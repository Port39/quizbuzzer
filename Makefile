CC=avr-gcc
GDB=avr-gdb
CXX=avr-g++
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude

# CPU model
MCU?=atmega328p
# Frequency
# Only 16000000 or 1000000 are currently supported.
#F_CPU?=16000000
F_CPU?=1000000
# Baud rate of the UART
BAUD?=1200

# Set the board revision to compile the firmware for your board.
BOARD_REVISION?=2

# Programmer and transmission settings

# If the Arduino bootload is in use:
#PROGRAMMER=arduino
#PROGRAMMER_BAUD=9600
# If you use another Arduino as an ISP programmer:
#PROGRAMMER?=stk500v1
#PROGRAMMER_BAUD?=19200
# If you use the "mySmartUSB light" (select the STK500 mode with the Windows application!)
PROGRAMMER?=stk500
PROGRAMMER_BAUD?=115200

PORT?=/dev/ttyUSB0

# Arguments shared by C and C++ compiles
common = -Os -DBAUD=$(BAUD) -D__AVR_$(MCU)__ -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Wall -Wextra -pedantic -Wno-unknown-pragmas -lc -lm -DBOARD_REVISION=$(BOARD_REVISION)
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
	$(SIZE) --mcu atmega328p $(OUT) -C
	$(AVRDUDE) -v -V -p $(MCU) -c $(PROGRAMMER) "-P$(PORT)" -b$(PROGRAMMER_BAUD) "-Uflash:w:$(OUT):i"

OBJS = main.o uart.o logic.o board_revision1.o board_revision2.o
DEP = $(OBJS:%.o=%.d)

buzzer.elf: $(OBJS)
	$(CXX) $(LDFLAGS) -o buzzer.elf $^

# For every existing
-include $(DEP)

%.hex : %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

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