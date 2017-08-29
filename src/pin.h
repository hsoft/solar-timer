#pragma once

#include <stdbool.h>
/* Pin structure is 3 bits for the pin number and 2 bits for the port number */

typedef enum {
    PinB0 = 0b01000,
    PinB1 = 0b01001,
    PinB2 = 0b01010,
    PinB3 = 0b01011,
    PinB4 = 0b01100,
    PinB5 = 0b01101,
    PinB6 = 0b01110,
    PinB7 = 0b01111,
#ifdef PORTC
    PinC0 = 0b10000,
    PinC1 = 0b10001,
    PinC2 = 0b10010,
    PinC3 = 0b10011,
    PinC4 = 0b10100,
    PinC5 = 0b10101,
    PinC6 = 0b10110,
    PinC7 = 0b10111,
#endif
#ifdef PORTD
    PinD0 = 0b11000,
    PinD1 = 0b11001,
    PinD2 = 0b11010,
    PinD3 = 0b11011,
    PinD4 = 0b11100,
    PinD5 = 0b11101,
    PinD6 = 0b11110,
    PinD7 = 0b11111,
#endif
} Pin;

void pinhigh(Pin pin);
void pinlow(Pin pin);
void pinset(Pin pin, bool high);
void pintoggle(Pin pin);
bool pinishigh(Pin pin);
void pininputmode(Pin pin);
void pinoutputmode(Pin pin);
