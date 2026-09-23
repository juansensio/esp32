# ESP-IDF C++ migration

This directory is a new ESP-IDF project. The MicroPython files in the parent
directory remain available as a reference. The app sets both motor inputs low,
then lights the LED and sounds the buzzer for one second during boot.



## Installation

Install ESP-IDF for macOS using Espressif's installation guide and open an ESP-IDF terminal. Check that `idf.py --version` works. [https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/macos-setup.html#online-installation-using-eim-cli](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/macos-setup.html#online-installation-using-eim-cli)

## Stage 1: safe GPIO startup

1. Remove the motor batteries for the first flash. In this directory, run:
  ```sh
   idf.py set-target esp32s3
   idf.py build
   idf.py -p /dev/cu.usbmodem5C930007091 flash monitor
  ```
2. Confirm the serial monitor prints:
  ```text
   Stage 1 ready: motor inputs GPIO1 and GPIO2 are low
  ```
3. Exit the monitor with Ctrl+T, then X (press them in sequence). Reconnect the motor supply and reset the board.
  The motor should remain still. If it twitches before the startup message,
   fit a 10 kOhm pull-down resistor from each driver input to ground, or keep
   the driver disabled until the ESP32 has configured the pins. Software cannot
   control GPIOs during the initial reset interval.

The serial port shown above was present when this project was prepared. Run
`ls /dev/cu.*` if it changes. Do not use `mpremote` to flash this C++ project.

## Stage 2: LED and buzzer at boot

The current app uses GPIO20 for the LED and GPIO21 for the buzzer, matching the
MicroPython `boot.py`. With the motor batteries removed, flash it again:

```sh
idf.py -p /dev/cu.usbmodem5C930007091 flash monitor
```

The LED should come on while the buzzer sounds at 2 kHz for one second, then
both should turn off. The monitor should print `Boot complete; LED and buzzer
off`. Exit with Ctrl+T, then X.

GPIO20 is also the ESP32-S3's native USB D+ pin. This wiring is intended for
the USB-to-serial port currently used for flashing. If you move to native USB,
move the LED to another free GPIO and update `kLed` in `main/main.cpp`. The LED
needs a series resistor, and the buzzer's other terminal must connect to ground.

Next stage: add motor PWM and direction changes after confirming the boot
sequence works without a restart.
