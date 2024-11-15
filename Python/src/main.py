import esp, esp32
esp.osdebug(0)
#import module micropython
import os
from machine import Pin, SPI
from package.ST7735 import ST7735, Font_7x10
from package import data
from neopixel import NeoPixel

turn_error = NeoPixel(Pin(48, Pin.OUT), 1)
turn_error[0] = (0, 0, 0)
turn_error.write()
spi0 = SPI(1, baudrate = 20000000, polarity = 0, phase = 0, sck = Pin(12), mosi = Pin(11), miso = None)
st7735 = ST7735(spi0, cs = Pin(15, Pin.OUT), dc = Pin(16, Pin.OUT), reset = Pin(17, Pin.OUT))
st7735.init()
st7735.setScreen(st7735._COLOR565(0xFF, 0xFF, 0xFF))
st7735.setImagesFullScreen(data)
while True:
    '''code'''
    pass
            