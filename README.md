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

> To run code on boot, use copy a `boot.py` file to the ESP32. `boot.py` is intended for early initialization: filesystem setup, Wi-Fi configuration, low-level hardware setup, etc. It should normally finish quickly. Then MicroPython automatically executes `main.py`.

## Issues

- do not add an infite loop in `boot.py`, it runs before repl and will freeze the ESP32.
- use `uv run mpremote mount . run main.py` to avoid having to manually copy the code to the ESP32 everytime you make a change.

## References

- https://docs.espressif.com/projects/esptool/en/latest/esp32s3/
- https://micropython.org/download/ESP32_GENERIC_S3/
