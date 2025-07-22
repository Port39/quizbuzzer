# Simple quiz buzzer

Features:
- Four/six inputs for buttons.
- Five/eight open collector outputs to trigger external periphery.
- 7 segment
- Powered by an ATmegaXY8(P) (e.g., an Atmega328, or even an ATmega48)

## Building and flashing the firmware

You need AVR-GCC or something compatible to build this.
Just run `make` and then `make flash`.
The latter comand uses *avrdude* to flash your µC.
You _should_ be able to build the firmware with ~~Atmel~~ Microchip Studio as well.
Set the variables of the makefile accordingly:

- `MCU` (`atmega328p`): Set to your variant of the uC (e.g., if you have an ATmega168, set it to `atmega168`)
- `F_CPU` (`1000000`): The CPU speed your controller runs at. This does not set the actual CPU speed (set fuses for that!), but just tells the compiler. Currently only `1000000` and `16000000` are supported yet.
- `PORT` (`/dev/ttyUSB0`): The port your programmer is connected to.
- `PROGRAMMER` (`arduino`): The programmer you use (see below for some common configurations)
- `BAUD` (`9600`): The baud rate of the programmer.

### Programmer and baud rate combinations

- "Arduino as ISP": `PROGRAMMER=stk500v1`, `BAUD=19200`
- USB bootloader from the Arduino project: `PROGRAMMER=arduino`, `BAUD=9600`
- "mySmartUSB light" in stk500 mode: `PROGRAMMER=stk500`, `BAUD=115200`

## Schematics

Schematics are made with KiCad.
Only standard components were used (or the used footprint is embedded).

## Revisions

The firmware is not compatible between board revisions.

### Revision 1

Revision 1 is for building on a 7x9cm prototyping board.

Depending on the pinout of your 7-segment display, you may need to change the array in *7segment.c*.

### Revision 2

Revision 2 (planned) is for soldering an manufactured PCB.
The serial (UART) outputs are available for programming (if a bootloader is flashed),
and output the current game state.

The second revision has the following upgrades:
- A CH340 on the board. This allows serial communication and programming over USB!
- Thanks to multiplexers, up to six players can play now!
- And an additional open collector "buzzer" pin is added. This allows for external buzzers circuits.
- There now is a power on LED.