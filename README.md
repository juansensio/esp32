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

## References

- https://docs.espressif.com/projects/esptool/en/latest/esp32s3/
- https://micropython.org/download/ESP32_GENERIC_S3/
