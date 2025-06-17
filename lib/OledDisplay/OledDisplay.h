#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_SH110X.h>
#include <WiFi.h>

// OLED FeatherWing buttons map to different pins depending on board:
#if defined(ESP8266)
#define BUTTON_A 0
#define BUTTON_B 16
#define BUTTON_C 2
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3_NOPSRAM)
#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14
#elif defined(ARDUINO_STM32_FEATHER)
#define BUTTON_A PA15
#define BUTTON_B PC7
#define BUTTON_C PC5
#elif defined(TEENSYDUINO)
#define BUTTON_A 4
#define BUTTON_B 3
#define BUTTON_C 8
#elif defined(ARDUINO_NRF52832_FEATHER)
#define BUTTON_A 31
#define BUTTON_B 30
#define BUTTON_C 27
#else // 32u4, M0, M4, nrf52840, esp32-s2, esp32-s3 and 328p
#define BUTTON_A 9
#define BUTTON_B 6
#define BUTTON_C 5
#endif

class OledDisplay
{
public:
	OledDisplay();
	void begin();
	void clear(int x, int y, int width, int height);

	void print(const char *text, int x, int y);
	void line(int x1, int y1, int x2, int y2);
	// void line(float x1, float y1, float x2, float y2);

	void grid(int string, int fret, bool state);

	void log(const char *text);
	void log(int value);
	void log(String value);

	void printLogs();
	bool button(int button);

private:
	Adafruit_SH1107 display;
	void initDisplay();
	void initGrid();
};

#endif // OLED_DISPLAY_H