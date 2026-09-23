from machine import Pin, PWM
from time import sleep

PIN_IN1 = 42
PIN_IN2 = 2
SPEED = 0.5


def test():
    in1 = Pin(PIN_IN1, Pin.OUT, value=0)
    in2 = Pin(PIN_IN2, Pin.OUT, value=0)

    # forward
    print("hola")
    in1.value(1)
    in2.value(0)
    sleep(0.2)

    # reverse
    print("hola2")
    in1.value(0)
    in2.value(1)
    sleep(0.2)

    # stop / coast
    in1.value(0)
    in2.value(0)


def test2():
    in1 = PWM(Pin(PIN_IN1), freq=2000, duty_u16=0)
    in2 = PWM(Pin(PIN_IN2), freq=2000, duty_u16=0)

    def duty(speed):
        # speed: 0.0 -> 1.0
        speed = max(0.0, min(1.0, speed))
        return int(speed * 65535)

    def coast():
        in1.duty_u16(0)
        in2.duty_u16(0)

    def forward(speed):
        in2.duty_u16(0)
        in1.duty_u16(duty(speed))

    def reverse(speed):
        in1.duty_u16(0)
        in2.duty_u16(duty(speed))

    print("stop")
    coast()
    sleep(2)

    print("forward")
    forward(SPEED)
    sleep(1)
    coast()

    sleep(1)

    print("reverse")
    reverse(SPEED)
    sleep(1)
    coast()

    print("done")
