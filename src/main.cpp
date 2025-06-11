#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "../lib/oled/OledDisplay.h"
#include "../lib/servo/ServoController.h"

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);


WiFiClient wifi;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // Minimum pulse length count
#define SERVOMAX  600 // Maximum pulse length count

OledDisplay oled;
ServoController servo;

void setup() {
  Serial.begin(115200);

  oled.begin();
	oled.log("AO UKE...");
	oled.log("..display");

  servo.begin();
	oled.log("..servo");

	oled.log("..wifi");
  WiFi.setHostname("AO-Ukulele");
  WiFi.begin("Blaze", "shellycat");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
	oled.log(WiFi.localIP().toString()); // Convert IPAddress to String before logging



}

bool go = true;
int dr = 5;
int angle = 90;
int frame = 0;
int strum = 1;

void loop() {


	if (oled.button(BUTTON_A)) {
		if (go) {
			oled.log("pause");
		} else {
			oled.log("play");
		}
		go = !go;
	}

	if (oled.button(BUTTON_B)) {
		oled.log("Home");
		for (int i = 0; i<8; i++) {
			servo.set(i, 90);
		}
		go=false;
	}

	if (oled.button(BUTTON_C)) {
		int swing = 20;
		oled.log("Strum!");
		for (int i = 0; i<8; i++) {
			servo.set(i, 90-swing*strum);
		}
		delay(200);
		for (int i = 0; i<8; i++) {
			delay(50);
			servo.set(i, 90+swing*strum);
		}
		delay(200);
		angle=90;
		strum = -strum;
		go=false;
	}

	if (go) {
		for (int i = 0; i<8; i++) {
			servo.set(i, 70 + frame%(50+i));
		}
	}

	frame++;
	// oled.log(angle);
  delay(100);
  yield();

}



