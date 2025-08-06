// SPDX-License-Identifier: MPL-2.0
// Copyright (c) 2025 Simon Biewald
//
// Interface implementations for board revision 1.
//
// +--------+
// |    PB0 | Next
// |    PB1 | Buzzer (Audio)
// | PB2..5 | Player 1..4
// |        |
// |    PC0 | Ready
// |  PC1-4 | Player 1..4 (Input)
// |    PC5 | New
// |        |
// | PD0..7 | 7-segment (potentially cursed pinout)
// +--------+

#if BOARD_REVISION == 1
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "board.h"
#include "board_revision1.h"

#include "logic.h"

void setupHardware(void) {
    // Buzzer
    DDRB |= (1 << PIN1);

    // "Player 1-4 is active" LEDs
    DDRB |= (1 << PIN2);
    DDRB |= (1 << PIN3);
    DDRB |= (1 << PIN4);
    DDRB |= (1 << PIN5);

    // Enable pull-up for the "next player" button.
    PORTB |= (1 << PIN0);

    DDRC |=  (1 << PIN0);  // green LED
    DDRC &= ~(1 << PIN1);  // P1
    DDRC &= ~(1 << PIN2);  // P2
    DDRC &= ~(1 << PIN3);  // P3
    DDRC &= ~(1 << PIN4);  // P4
    DDRC &= ~(1 << PIN5);  // New Game

    // Enable pull-up resistors for P1-4
    // Buttons connect the pin with +5V
    PORTC |= (1 << PIN1);
    PORTC |= (1 << PIN2);
    PORTC |= (1 << PIN3);
    PORTC |= (1 << PIN4);
    PORTC |= (1 << PIN5);

    // 7-segment output
    DDRD = 0xff;

    // Ensure capacitors are charged before enabling interrupts.
    _delay_ms(100);

    // Enable interrupts for PCINT8-14 (though we only use 9-13; PORTC1-5)
    PCICR |= (1 << PCIE1);
    // Players + New Game
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13);
    // Next player
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);
}


static uint8_t chars [16] = {
        SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,
        SEGMENT_B | SEGMENT_C,
        SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_E | SEGMENT_D,
        SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_C | SEGMENT_D,
        SEGMENT_F | SEGMENT_G | SEGMENT_B | SEGMENT_C,
        SEGMENT_A | SEGMENT_F | SEGMENT_G | SEGMENT_C | SEGMENT_D,
        SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_E | SEGMENT_D | SEGMENT_C,
        SEGMENT_A | SEGMENT_B | SEGMENT_C,
        SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,
        SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,
};


void setDigit(int digit) {
    if (digit >= 16) return;
#ifdef INVERT_SEGMENTS
    PORTD = 0xff & (~chars[digit]);
#else
    PORTD = chars[value];
#endif
}

void highlightPlayer(unsigned char player) {
    if (player > MAX_PLAYERS && player != PLAYER_PLACEHOLDER) return;
    PORTB &= 0xc3;
    if (player == PLAYER_PLACEHOLDER) {
        PORTC |= 1 << PIN0;
        return;
    }
    PORTB |= (1 << (player + PIN2));
    PORTC &= ~(1 << PIN0);
}

void toggleBuzzer(void) {
    PORTB ^= (1 << PIN1);
}

void setBuzzerActive(bool active) {
    // no-op
    (void)active;
}


ISR(PCINT1_vect) {
        // Check if pins are triggered
        for (int i = 0; i < 5; i++) {
            if ((PINC & (1 << (PIN1 + i)))) continue;

            if (i<4) {
                // Player button
                addPlayer(i);
                buzz(1000);
            } else {
                newGame();
            }
        }
}

ISR(PCINT0_vect) {
    // Skip the "up" event.
    if (PINB & (1 << PIN0)) return;

    nextPlayer();
}

#endif
