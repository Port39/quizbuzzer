// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
//

#include "7segment.h"

// TODO: Find out ports
static char chars [16] = {0x7, 0xaf, 0x25, 0x25, 0x8d, 0x45, 0x45, 0x2f, 0x5, 0x5, 0xd, 0xcd, 0xe5, 0xc5, 0x45, 0x4d};

void setDigit(volatile uint8_t *port, char value) {
    if (value >= 16) return;
    *port = chars[value];
}