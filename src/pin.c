#include "pin.h"
#include "util.h"

void pinhigh(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            sbi(PORTB, pin & 0b111);
            break;
#ifdef PORTC
        case 0b10:
            sbi(PORTC, pin & 0b111);
            break;
#endif
#ifdef PORTD
        case 0b11:
            sbi(PORTD, pin & 0b111);
            break;
#endif
    }
}

void pinlow(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            cbi(PORTB, pin & 0b111);
            break;
#ifdef PORTC
        case 0b10:
            cbi(PORTC, pin & 0b111);
            break;
#endif
#ifdef PORTD
        case 0b11:
            cbi(PORTD, pin & 0b111);
            break;
#endif
    }
}

void pinset(Pin pin, bool high)
{
    if (high) {
        pinhigh(pin);
    } else {
        pinlow(pin);
    }
}

void pintoggle(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            tbi(PORTB, pin & 0b111);
            break;
#ifdef PORTC
        case 0b10:
            tbi(PORTC, pin & 0b111);
            break;
#endif
#ifdef PORTD
        case 0b11:
            tbi(PORTD, pin & 0b111);
            break;
#endif
    }
}

bool pinishigh(Pin pin)
{
    unsigned char portval = 0;
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            portval = PINB;
            break;
#ifdef PINC
        case 0b10:
            portval = PINC;
            break;
#endif
#ifdef PIND
        case 0b11:
            portval = PIND;
            break;
#endif
    }
    return (portval & (1 << (pin & 0b111))) != 0;
}

void pininputmode(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            cbi(DDRB, pin & 0b111);
            break;
#ifdef DDRC
        case 0b10:
            cbi(DDRC, pin & 0b111);
            break;
#endif
#ifdef DDRD
        case 0b11:
            cbi(DDRD, pin & 0b111);
            break;
#endif
    }
}

void pinoutputmode(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            sbi(DDRB, pin & 0b111);
            break;
#ifdef DDRC
        case 0b10:
            sbi(DDRC, pin & 0b111);
            break;
#endif
#ifdef DDRD
        case 0b11:
            sbi(DDRD, pin & 0b111);
            break;
#endif
    }
}

