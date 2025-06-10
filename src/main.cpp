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
	oled.log("init...");
	oled.log("..display");

  servo.begin();
	oled.log("..servo");



}

bool go = true;
int dr = 5;
int angle = 90;
int frame = 0;

void loop() {

	servo.set(0,angle);
  servo.set(1,angle);
	servo.set(2,angle);
	servo.set(3,angle);
	servo.set(4,angle);
	servo.set(5,angle);
	servo.set(6,angle);
	servo.set(7,angle);
	servo.set(8,angle);

	// angle += frame++;
	oled.log(angle);
  delay(2000);
  yield();

}



