# ESP32

## Installation 

Installing micropython on the ESP32:

- install `esptool` with `uv add esptool`
- Erase the flash with `make erase-flash`
- download the firmware from `https://micropython.org/download/ESP32_GENERIC_S3/`
- Install micropython with `make write-flash`
- Install `mpremote` with `uv add mpremote`

## Usage

- Connect the ESP32 to the computer
- Run `make repl` to enter the REPL
- Run `make run` to run the code
- Run `make cp` to copy the code to the ESP32
- Run `make ls` to list the files on the ESP32

After using the ESP-IDF C++ app, switch the board back to MicroPython before
deploying these files. Disconnect the motor batteries, then run `make
erase-flash`, `make write-flash`, and `make deploy` from this directory. Erasing
flash removes the C++ app and any files stored on the board. Reconnect the motor
batteries and press RST to test the boot sequence and motor motion.

`make deploy` copies `src/`, `boot.py`, and `main.py` to the board. The Python
sequence uses motor IN1 on GPIO1, IN2 on GPIO42, LED on GPIO21, and buzzer on
GPIO41.

> To run code on boot, use copy a `boot.py` file to the ESP32. `boot.py` is intended for early initialization: filesystem setup, Wi-Fi configuration, low-level hardware setup, etc. It should normally finish quickly. Then MicroPython automatically executes `main.py`.

## Issues

- do not add an infite loop in `boot.py`, it runs before repl and will freeze the ESP32.
- use `uv run mpremote mount . run main.py` to avoid having to manually copy the code to the ESP32 everytime you make a change.
- seeing some inconsistencies with micropython (buzzer sounds while motor moves), trying to use C++ instead. C++ solves the issue.
- solved the issue with micropython

> The likely cause was PWM channel reuse in MicroPython. The buzzer used a PWM channel first. deinit() stopped its tone, but MicroPython’s ESP32 code can reuse that channel for the motor while the buzzer pin still has the old PWM routing. That fits why it buzzed only when motor IN1 ran forward. MicroPython’s ESP32 PWM code shows the channel being released and later reused.
The fix reconfigures GPIO41 as an ordinary low output after the tone, disconnecting that PWM routing. We also set it low again before the motor test.

## References

- https://docs.espressif.com/projects/esptool/en/latest/esp32s3/
- https://micropython.org/download/ESP32_GENERIC_S3/
