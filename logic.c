// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
//

#include <avr/interrupt.h>
#include <stdio.h>

#include "logic.h"
#include "board.h"


static volatile uint8_t playerIndex = PLAYER_PLACEHOLDER;
static volatile uint8_t players[MAX_PLAYERS];

#ifdef SERIAL
static char lookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
#endif


static void printStatus(void) {
#ifdef SERIAL
    printf("S");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("%c", players[i] == PLAYER_PLACEHOLDER ? 'x' : lookup[players[i]] );
    }
    printf(".%c\n", lookup[playerIndex]);
#endif
}


void displayPosition(uint8_t position) {
    if (position > MAX_PLAYERS) return;
    if (players[position] == PLAYER_PLACEHOLDER) return;
    // Humans often are "1-indexed"
    setDigit(position+1);
    highlightPlayer(players[position]);
}


void nextPlayer(void) {
    // No player pressed a button yet.
    if (playerIndex == PLAYER_PLACEHOLDER) return;

    playerIndex++;
    if (playerIndex >= MAX_PLAYERS || players[playerIndex] == PLAYER_PLACEHOLDER) {
        playerIndex = 0;
    }
    displayPosition(playerIndex);
    printStatus();
}


void addPlayer(uint8_t player) {
    if (player >= MAX_PLAYERS) return;
#ifdef SERIAL
    printf("A%d\n", player);
#endif
    for (uint8_t i = 0; i < MAX_PLAYERS; i++) {
        // Player already in the list
        if (players[i] == player) break;
        // Place already used
        if (players[i] != PLAYER_PLACEHOLDER) continue;

        // Found the first free spot.
        players[i] = player;
        if (!i) {
            playerIndex = 0;
            displayPosition(0);
        }
        break;
    }
    printStatus();
}


void newGame(void) {
    cli();
    playerIndex = PLAYER_PLACEHOLDER;
    for (uint8_t i = 0; i < MAX_PLAYERS; i++) {
        players[i] = PLAYER_PLACEHOLDER;
    }
    highlightPlayer(PLAYER_PLACEHOLDER);
    setDigit(0);
    sei();
#ifdef SERIAL
    printf("N%d.%c\n", BOARD_REVISION, lookup[MAX_PLAYERS]);
#endif
}


static volatile uint16_t count = 0;

void buzz(uint16_t milliseconds) {
    TCCR2A = 0;
#if F_CPU == 16000000u
    TCCR2B = 5;
#elif F_CPU == 1000000u
    TCCR2B = 2;
#else
#error CPU clock must be either 16 MHz (external crystal) or 1 MHz (internal oscillator)!
#endif
    TIMSK2 = 1;
    setBuzzerActive(true);
    count = milliseconds / 2;
}


ISR(TIMER2_OVF_vect) {
        count--;
        if (!count) {
            TIMSK2 = 0;
            setBuzzerActive(false);
            return;
        }
        toggleBuzzer();
}
