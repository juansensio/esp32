# from machine import Pin
# from utime import sleep

# pin = Pin("LED", Pin.OUT)

# print("LED starts flashing...")
# while True:
#     try:
#         pin.toggle()
#         sleep(1)
#     except KeyboardInterrupt:
#         break
# pin.off()
# print("Finished.")


from machine import Pin
import neopixel
import time

led = neopixel.NeoPixel(Pin(48), 1)

while True:
    try:
        print("LED starts flashing...")
        led[0] = (0, 0, 0)
        led.write()
        time.sleep(0.5)
        led[0] = (255, 0, 0)
        led.write()
        time.sleep(0.5)
        led[0] = (0, 255, 0)
        led.write()
        time.sleep(0.5)
        led[0] = (0, 0, 255)
        led.write()
        time.sleep(0.5)
    except KeyboardInterrupt:
        break
