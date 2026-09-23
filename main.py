from machine import Pin
import neopixel

from src.rgb import flash_led

led = neopixel.NeoPixel(Pin(48), 1)

while True:
    try:
        flash_led(led)
    except KeyboardInterrupt:
        break
