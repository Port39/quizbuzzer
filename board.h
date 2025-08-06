// Hardware interface definitions.

#ifndef BUZZER_BOARD_H
#define BUZZER_BOARD_H

// Set SERIAL on boards that support a serial output.
#if BOARD_REVISION == 1
#define MAX_PLAYERS 4

#elif BOARD_REVISION == 2
#define MAX_PLAYERS 6
#define SERIAL

#else
#error Unknown board revision or revision not set.
#endif  /* BOARD_REVISION */

#include <stdbool.h>

/**
 * Setup ports and other hardware.
 */
void setupHardware(void);

/**
 * Writes a digit on the 7-segment display.
 */
void setDigit(int digit);

/**
 * Sets the player's LED.
 * Can be called with PLAYER_PLACEHOLDER to indicate "game ready".
 */
void highlightPlayer(unsigned char player);

/**
* Toggles the buzzer pin (for tone generation).
*/
void toggleBuzzer(void);

/**
* Sets the "buzzer active" pin (e.g. for an external buzzer).
*/
void setBuzzerActive(bool active);

#endif
