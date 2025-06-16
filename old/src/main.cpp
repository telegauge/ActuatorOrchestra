#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <Adafruit_PWMServoDriver.h>

#include "../lib/oled/OledDisplay.h"
#include "../lib/servo/ServoController.h"

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

WiFiClient wifi;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 150 // Minimum pulse length count
#define SERVOMAX 600 // Maximum pulse length count

#define MODE_STRUM 0
#define MODE_TUNING 1

OledDisplay oled;
ServoController servo;

void setup()
{
	Serial.begin(115200);

	oled.begin();
	oled.log("AO UKE...");
	oled.log("..display");

	servo.begin();
	oled.log("..servo");

	oled.log("..wifi");
	WiFi.setHostname("AO-Ukulele");
	WiFi.begin("Blaze", "shellycat");
	while (WiFi.status() != WL_CONNECTED)
	{
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
int swing = 15;
int target_servo = -1;

#define MODE_COUNT 2
int mode = 0;
const char *mode_names[] = {"Strum", "Tuning"};

void loop()
{
	Serial.println(mode_names[mode]);

	if (oled.button(BUTTON_C))
	{
		if (go)
		{
			oled.log("PAUSE");
		}
		else
		{
			oled.log("PLAY");
		}
		delay(200); // give the button a chance to be releasedwa
		go = !go;
	}

	if (oled.button(BUTTON_B))
	{
		target_servo = target_servo + 1;
		if (target_servo > 8)
		{
			target_servo = -1;
			oled.log("ALL");
		}
		else
		{
			oled.log(target_servo);
		}
		delay(200);
	}

	if (oled.button(BUTTON_A))
	{
		mode = (mode + 1) % MODE_COUNT;
		oled.log(mode_names[mode]);
		delay(200);
	}

	if (go)
	{
		switch (mode)
		{
		case MODE_STRUM:
			for (int i = 0; i < 8; i++)
			{
				if (target_servo == -1 || i == target_servo)
				{
					servo.set(i, 90 - swing * strum);
					delay(50);
				}
			}
			angle = 90;
			strum = -strum;
			break;
		case MODE_TUNING:
			for (int i = 0; i < 8; i++)
			{
				if (target_servo == -1 || i == target_servo)
				{
					servo.set(i, 90 - swing);
				}
			}
			delay(100);
			for (int i = 0; i < 8; i++)
			{
				if (target_servo == -1 || i == target_servo)
				{
					servo.set(i, 90 + swing);
				}
			}

			break;
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (target_servo == -1 || i == target_servo)
			{
				servo.set(i, 90);
			}
		}
	}

	// oled.log(angle);
	frame++;
	if (frame % 500 == 0)
	{
		oled.log(" ");
	}

	delay(100);
	yield();
}
