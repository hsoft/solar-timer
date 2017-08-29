#pragma once

#include <avr/io.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define tbi(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit))
#define isset(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))

