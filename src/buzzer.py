from machine import PWM
import time


def buzz(pin):
    buzzer = PWM(pin)
    buzzer.freq(2000)
    buzzer.duty_u16(32768)
    time.sleep(1)
    buzzer.duty_u16(0)
    buzzer.deinit()
