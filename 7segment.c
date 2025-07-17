// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
//

#include "7segment.h"

// TODO: Find out ports
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

void setDigit(volatile uint8_t *port, int value) {
    if (value >= 16) return;
#ifdef INVERT_SEGMENTS
    *port = 0xff & (~chars[value]);
#else
    *port = chars[value];
#endif

}