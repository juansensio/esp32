from machine import Pin
import neopixel

from src.rgb import flash_led
from src.buzzer import buzz

l = Pin(41, Pin.OUT)
l.on()

b = Pin(37, Pin.OUT)
buzz(b)

led = neopixel.NeoPixel(Pin(48), 1)

while True:
    try:
        flash_led(led)
    except KeyboardInterrupt:
        break
