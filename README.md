# Simple quiz buzzer

Features:
- Four inputs for buttons.
- Five open collector outputs to trigger external periphery.
- 7 segment
- Powered by an ATmegaXY8P

Revisions:
- Revision 1 is for building on a 7x9cm prototyping board.
- Revision 2 (planned) is for soldering an ordered board. 
The serial (UART) outputs are available for programming (if a bootloader is flashed),
and output the current game state.

## Building and flashing the program

You need AVR-GCC or something compatible.
You _should_ be able to build the firmware with ~~Atmvel~~ Microchip Studio.
If you do not use an ATmega328, but another pin-compatible varient (like a 168 oder an 88) set the environment variable `MCU` to your variant.
As a high precision is not required, you may choose to not include the 16 MHz crystal;
set the variable `F_CPU` to 8000000 in this case.

## Notices for each revision

The firmware is not compatible between board revisions.

### Revision 1

Depending on the pinout of your 7-segment display, you may need to change array in *7segment.c*.

### Revision 2

TBD