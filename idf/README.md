# ESP-IDF C++ migration

This directory is a new ESP-IDF project. The MicroPython files in the parent
directory remain available as a reference. The app sets both motor inputs low,
then lights the LED and sounds the buzzer for one second during boot. After a
pause, it runs a short forward and reverse motor test.



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
   Motor inputs GPIO1 and GPIO42 are low
  ```
3. Exit the monitor with Ctrl+T, then X (press them in sequence). Reconnect the motor supply and reset the board.
  The motor should remain still. If it twitches before the startup message,
   fit a 10 kOhm pull-down resistor from each driver input to ground, or keep
   the driver disabled until the ESP32 has configured the pins. Software cannot
   control GPIOs during the initial reset interval.

The serial port shown above was present when this project was prepared. Run
`ls /dev/cu.*` if it changes. Do not use `mpremote` to flash this C++ project.

## Stage 2: LED and buzzer at boot

The current app uses GPIO21 for the LED. Move the buzzer's signal wire from
GPIO36 to GPIO41 before testing: GPIO36 is used by octal PSRAM on N16R8 boards.
GPIO41 is on the accessible row, five pins toward the antenna from GPIO36
(passing GPIO37, GPIO38, GPIO39, and GPIO40).
With the motor batteries removed, flash it again:

```sh
idf.py -p /dev/cu.usbmodem5C930007091 flash monitor
```

The LED should come on while the buzzer sounds at 2 kHz for one second, then
both should turn off. The monitor should print `Boot complete; LED and buzzer
off`. Exit with Ctrl+T, then X.

The LED needs a series resistor, and the buzzer's other terminal must connect
to ground. Leave GPIO36 disconnected from the buzzer.

## Stage 3: motor test

The motor driver inputs are GPIO1 (IN1) and GPIO42 (IN2). The test uses 2 kHz
PWM at 50% duty. After the LED and buzzer turn off, the motor stays stopped for
two seconds, runs forward for one second, stops for one second, runs in reverse
for one second, and stops.

Flash with the motor batteries removed. Keep the monitor open, reconnect the
motor batteries, then press the board's RST button to run the full sequence:

```sh
idf.py -p /dev/cu.usbmodem5C930007091 flash monitor
```

The monitor should print `Motor forward at 50%`, `Motor reverse at 50%`, and
`Motor test complete; motor stopped` once per reset. If the first direction is
opposite to what you expect, swap the motor's two output wires or the IN1/IN2
assignments. If the LED and buzzer repeat during motor motion, check for a
power-supply reset before continuing. Exit the monitor with Ctrl+T, then X.
