from machine import Pin

# Hold both driver inputs low before starting the LED or buzzer.
motor_in1 = Pin(1, Pin.OUT, value=0)
motor_in2 = Pin(42, Pin.OUT, value=0)

from src.buzzer import buzz

led = Pin(21, Pin.OUT, value=0)
print("Motor inputs GPIO1 and GPIO42 are low")
led.on()
print("LED on; buzzer on at 2 kHz")
buzz(Pin(41, Pin.OUT))
led.off()
print("Boot complete; LED and buzzer off")
