// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
//

#include "buzz.h"

#include <avr/interrupt.h>
#include <math.h>


#if F_CPU != 16000000u && F_CPU != 8000000UL

#endif

static volatile uint16_t count = 0;

void buzz(uint16_t milliseconds) {
    DDRB |= (1 << PIN1);
    TCCR2A = 0;
#if F_CPU == 16000000u
    TCCR2B = 5;
#elif F_CPU == 8000000u
    TCCR2B = 4;
#else
    #error CPU clock must be either 16 MHz or 8 MHz!
#endif
    TIMSK2 = 1;

    count = milliseconds / 2;
}

ISR(TIMER2_OVF_vect) {
    count--;
    if (!count) {
        TIMSK2 = 0;
        return;
    }
    PORTB ^= (1 << PIN1);
}
