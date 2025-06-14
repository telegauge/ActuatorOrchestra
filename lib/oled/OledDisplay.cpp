#include "OledDisplay.h"


const unsigned char logo [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x12, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7a, 0x5e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x56, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0a, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x56, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7a, 0x5e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x56, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x05, 0xa0, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xb0, 0xb8, 0x00, 0x00, 
	0x00, 0x00, 0x60, 0x3d, 0xbd, 0xff, 0x00, 0x00, 0x00, 0x00, 0x60, 0x65, 0xa7, 0xc7, 0x00, 0x00, 
	0x00, 0x00, 0x60, 0xc5, 0xa3, 0x9b, 0x00, 0x00, 0x00, 0x01, 0xf9, 0x85, 0xa1, 0xbd, 0x80, 0x00, 
	0x00, 0x01, 0x6f, 0x05, 0xa0, 0xbd, 0x80, 0x00, 0x00, 0x03, 0xff, 0x07, 0xe0, 0xd9, 0x00, 0x00, 
	0x00, 0x03, 0x7f, 0x0f, 0xf0, 0xc3, 0x00, 0x00, 0x00, 0x01, 0x1f, 0x0f, 0xf0, 0xff, 0x00, 0x00, 
	0x00, 0x01, 0xf9, 0x8f, 0xf1, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xf1, 0x8f, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x8f, 0xf1, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xf1, 0x07, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x87, 0xe1, 0x87, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0xa0, 0xc7, 0x80, 0x00, 
	0x00, 0x00, 0x06, 0x05, 0xa0, 0x6c, 0x80, 0x00, 0x00, 0x00, 0x04, 0x05, 0xa0, 0x25, 0x80, 0x00, 
	0x00, 0x00, 0x0c, 0x05, 0xa0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xf8, 0x3e, 0x00, 0x00, 
	0x00, 0x00, 0x0c, 0x1f, 0xf8, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1f, 0xf8, 0xe8, 0x00, 0x00, 
	0x00, 0x00, 0x08, 0x1f, 0xf9, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xfb, 0xd8, 0x00, 0x00, 
	0x00, 0x00, 0x0c, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x70, 0x00, 0x00, 
	0x00, 0x00, 0x02, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define LOG_X 50
#define LOG_SIZE 6
String logs[LOG_SIZE];

OledDisplay::OledDisplay() : display(64, 128, &Wire) {
}

void OledDisplay::begin() {
    initDisplay();
}

void OledDisplay::log(int value) {
	log(String(value));
}

void OledDisplay::log(String value) {
	for (int i = 0; i < LOG_SIZE-1; i++) {
		logs[i] = logs[i + 1];
	}
	logs[LOG_SIZE-1] = value;
	printLogs();
	display.display();
}

void OledDisplay::log(const char* text) {
	log(String(text));
}

bool OledDisplay::button(int button) {
	return digitalRead(button) != HIGH;
}

void OledDisplay::printLogs() {
	clear(LOG_X,0 ,128-LOG_X,64);

	display.setRotation(3);
	for (int i = 0; i < LOG_SIZE; i++) {
		print(logs[i].c_str(),LOG_X, i * 10);
	}
}

void OledDisplay::initDisplay() {
	display.begin(0x3C, true); // Address 0x3C default
	display.clearDisplay();
	display.setRotation(3);
	display.setTextSize(1);
	display.setTextColor(SH110X_WHITE);
	display.drawBitmap(-10,0, logo, 64,64, SH110X_WHITE);

	pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

	printLogs();
  display.display();
}

void OledDisplay::print(const char* text, int x, int y) {
	display.setCursor(x, y);
	display.print(text);
	display.display();
}

void OledDisplay::clear(int x, int y, int width, int height) {
	display.fillRect(x, y, width, height, SH110X_BLACK);
	// display.display();
}
