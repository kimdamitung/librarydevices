from machine import ADC, Pin

class MQ135(object):
    _RESISTER_LOAD_ = 10.0
    _RESISTER_ZERO_ = 76.63
    _PARA_          = 116.6020682
    _PARB_          = 2.769034857
    CORA            = 0.00035
    CORB            = 0.02718
    CORC            = 1.39538
    CORD            = 0.0018
    CORE            = -0.003333333
    CORF            = -0.001923077
    CORG            = 1.130128205
    ATMOCO2         = 415.58
    
    def __init__(self, pin: Pin):
        self.adc = ADC(pin)
        self.adc.atten(ADC.ATTN_11DB)
        self.adc.width(ADC.WIDTH_12BIT)
        self.temp = None
        self.humi = None
    
    @property
    def temperature(self):
        return self.temp
    @temperature.setter
    def temperature(self, value):
        self.temp = value

    @property
    def humidity(self):
        return self.humi
    @humidity.setter
    def humidity(self, value):
        self.humi = value

    def correction(self, temp, humi):
        if temp < 20:
            return (self.CORA * (temp ** 2)- self.CORB * temp + self.CORC - (humi - 33.) * self.CORD)
        else:
            return (self.CORE * temp + self.CORF * humi + self.CORG)

    @property
    def resistance(self):
        values = self.adc.read_u16()
        voltage = (values * (3.3 / 65535))
        if voltage == 0:
            raise ValueError("ADC reading is zero, cannot calculate resistance.")
        if self.temp is None or self.humi is None:
            return ((3.3 / voltage) - 1) * self._RESISTER_LOAD_
        else:
            return (((3.3 / voltage) - 1) * self._RESISTER_LOAD_) / self.correction(self.temp, self.humi)
    
    @property
    def PPM(self):
        return (self._PARA_ * ((self.resistance / self._RESISTER_ZERO_) ** -self._PARB_))
    
    @property
    def RZERO(self):
        return (self.resistance * ((self._PARA_ / self.PPM) ** (1 / self._PARB_)))
    
if __name__ == "__main__":
    import time
    mq135 = MQ135(pin = Pin(36))
    while True:
        print(mq135.PPM)
        time.sleep(1)