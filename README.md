# Actuator Orchestra

Actuator Orchestra is a modular system for robotic musical instruments. Each instrument (e.g., Ukulele, Drum, etc.) is controlled by its own board (e.g., Adafruit Feather ESP32), and can be configured and coordinated to play music together.

---

## New Architecture (2024+)

- **Multi-instrument support:** Each instrument is defined by a configuration file (see `config/`).
- **Modular actuators:** Each actuator (servo, solenoid, etc.) is managed by its own class, instantiated based on config.
- **Timing engine:** Each board can play notes at precise times, and synchronize with others for ensemble performance.
- **Song files (future):** Songs will be defined in files and played by the system.
- **Old code:** Previous implementation is preserved in the `old/` directory.

### Directory Structure

- `src/` - Main application code (new architecture)
- `lib/` - Custom libraries (actuators, timing, etc.)
- `include/` - Project headers
- `config/` - Instrument configuration files (JSON)
- `test/` - Unit tests
- `old/` - Previous codebase (for reference)

---

## Features

- Controls multiple actuators (servos, solenoids, etc.) via I2C PWM or GPIO
- Configurable instruments (number of strings, actuator types, etc.)
- Interactive OLED display with button controls (optional)
- WiFi connectivity for synchronization (future)
- Modular code for easy expansion

## Hardware Requirements

- Instrument (e.g., Ukulele)
- [Adafruit Feather ESP32 V2](https://www.adafruit.com/product/5400)
- [Adafruit SH1107 128x64 OLED FeatherWing](https://www.adafruit.com/product/4650) (optional)
- [Adafruit 16-Channel 12-bit PWM/Servo Driver - I2C interface (PCA9685)](https://www.adafruit.com/product/815)
- Actuators (servos, solenoids, etc.)
- Pushbuttons (optional)

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

- Configure your instrument in `config/` (see example `ukulele.json`)
- Upload firmware to your board
- (Future) Play a song file to coordinate multiple instruments

## Project Structure

- `src/` - Main application code
- `lib/` - Custom libraries (actuators, timing, etc.)
- `include/` - Project headers
- `config/` - Instrument configuration files
- `test/` - Unit tests
- `old/` - Previous codebase

## Dependencies

Defined in `platformio.ini`:

- Adafruit GFX Library (for the OLED)
- Adafruit SH110X (for the OLED)
- Adafruit PWM Servo Driver Library (for the servos)

## Connect

![Bluesky](https://img.shields.io/badge/-Bluesky-3686f7?style=flat&logo=icloud&logoColor=white)
[@acutatororchestra.bsky.social](https://bsky.app/profile/acutatororchestra.bsky.social)
