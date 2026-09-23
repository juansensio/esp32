from machine import Pin
import neopixel

from src.rgb import flash_led
from src.buzzer import buzz

l = Pin(37, Pin.OUT)
l.on()
buzz(l)

led = neopixel.NeoPixel(Pin(48), 1)

while True:
    try:
        flash_led(led)
    except KeyboardInterrupt:
        break
