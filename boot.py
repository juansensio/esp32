from machine import Pin

from src.buzzer import buzz

print("Booting...", end="")
l = Pin(21, Pin.OUT)
l.on()
b = Pin(36, Pin.OUT)
buzz(b)
l.off()
print("Done.")
