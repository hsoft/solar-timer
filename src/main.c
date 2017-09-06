#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../common/pin.h"
#include "../common/timer.h"
#include "../common/adc.h"

/* 3 7-segments displays managed with shift registers
 *
 * The goal is to control 3 7-segments displays using only 3 pins on the MCU. We do this
 * through 3 shift registers. Between these SRs and the MCU comes a 2-to-4 decoder, allowing
 * us to use one less pin.
 *
 * How it works is that serial input for all SRs are always the same (PB4 on the MCU), it's
 * SRs *clock* that is activated one at a time, through the decoder.
 *
 * Parts:
 *
 * - 2x TI CD74AC164E (8bit shift register)
 * - 1x TI SN74HC595N (I know, it needlessly complicated things, but I only had two CD74AC164E in stock :( )
 * - 1x TI SN74AHC139 (2-to-4 decoder)
 * - a 7-segment led matrix with a dot pin. I use a Lite-On LSHD-A101
 *
 * For SR1 and SR2, connect MR and DS2 to power so they're always high. CP to outpus Y0 and Y1 of
 * the decoder.
 *
 * For SR3, it's OE we connect to ground, SER we connect to PB4. Then, we connect both SRCLK and
 * RCLK to Y2 on the decoder.
 *
 * Decoder's inputs are PB0 and PB1.
 *
 * Connect the matrix' common anode to power through a 110 ohms resistance.
 *
 * The LED matrix is connected to the shift register's output as follow:
 *
 * F = Q0 (QA on SR3)
 * G = Q1
 * E = Q2
 * D = Q3
 * C = Q4
 * B = Q5
 * A = Q6
 * DP = Q7
 *
 * After that comes the phototransistor which is connected on ADC1 (PB3) with a 10K pull down
 * resistor.
 *
 * The program will show the last read value from ADC1 and refresh that value every second.
 *
 * Possible values range from 0 to 1023. If the value is higher than 999, the last 3 digit are
 * shown and the leftmost digit will have its dot shown.
 */

// CP outputs to our 3 shift registers go through a 2-4 decoder.
#define CP1 PinB1
#define CP2 PinB0
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
#define Seg7_Dash 0b00000010
#define Seg7_Dot 0b10000000

typedef enum {
    SR1_CP,
    SR2_CP,
    SR3_CP
} SR_CP_PIN;

static volatile bool refresh_needed = true;

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

    // SR3 is not a CD74AC164E (I only had 2 when assembling the prototype), but a SN74HC595N.
    // This SR has an extra RCLK step which "commits" registers set with SRCLK to outputs. Because
    // I don't have many pins handy, I simply soldered SRCLK to RCLK, but that means that outputs
    // are always one step late. For this SR, we need an extra SRCLK/RCLK toggle to have proper
    // outputs.
    if (pin == SR3_CP) {
        togglecp(pin);
    }
}

static void senddigits(unsigned int val)
{
    unsigned char digits[10] = {Seg7_0, Seg7_1, Seg7_2, Seg7_3, Seg7_4, Seg7_5, Seg7_6, Seg7_7, Seg7_8, Seg7_9};

    // We use bitwise NOT because our 7seg is a Common Anode, which means that *low* pins are
    // enabled.
    shiftsend(SR1_CP, ~digits[val % 10]);
    val /= 10;
    shiftsend(SR2_CP, ~digits[val % 10]);
    val /= 10;

    // We indicate overflow by enabling the dot on the 3rd digit
    if (val >= 10) {
        shiftsend(SR3_CP, ~(digits[val % 10] | Seg7_Dot));
    } else {
        shiftsend(SR3_CP, ~digits[val % 10]);
    }
}

ISR(TIMER0_COMPA_vect)
{
    refresh_needed = true;
}

static void setup()
{
    sei();

    pinoutputmode(CP1);
    pinoutputmode(CP2);
    pinoutputmode(DS);

    // All CP pins high. See togglecp()
    pinhigh(CP1);
    pinhigh(CP2);

    // Set ADC
    adc_select(ADC_PIN1); // ADC1
    adc_enable();

    // Set timer that controls refreshes
    set_timer1_target(F_CPU); // every 1 second
    set_timer1_mode(TIMER_MODE_INTERRUPT);
}

int main(void)
{
    setup();

    while (1) {
        if (refresh_needed) {
            refresh_needed = false;
            senddigits(adc_val());
        }
    }
}
