# Actuator Orchestra

Actuator Orchestra is a project wherein a device is
  strapped to an instrument (like a Ukulele) and
	a series of servos or solonoids "play" the instrument.

In the short term, there's this Ukulele.

In the long term, multiple instruments should be able
  to work together to make a band.
  *Dueling Banjos* is a reasonable goal.  
  *Tom Sawyer* is more of a challenge.  
  *Bohemian Rhapsody* is ... well, that's unlikely.

## Features

- Controls up to 8 servos (so far) via I2C PWM driver
- Interactive OLED display with button controls
- WiFi connectivity to receive commands from a web app (tbd)
- Modular code with custom libraries for display and servo control

## Hardware Requirements

- A Ukulele
- [Adafruit Feather ESP32 V2](https://www.adafruit.com/product/5400)
- [Adafruit SH1107 128x64 OLED FeatherWing](https://www.adafruit.com/product/4650)
- [Adafruit 16-Channel 12-bit PWM/Servo Driver - I2C interface (PCA9685)](https://www.adafruit.com/product/815)
- Up to 8 servos (for actuation)
- Pushbuttons (or, integrated on OLED FeatherWing)

## Software Requirements

- [PlatformIO](https://platformio.org/) (for building and uploading firmware)
- Arduino framework

## Installation
1. Clone this repository:
   ```sh
   git clone <repo-url>
   cd ActuatorOrchestra
   ```
2. Install PlatformIO (if not already installed):
   https://platformio.org/install
3. Open the project in [VSCode](https://code.visualstudio.com/) with the PlatformIO extension.

## Building and Uploading
1. Connect your Adafruit Feather ESP32 V2 via USB.
2. Build and upload the firmware using PlatformIO:
   ```sh
   pio run --target upload
   ```
3. Open the serial monitor to view logs:
   ```sh
   pio device monitor
   ```

## Usage

As of June, 2025

- **Button A**: Toggle play/pause of the servo sequence
- **Button B**: Home all servos (set to neutral position)
- **Button C**: Perform a strum action with all servos
- The OLED displays logs and status messages

## Project Structure

- `src/` - Main application code
- `lib/` - The Library
  - `servo/` - Servo control logic
  - `oled/` - Display tools (and buttons, oddly)
- `include/` - Project headers
- `test/` - (Optional) Unit tests

## Dependencies

Defined in `platformio.ini`:

- Adafruit GFX Library (for the OLED)
- Adafruit SH110X (for the OLED)
- Adafruit PWM Servo Driver Library (for the servos)

