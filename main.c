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
#include "buzz.h"
#include "7segment.h"

#define PLAYERS 4
#define PLAYER_PLACEHOLDER 255

static volatile uint8_t displayedPlayer = PLAYER_PLACEHOLDER;
static volatile uint8_t players[PLAYERS];

void lightPlayer(uint8_t position) {
    if (position > PLAYERS) return;
    if (players[position] == PLAYER_PLACEHOLDER) return;
    setDigit(&PORTD, position+1);
    PORTB &= 0xc3;
    PORTB |= (1 << (players[position] + PIN2));
    PORTC &= ~(1 << PIN0);  // Disable "green" ready LED
}

void addPlayer(uint8_t player) {
    for (int i = 0; i < PLAYERS; i++) {
        // Player already in the list
        if (players[i] == player) return;
        // Place already used
        if (players[i] != PLAYER_PLACEHOLDER) continue;

        // Found the first free spot.
        players[i] = player;
        if (!i) {
            displayedPlayer = 0;
            lightPlayer(0);
        }
        break;

    }
}

void newGame(void) {
    cli();
    displayedPlayer = PLAYER_PLACEHOLDER;
    for (int i = 0; i < PLAYERS; i++) {
        players[i] = PLAYER_PLACEHOLDER;
        PORTB &= ~ (1 << (PIN2+i));
    }
    PORTC |= 1 << PIN0;
    setDigit(&PORTD, 0);
    sei();
    printf("Ready!\n");
}

int main(void) {
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
    // Buttons connect the pin with ground.
    PORTC |= (1 << PIN1);
    PORTC |= (1 << PIN2);
    PORTC |= (1 << PIN3);
    PORTC |= (1 << PIN4);
    PORTC |= (1 << PIN5);

    // 7-segment output
    DDRD = 0xff;

    _delay_ms(100);

    // Enable interrupts for PCINT8-14 (though we only use 9-13; PORTC1-5)
    PCICR |= (1 << PCIE1);
    // Players + New Game
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13);
    // Next player
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);

    // init_serial();
    newGame();

    // This is an event (interrupt) based program.
    // The following loop prevents the controller from halting.
    for (;;);
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

ISR(PCINT0_vect) {
    // Skip the "up" event.
    if (PINB & (1 << PIN0)) return;

    // No player pressed a button yet.
    if (displayedPlayer == PLAYER_PLACEHOLDER) return;

    displayedPlayer++;
    if (displayedPlayer >= PLAYERS || players[displayedPlayer] == PLAYER_PLACEHOLDER) {
        displayedPlayer = 0;
    }
    lightPlayer(displayedPlayer);
}


