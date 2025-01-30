from machine import Pin, I2C

class LM75(object):
    _TEMPERATURE_   = 0x00
    _CONFIGURATION_ = 0x01
    _THYST_REG_     = 0x02
    _TOS_REG_       = 0x03
    _PRODUCT_ID_    = 0x07
    _COMPARATOR_    = 0
    _INTERRUPT_     = 1
    LOW             = 0
    HIGH            = 1
    def __init__(self, scl: Pin, sda: Pin, address: hex = 0x48, os: Pin = None):
        self.i2c = I2C(1, scl = scl, sda = sda, freq = 400000)
        self.address = address
        if os is not None:
            self.os = os

    def read(self, address: hex, number: int):
        self.i2c.writeto(self.address, bytearray([address]))
        return self.i2c.readfrom(self.address, number)
    
    def write(self, address: hex, data):
        if isinstance(data, int):
            data = [data]
        self.i2c.writeto(self.address, bytearray([address] + data))
        
    @property 
    def getID(self):
        return self.read(self._PRODUCT_ID_, 1)[0]
    
    @property 
    def isConnected(self):
        devices = self.i2c.scan()
        for device in devices:
            if device == self.address:
                try:
                    if self.i2c.readfrom(self.address, 1):
                        return True
                except:
                    return False
            else:
                return False

    def getStatus(self):
        data = self.read(self._CONFIGURATION_, 1)[0]
        result = (
            (data & 0x80) >> 7, 
            (data & 0x40) >> 6, 
            (data & 0x20) >> 5, 
            (data & 0x10) >> 4, 
            (data & 0x08) >> 3, 
            (data & 0x04) >> 2, 
            (data & 0x02) >> 1, 
            (data & 0x01) >> 0
        )
        return result
    
    def turnUP(self):
        data = self.read(self._CONFIGURATION_, 1)[0]
        data &= ~0x01
        self.write(self._CONFIGURATION_, data)

    def shutdown(self):
        data = self.read(self._CONFIGURATION_, 1)[0]
        data |= 0x01
        self.write(self._CONFIGURATION_, data)
    
    @property 
    def Temperature(self):
        data = self.read(self._TEMPERATURE_, 2)
        temp = (data[0] << 8 | data[1])
        return temp / 256.0
    
if __name__ == "__main__":
    import time
    lm75 = LM75(scl = Pin(22), sda = Pin(21))
    while True:
        print(lm75.Temperature)
        time.sleep(1)