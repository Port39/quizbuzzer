#ifndef BUZZER_BOARD_REVISION1_H
#define BUZZER_BOARD_REVISION1_H

// Pin definitions for the 7-segment display.
#define SEGMENT_A (1 << PIN2)
#define SEGMENT_B (1 << PIN3)
#define SEGMENT_C (1 << PIN6)
#define SEGMENT_D (1 << PIN7)
#define SEGMENT_E (1 << PIN4)
#define SEGMENT_F (1 << PIN1)
#define SEGMENT_G (1 << PIN0)
// Comment out for common cathode display
#define INVERT_SEGMENTS

#endif
