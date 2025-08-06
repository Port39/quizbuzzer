// SPDX-License-Identifier: MPL-2.0
// Copyright (c) 2025 Simon Biewald
//
// Interface implementations for board revision 2.
//
// +--------+
// |    PB0 | Next
// |    PB1 | New
// |    PB2 | Buzzer (Audio)
// |    PB5 | Ext. Buzzer
// |        |
// | PC0..2 | Ready, Player 1..6 (BCD)
// | PC3..5 | 7-segment (BCD)
// |        |
// |    PD0 | RX
// |    PD1 | TX
// | PD2..7 | Player 1..6 (Input)
// +--------+

#if BOARD_REVISION == 2
#include "board.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// #include "board_revision2.h"

#include "logic.h"

void setupHardware(void) {
    // Next, New
    DDRB &= ~(1 << PIN0) | (1 << PIN1);
    // Next, New (Pull up)
    PORTB |= (1 << PIN0);
    PORTB |= (1 << PIN1);

    // Buzzer (Audio)
    DDRB |= (1 << PIN2);
    // Buzzer (external trigger)
    DDRB |= (1 << PIN5);

    // Ready and player 1-6 LEDs
    DDRC |= (1 << PIN0) | (1 << PIN1) | (1 << PIN2);
    // 7-segment
    DDRC |= (1 << PIN3) | (1 << PIN4) | (1 << PIN5);
    // Set output to 0
    PORTC &= ~(1 << PIN0) | (1 << PIN1) | (1 << PIN2) | (1 << PIN3) | (1 << PIN4) | (1 << PIN5);

    // Player 1-6 buttons
    DDRD &= ~(1 << PIN2) | (1 << PIN3) | (1 << PIN4) | (1 << PIN5) | (1 << PIN6) | (1 << PIN7);
    // Player 1-6 buttons (pull up)
    PORTD |= (1 << PIN2) | (1 << PIN3) | (1 << PIN4) | (1 << PIN5) | (1 << PIN6) | (1 << PIN7);

    // Ensure capacitors are charged before enabling interrupts;
    // it would trigger a "buzzer" event otherwise.
    // This is strictly only necessary after a full board reset which we don't track.
    _delay_ms(100);

    // Enable interrupts for PORTD
    PCICR |= (1 << PCIE2);
    // Enable interrupts on PIN PD7..2 (PCINT23..18)
    PCMSK2 |= (1 << PCINT23) | (1 << PCINT22) | (1 << PCINT21) | (1 << PCINT20) | (1 << PCINT19) | (1 << PCINT18);

    // Enable interrupts for PORTB
    PCICR |= (1 << PCIE0);
    // Players + New Game PIN PB1..0 (PCINT1..0)
    PCMSK0 |= (1 << PCINT1) | (1 << PCINT0);
}


void setDigit(int digit) {
    // We only set three bits.
    if (digit >= 7) return;
    PORTC &= ~(7 << 3);
    PORTC |= (digit << 3);
}

void highlightPlayer(unsigned char player) {
    if (player == PLAYER_PLACEHOLDER) {
        PORTC &= ~7;
    }
    if (player > MAX_PLAYERS) return;
    // Player 0 would be game ready
    PORTC = (PORTC & ~7) | player + 1;
}

void toggleBuzzer(void) {
    PORTB ^= (1 << PIN2);
}

void setBuzzerActive(bool active) {
    if (active) {
        PORTB |= (1 << PIN5);
    } else {
        PORTB &= ~(1 << PIN5);
    }
}


ISR(PCINT0_vect) {
    if (!(PINB & (1 << PIN0))) nextPlayer();
    if (!(PINB & (1 << PIN1))) newGame();
}


ISR(PCINT2_vect) {
    for (int i = PIN2; i <= PIN7; i++) {
        // If the button is "up" (pull-up connects it with +5V), check next.
        if (PIND & (1 << i)) continue;
        addPlayer(i - PIN2);
        buzz(1000);
    }
}

#endif
