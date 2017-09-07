import sys
sys.path.insert(0, "/home/vdupras/src/icemu")

from icemu.chip import Chip
from icemu.pin import OutputPin
from icemu.decoders import SN74HC138

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

        self.dec.pin_A.wire_to(self.mcu.pin_B1)
        self.dec.pin_B.wire_to(self.mcu.pin_B0)
        self.dec.update()

    def update(self):
        self.dec.update()
        print(self.dec.pin_Y0)
        print(self.dec.pin_Y1)
        print(self.dec.pin_Y2)
        print(self.dec.pin_Y3)

circuit = None

def pinset(pin_number, high):
    pin = circuit.mcu.pin_from_int(pin_number)
    pin.set(high)
    print('pinset', pin)
    circuit.update()

def pinishigh(pin_number):
    pin = circuit.mcu.pin_from_int(pin_number)
    return pin.ishigh()

def main():
    global circuit
    circuit = Circuit()

if __name__ == '__main__':
    main()
