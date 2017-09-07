import random

from icemu.chip import Chip
from icemu.decoders import SN74HC138
from icemu.shiftregisters import CD74AC164, SN74HC595
from icemu.seg7 import Segment7, combine_repr

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
        self.time = 0
        self.timer1 = 0
        self.timer1_target = -1
        self.timer1_triggered = False

        self.dec.pin_A.wire_to(self.mcu.pin_B1)
        self.dec.pin_B.wire_to(self.mcu.pin_B0)

        self.sr1.pin_CP.wire_to(self.dec.pin_Y0)
        self.sr1.pin_DS1.wire_to(self.mcu.pin_B4)

        self.seg1.pin_F.wire_to(self.sr1.pin_Q0)
        self.seg1.pin_G.wire_to(self.sr1.pin_Q1)
        self.seg1.pin_E.wire_to(self.sr1.pin_Q2)
        self.seg1.pin_D.wire_to(self.sr1.pin_Q3)
        self.seg1.pin_C.wire_to(self.sr1.pin_Q4)
        self.seg1.pin_B.wire_to(self.sr1.pin_Q5)
        self.seg1.pin_A.wire_to(self.sr1.pin_Q6)
        self.seg1.pin_DP.wire_to(self.sr1.pin_Q7)

        self.sr2.pin_CP.wire_to(self.dec.pin_Y1)
        self.sr2.pin_DS1.wire_to(self.mcu.pin_B4)

        self.seg2.pin_F.wire_to(self.sr2.pin_Q0)
        self.seg2.pin_G.wire_to(self.sr2.pin_Q1)
        self.seg2.pin_E.wire_to(self.sr2.pin_Q2)
        self.seg2.pin_D.wire_to(self.sr2.pin_Q3)
        self.seg2.pin_C.wire_to(self.sr2.pin_Q4)
        self.seg2.pin_B.wire_to(self.sr2.pin_Q5)
        self.seg2.pin_A.wire_to(self.sr2.pin_Q6)
        self.seg2.pin_DP.wire_to(self.sr2.pin_Q7)

        self.sr3.pin_SRCLK.wire_to(self.dec.pin_Y2)
        self.sr3.pin_RCLK.wire_to(self.dec.pin_Y2)
        self.sr3.pin_SER.wire_to(self.mcu.pin_B4)

        self.seg3.pin_F.wire_to(self.sr3.pin_QA)
        self.seg3.pin_G.wire_to(self.sr3.pin_QB)
        self.seg3.pin_E.wire_to(self.sr3.pin_QC)
        self.seg3.pin_D.wire_to(self.sr3.pin_QD)
        self.seg3.pin_C.wire_to(self.sr3.pin_QE)
        self.seg3.pin_B.wire_to(self.sr3.pin_QF)
        self.seg3.pin_A.wire_to(self.sr3.pin_QG)
        self.seg3.pin_DP.wire_to(self.sr3.pin_QH)

        self.update()

    def update(self):
        self.dec.update()
        self.sr1.update()
        self.sr2.update()
        self.sr3.update()

    def delay(self, us):
        THRESHOLD = 1000 * 1000 # 1 sec
        self.time += us
        if self.timer1_target:
            self.timer1 += us
        if self.time >= THRESHOLD:
            self.time -= THRESHOLD
            print(combine_repr(self.seg3, self.seg2, self.seg1))
        if self.timer1 >= self.timer1_target:
            self.timer1 -= self.timer1_target
            self.timer1_triggered = True

    def timer1_check(self):
        if self.timer1_triggered:
            self.timer1_triggered = False
            return True
        return False


circuit = None

def pinset(pin_number, high):
    pin = circuit.mcu.pin_from_int(pin_number)
    pin.set(high)
    circuit.update()

def pinishigh(pin_number):
    pin = circuit.mcu.pin_from_int(pin_number)
    return pin.ishigh()

def delay(us):
    circuit.delay(us)

def adcval():
    result = random.randint(0, 1023)
    return result

def set_timer1_target(ticks):
    circuit.timer1_target = ticks

def set_timer1_mode(mode):
    pass

def timer1_interrupt_check():
    return circuit.timer1_check()

def main():
    global circuit
    circuit = Circuit()

if __name__ == '__main__':
    main()
