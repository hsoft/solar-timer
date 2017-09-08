import random
from functools import partial
import os
import signal
import time

from icemu.chip import Chip
from icemu.decoders import SN74HC138
from icemu.shiftregisters import CD74AC164, SN74HC595
from icemu.seg7 import Segment7, combine_repr
from icemu.ui import UIScreen

class ATtiny45(Chip):
    OUTPUT_PINS = ['B0', 'B1', 'B2', 'B3', 'B4']

    def pin_from_int(self, val):
        PinB0 = 0b01000
        PinB1 = 0b01001
        PinB2 = 0b01010
        PinB3 = 0b01011
        PinB4 = 0b01100
        code = {
            PinB0: 'B0',
            PinB1: 'B1',
            PinB2: 'B2',
            PinB3: 'B3',
            PinB4: 'B4',
        }[val]
        return self.getpin(code)


class Circuit:
    def __init__(self):
        self.mcu = ATtiny45()
        self.dec = SN74HC138()
        self.sr1 = CD74AC164()
        self.sr2 = CD74AC164()
        self.sr3 = SN74HC595()
        self.seg1 = Segment7()
        self.seg2 = Segment7()
        self.seg3 = Segment7()
        self.timer1 = 0
        self.timer1_target = -1
        self.timer1_triggered = False
        self.luminosity_reading = 100

        self.dec.wirepins(self.mcu, ['A', 'B'], ['B1', 'B0'])

        self.sr1.pin_CP.wire_to(self.dec.pin_Y0)
        self.sr1.pin_DS1.wire_to(self.mcu.pin_B4)

        self.seg1.wirepins(
            self.sr1,
            ['F', 'G', 'E', 'D', 'C', 'B', 'A', 'DP'],
            ['Q0', 'Q1', 'Q2', 'Q3', 'Q4', 'Q5', 'Q6', 'Q7'],
        )

        self.sr2.pin_CP.wire_to(self.dec.pin_Y1)
        self.sr2.pin_DS1.wire_to(self.mcu.pin_B4)

        self.seg2.wirepins(
            self.sr2,
            ['F', 'G', 'E', 'D', 'C', 'B', 'A', 'DP'],
            ['Q0', 'Q1', 'Q2', 'Q3', 'Q4', 'Q5', 'Q6', 'Q7'],
        )

        self.sr3.wirepins(self.dec, ['SRCLK', 'RCLK'], ['Y2', 'Y2'])
        self.sr3.pin_SER.wire_to(self.mcu.pin_B4)

        self.seg3.wirepins(
            self.sr3,
            ['F', 'G', 'E', 'D', 'C', 'B', 'A', 'DP'],
            ['QA', 'QB', 'QC', 'QD', 'QE', 'QF', 'QG', 'QH'],
        )

    def delay(self, us):
        begin = time.time()
        end = begin + us / (1000 * 1000)
        if self.timer1_target:
            self.timer1 += us
        if self.timer1 >= self.timer1_target:
            self.timer1 -= self.timer1_target
            self.timer1_triggered = True
        while circuit and time.time() < end:
            if uiscreen:
                uiscreen.refresh()

    def timer1_check(self):
        if self.timer1_triggered:
            self.timer1_triggered = False
            return True
        return False

    def increase_light(self, amount):
        newval = self.luminosity_reading + amount
        newval = max(0, min(newval, 1023))
        self.luminosity_reading = newval

circuit = None
uiscreen = None

def pinset(pin_number, high):
    pin = circuit.mcu.pin_from_int(pin_number)
    pin.set(high)
    if uiscreen:
        uiscreen.refresh()

def pinishigh(pin_number):
    pin = circuit.mcu.pin_from_int(pin_number)
    return pin.ishigh()

def delay(us):
    circuit.delay(us)

def adcval():
    val = circuit.luminosity_reading + random.randint(-10, 10) # so that all digits get some change
    return max(0, min(val, 1023))

def set_timer1_target(ticks):
    circuit.timer1_target = ticks

def set_timer1_mode(mode):
    pass

def timer1_interrupt_check():
    return circuit.timer1_check()

def stop():
    global circuit, uiscreen
    uiscreen.stop()
    circuit = None
    uiscreen = None

def main():
    global circuit, uiscreen
    circuit = Circuit()
    uiscreen = UIScreen()
    uiscreen.add_element(
        "LED Matrix output:",
        partial(combine_repr, circuit.seg3, circuit.seg2, circuit.seg1)
    )
    uiscreen.add_action(
        'q', "Quit",
        partial(os.kill, os.getpid(), signal.SIGINT),
    )
    uiscreen.add_action(
        '+', "More light",
        partial(circuit.increase_light, 20),
    )
    uiscreen.add_action(
        '-', "Less light",
        partial(circuit.increase_light, -20),
    )
    uiscreen.refresh()

if __name__ == '__main__':
    main()
