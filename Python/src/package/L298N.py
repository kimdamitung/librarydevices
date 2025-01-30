from  machine import Pin, PWM

class L298N(object):
    def __init__(self, in1: Pin, in2: Pin, in3: Pin , in4: Pin):
        self.in1 = in1
        self.in2 = in2
        self.in3 = in3
        self.in4 = in4
    def stop(self):
        self.in1.value(0)
        self.in2.value(0)
        self.in3.value(0)
        self.in4.value(0)
    def forward(self):
        self.in1.value(1)
        self.in2.value(0)
        self.in3.value(1)
        self.in4.value(0)
    def backward(self):
        self.in1.value(0)
        self.in2.value(1)
        self.in3.value(0)
        self.in4.value(1)
    def right(self):
        self.in1.value(1)
        self.in2.value(0)
        self.in3.value(0)
        self.in4.value(1)
    def left(self):
        self.in1.value(0)
        self.in2.value(1)
        self.in3.value(1)
        self.in4.value(0)