
from time import sleep

class LCD(object):
    _20_04          = "20x04"
    _16_02          = "16x02"
    _CLEAR_         = 0x01
    _HOME_          = 0x02
    _MODE_          = 0x04
    _CONTROL_       = 0x08
    _CURSOR_        = 0x10
    _FUNCTION_      = 0x20
    _CGRAM_         = 0x40
    _DDRAM_         = 0x80
    _ENTRY_         = [0x02, 0x00] #entry mode: entry left, entry shift
    _ON_            = [0x04, 0x02, 0x01] #display control: display, cursor, blink
    _LINE_          = 0x08
    _DOTS_          = 0x00
    _LIGHT_         = 0x08
    _ENABLE_        = 0x04
    # left center right
    # 0x4  0x2    0x1
    # 3 bit 
    # 0b100 0b010 0b001
    _MARGIN_LEFT    = 0x4
    _MARGIN_CENTER  = 0x2
    _MARGIN_RIGHT   = 0x1
    def __init__(self, bus: int, scl: int, sda: int, data_type: str, address: hex = 0x27):
        self.platform: str = None
        import sys
        if sys.implementation.name == "micropython":
            self.platform = "micropython"
            from machine import I2C, Pin
            self.i2c = I2C(bus, scl=Pin(scl), sda=Pin(sda), freq=400000)
            print("Đang chạy trên MicroPython (ESP32)")
        elif sys.platform.startswith("linux"):
            self.platform = "linux"
            from smbus2 import SMBus
            self.i2c = SMBus(bus)
            print("Đang chạy trên Linux")
        else:
            print("Không xác định được nền tảng")
        self.data_type = data_type
        if self.data_type == "20x04":
            self.columns = 20
            self.rows = 4
        elif self.data_type == "16x02":
            self.columns = 16
            self.rows = 2
        self.address = address
        self.light = self._LIGHT_
        self.control = self._ON_[0]
        self.init()
    def init(self):
        sleep(5 / 1000)
        self.send(0x03, 0)
        sleep(5 / 1000)
        self.send(0x03, 0)
        sleep(15 / 1000000)
        self.send(0x03, 0)
        self.send(0x02, 0)
        self.send(self._FUNCTION_ | self._LINE_ | self._DOTS_, 0)
        self.send(self._CONTROL_ | self.control, 0)
        self.send(self._CLEAR_, 0)
        sleep(2 / 1000)
        self.send(self._MODE_ | self._ENTRY_[0], 0)
    def write(self, data):
        if self.platform == "micropython":
            self.i2c.writeto(self.address, bytearray([data]))
        elif self.platform == "linux":
            self.i2c.write_byte(self.address, data)
    def pulse_enable(self, data):
        self.write(data | self._ENABLE_)
        sleep(1 / 1000000)
        self.write(data & ~self._ENABLE_)
        sleep(5 / 1000000)
    def send(self, data, mode):
        buff = [(mode | (data & 0xF0) | self.light), (mode | ((data << 4) & 0xF0) | self.light)]
        self.write(buff[0])
        self.pulse_enable(buff[0])
        self.write(buff[1])
        self.pulse_enable(buff[1])
    def clear(self):
        self.send(self._CLEAR_, 0)
        sleep(2 / 1000)
    def setCursor(self, col, row):
        buff = [0x00, 0x40, 0x14, 0x54]
        if row > self.rows:
            row = self.rows - 1
        self.send(self._DDRAM_ | (col + buff[row]), 0)
    def printf(self, string: str, line: int, margin: hex = 0x4):
        if line not in  range(1, 5):
            raise ValueError("the line you entered is longer than the number of rows contained in the LCD")
        if len(string) == self.columns:
            self.setCursor(0, line - 1)
            for char in string:
                self.send(ord(char), 0x01)
        elif len(string) < self.columns:
            if margin == self._MARGIN_CENTER:
                self.setCursor((self.columns - len(string)) // 2, line - 1)
                for char in string:
                    self.send(ord(char), 0x01)
            elif margin == self._MARGIN_RIGHT:
                self.setCursor((self.columns - len(string)), line - 1)
                for char in string:
                    self.send(ord(char), 0x01)
            else:
                self.setCursor(0, line - 1)
                for char in string:
                    self.send(ord(char), 0x01)
        else:
            raise ValueError("the string you entered is longer than the number of columns contained in the LCD")
    @property
    def backlight(self):
        return self.light
    @backlight.setter
    def backlight(self, values):
        if values:
            if self.light != 0x08:
                self.light = 0x08
            self.write(self.light)
        else:
            if self.light != 0x00:
                self.light = 0x00
            self.write(self.light)

if __name__ == "__main__":
    lcd = LCD(0, 22, 21, LCD._16_02)
    lcd.clear()
    lcd.printf("Hello ", 1, LCD._MARGIN_CENTER)