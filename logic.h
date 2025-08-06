// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
// Implements the board (but not MCU independent) functions.

#ifndef BUZZER_LOGIC_H
#define BUZZER_LOGIC_H

#include <stdint.h>

#define PLAYER_PLACEHOLDER 255

void buzz(uint16_t ms);
void displayPosition(uint8_t position);
void addPlayer(uint8_t player);
void newGame(void);
void nextPlayer(void);

static void printStatus(void);

#endif //BUZZER_LOGIC_H
