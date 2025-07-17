#include <stdint.h>
#include <avr/io.h>

#define SEGMENT_A (1 << PIN2)
#define SEGMENT_B (1 << PIN3)
#define SEGMENT_C (1 << PIN6)
#define SEGMENT_D (1 << PIN7)
#define SEGMENT_E (1 << PIN4)
#define SEGMENT_F (1 << PIN1)
#define SEGMENT_G (1 << PIN0)
#define INVERT_SEGMENTS 1

void setDigit(volatile uint8_t *port, int value);
