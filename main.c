// SPDX-License-Identifier: 
// Copyright (c) 2025 Simon Biewald
//
//

#include "board.h"
#include "logic.h"
#include "uart.h"

int main(void) {
    setupHardware();
#ifdef SERIAL
    init_serial();
#endif
    newGame();

    // This is an event (interrupt) based program.
    // The following loop prevents the controller from halting.
    for (;;);
}
