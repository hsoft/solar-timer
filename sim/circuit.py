import sys
sys.path.insert(0, "/home/vdupras/src/icemu")

from icemu.chip import Chip
from icemu.decoders import SN74HC138
from icemu.shiftregisters import CD74AC164
from icemu.seg7 import Segment7

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
        self.seg1 = Segment7()
        self.time = 0

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

        self.update()

    def update(self):
        self.dec.update()
        self.sr1.update()

    def delay(self, us):
        THRESHOLD = 1000 * 1000 # 1 sec
        self.time += us
        if self.time >= THRESHOLD:
            self.time -= THRESHOLD
            print(self.seg1)

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

def main():
    global circuit
    circuit = Circuit()

if __name__ == '__main__':
    main()
