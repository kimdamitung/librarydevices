from machine import Pin, I2C
from time import sleep

class PCF8574P(object):
    _MAX_KEYS_  = 4
    _OUT_PUT_   = 3
    _IN_PUT_    = 2
    _HIGH_      = True
    _LOW_       = False

    class MODE(object):
        def __init__(self, pcf8574p: "PCF8574P", pin: int, mode: int):
            self.pcf8574p = pcf8574p
            self.pin = pin
            self.mode = mode
            if self.mode not in (PCF8574P._IN_PUT_, PCF8574P._OUT_PUT_):
                raise ValueError(f"[ERROR]  Mode must be 2 [INPUT] or 3 [OUTPUT]")
            self.pcf8574p.setting(self.pin, self.mode)
        
        @property
        def values(self):
            self.pcf8574p.update()
            return (self.pcf8574p.memory[0] >> self.pin) & 1
        
        @values.setter
        def values(self, value: bool):
            if self.mode == PCF8574P._OUT_PUT_:
                self.pcf8574p.output(self.pin, value)
            else:
                raise ValueError(f"[ERROR] Pin {self.pin} is in INPUT mode. Use .values to get input.") 

    def __init__(self, i2c: I2C, address: hex = 0x20):
        self.i2c = i2c
        self.address = address
        self.memory = bytearray(1)
        self.memory[0] = 0xFF
        self.i2c.writeto(self.address, self.memory)
    
    def searchAddress(self):
        _address = self.i2c.scan()
        if not _address:
            return []
        return [hex(_temp) for _temp in _address]
    
    def update(self):
        try:
            self.i2c.readfrom_into(self.address, self.memory)
        except OSError as e:
            print(f"[ERROR] {e}")
    
    def setting(self, pin: int, mode: int):
        if not 0 <= pin <= 7:
            raise ValueError(f"[ERROR]  Pin {pin} not in range 0-7")
        elif mode not in (self._OUT_PUT_, self._IN_PUT_):
            raise ValueError(f"[ERROR]  Mode {mode} must be 2 [INPUT] or 3 [OUTPUT]")
        if (self.memory[0] & (1 << pin)) == (mode << pin):
            return
        if mode == self._OUT_PUT_:
            self.memory[0] &= ~(1 << pin)
        elif mode == self._IN_PUT_:
            self.memory[0] |= (1 << pin)
        self.i2c.writeto(self.address, self.memory)
    
    def output(self, pin: int, value: bool):
        if not 0 <= pin <= 7:
            raise ValueError(f"[ERROR]  Pin {pin} not in range 0-7")
        elif not isinstance(value, bool):
            raise ValueError(f"[ERROR]  Value {value} must be 0 [LOW][False] and 1 [HIGH][True]")
        if value:
            self.memory[0] |= (1 << pin)
        else:
            self.memory[0] &= ~(1 << pin)
        self.i2c.writeto(self.address, self.memory)
    
    def PIN(self, pin: int, mode: int):
        return self.MODE(self, pin, mode)  # Correctly call the MODE constructor
    
    @property
    def P_8BIT(self):
        try:
            self.i2c.readfrom_into(self.address, self.memory)
            return self.memory[0]
        except OSError as e:
            print(f"[ERROR] {e}")
    
    @P_8BIT.setter
    def P_8BIT(self, value):
        try:
            if not 0 <= value <= 0xFF:
                raise ValueError(f"[ERROR] Value must be in range 0-255")
            self.memory[0] = value
            self.i2c.writeto(self.address, self.memory)
        except OSError as e:
            print(f"[ERROR] {e}")



def main():
    chip1 = PCF8574P(I2C(0, scl=Pin(22), sda=Pin(21)))
    chip1.P_8BIT = 0x00
    chip2 = PCF8574P(I2C(0, scl=Pin(22), sda=Pin(21)), 0x21)
    chip2.P_8BIT = 0x00
    print(chip1.searchAddress())
    led1 = [
        chip1.PIN(0, chip1._OUT_PUT_),
        chip1.PIN(1, chip1._OUT_PUT_),
        chip1.PIN(2, chip1._OUT_PUT_),
        chip1.PIN(3, chip1._OUT_PUT_),
        chip1.PIN(4, chip1._OUT_PUT_),
        chip1.PIN(5, chip1._OUT_PUT_),
        chip1.PIN(6, chip1._OUT_PUT_),
        chip1.PIN(7, chip1._OUT_PUT_)
    ]
    led2 = [
        chip2.PIN(0, chip2._OUT_PUT_),
        chip2.PIN(1, chip2._OUT_PUT_),
        chip2.PIN(2, chip2._OUT_PUT_),
        chip2.PIN(3, chip2._OUT_PUT_),
        chip2.PIN(4, chip2._OUT_PUT_),
        chip2.PIN(5, chip2._OUT_PUT_),
        chip2.PIN(6, chip2._OUT_PUT_),
        chip2.PIN(7, chip2._OUT_PUT_)
    ]
    while True:
        for i in range(16):
            if 0 <= i <= 7:
                led1[i].values = PCF8574P._HIGH_
            else:
                led2[i - 8].values = PCF8574P._HIGH_
            sleep(2)
        for i in range(16):
            if 0 <= i <= 7:
                led1[i].values = PCF8574P._LOW_
            else:
                led2[i - 8].values = PCF8574P._LOW_
            sleep(2)


if __name__ == "__main__":
    # main()
    pass