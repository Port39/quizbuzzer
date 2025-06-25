// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
//

#include <avr/io.h>
#include <stdint.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "util.h"
#include "uart.h"
#include "buzz.h"
#include "7segment.h"

#define PLAYERS 4

static volatile uint8_t displayedPlayer = 255;
static volatile uint8_t players[PLAYERS];

void lightPlayer(uint8_t position) {
    if (position > PLAYERS) return;
    PORTB &= 0xc3;
    PORTB |= (1 << (players[position] + PIN2));
}

void addPlayer(uint8_t player) {
    for (int i = 0; i < PLAYERS; i++) {
        if (players[i] == player) return;
        if (players[i] == 255) {
            players[i] = player;
            if (!i) {
                displayedPlayer = 0;
                lightPlayer(0);
            }
            break;
        }
    }
}

void newGame(void) {
    cli();
    displayedPlayer = 255;
    for (int i = 0; i < PLAYERS; i++) {
        players[i] = 255;
        PORTB &= ~ (1 << (PIN2+i));
    }
    PORTC |= 1 << PIN0;
    sei();
    printf("Ready!\n");
}

int main(void) {
    // "Ready" LED
    DDRC |= (1 << PIN0);

    // "Player 1-4 is active" LEDs
    DDRB |= (1 << PIN2);
    DDRB |= (1 << PIN3);
    DDRB |= (1 << PIN4);
    DDRB |= (1 << PIN5);

    DDRC |=  (1 << PIN0);  // green LED
    DDRC &= ~(1 << PIN1);  // P1
    DDRC &= ~(1 << PIN2);  // P2
    DDRC &= ~(1 << PIN3);  // P3
    DDRC &= ~(1 << PIN4);  // P4
    DDRC &= ~(1 << PIN5);  // New Game

    PORTC |= (1 << PIN1);
    PORTC |= (1 << PIN2);
    PORTC |= (1 << PIN3);
    PORTC |= (1 << PIN4);
    PORTC |= (1 << PIN5);

    // Enable interruprs for PCINT8-14 (though we only use 9-13; PORTC1-5)
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13);

    init_serial();
    newGame();

    for (;;) {}
}


ISR(PCINT1_vect) {
    //printf("Interrupt!");
    //PORTB |= 0xff;
    // Check if pins are triggered
    for (int i = 0; i < 5; i++) {
        printf("Register: %d ", PINC);
        if ((PINC & (1 << (PIN1 + i)))) continue;

        if (i<4) {
            addPlayer(i);
            buzz(1000);
        } else {
            newGame();
        }
    }
}


