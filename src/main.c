#include <avr/io.h>
#include <util/delay.h>
#include "../common/pin.h"

/* A shift register connected to a 7-segments led matrix
 *
 * This allows us to demonstrate how to use a 7-segments matrix by using only 2 pins of
 * our MCU!
 *
 * - a 8bit shift register. I use a TI CD74AC164E
 * - a 7-segment led matrix. I use a Lite-On LSHD-A101
 *
 * Connect MR and DS2 to power so they're always high. CP to PB1, DS1 to PB2.
 *
 * Connect the matrix' common anode to power through a 110 ohms resistance.
 *
 * The LED matrix is connected to the shift register's output as follow:
 *
 * F = Q0
 * G = Q1
 * E = Q2
 * D = Q3
 * C = Q4
 * B = Q5
 * A = Q6
 * We leave DP alone, Q7 is not connected.
 *
 * The program will count from 0 to 9!
 */

// CP outputs to our 3 shift registers go through a 2-4 decoder.
#define CP1 PinB1
#define CP2 PinB2
#define DS PinB4

// Least significant bit is on Q0
//               XABCDEGF
#define Seg7_0 0b01111101
#define Seg7_1 0b00110000
#define Seg7_2 0b01101110
#define Seg7_3 0b01111010
#define Seg7_4 0b00110011
#define Seg7_5 0b01011011
#define Seg7_6 0b01011111
#define Seg7_7 0b01110000
#define Seg7_8 0b01111111
#define Seg7_9 0b01111011

typedef enum {
    SR1_CP,
    SR2_CP,
    SR3_CP
} SR_CP_PIN;

/* our decoder keep output pins high except for the selected one. To toggle our selected CP, our
 * strategy is to generally keep all CP pins high by selecting output Y3 of the decoder. Then, we
 * select the proper output to go low, wait for a shift register cycle, the select Y3 again.
 */
static void togglecp(SR_CP_PIN pin)
{
    switch (pin) {
        case SR1_CP:
            pinlow(CP1);
            pinlow(CP2);
            break;
        case SR2_CP:
            pinlow(CP2);
            break;
        case SR3_CP:
            pinlow(CP1);
            break;
    }
    _delay_us(100);
    // select y3 (all high)
    pinhigh(CP1);
    pinhigh(CP2);
}

// LSB goes on q0
static void shiftsend(SR_CP_PIN pin, unsigned char val)
{
    char i;

    for (i=7; i>=0; i--) {
        pinset(DS, val & (1 << i));
        togglecp(pin);
    }
}

int main (void)
{
    unsigned char digits[10] = {Seg7_0, Seg7_1, Seg7_2, Seg7_3, Seg7_4, Seg7_5, Seg7_6, Seg7_7, Seg7_8, Seg7_9};
    unsigned char i;

    pinoutputmode(CP1);
    pinoutputmode(CP2);
    pinoutputmode(DS);

    // All CP pins high. See togglecp()
    pinhigh(CP1);
    pinhigh(CP2);

    for (i=0; i<10; i++)
    {
        // We use bitwise NOT because our 7seg is a Common Anode, which means that *low* pins are
        // enabled.
        shiftsend(SR1_CP, ~digits[i]);
        shiftsend(SR2_CP, ~digits[i]);
        _delay_ms(1000);
    }
}
