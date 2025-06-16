#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <FS.h>
#include <SPIFFS.h>

#include "../lib/ConfigLoader/ConfigLoader.h"
#include "../lib/ActuatorFactory/ActuatorFactory.h"
#include "../lib/TimingEngine/TimingEngine.h"
#include "../lib/OledDisplay/OledDisplay.h"
#include "../lib/Ukulele/Ukulele.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Ukulele *ukulele = nullptr;
TimingEngine timingEngine;
OledDisplay oled;

const int STRUM_SWING_DEGREES = 30;
const int STRUM_DURATION_MS = 150;
bool paused = false;

void setup()
{
	Serial.begin(115200);
	oled.begin();
	oled.log("Booting...");
	if (!SPIFFS.begin(true))
	{
		oled.log("SPIFFS Fail");
		Serial.println("SPIFFS Mount Failed");
		return;
	}
	oled.log("SPIFFS OK");
	pwm.begin();
	oled.log("PWM OK");

	InstrumentConfig config;
	if (!ConfigLoader::loadConfig("/ukulele.json", config))
	{
		// TO upload: pio run -t upload
		oled.log("Config FAIL?");
		Serial.println("Failed to load config");
		return;
	}
	oled.log("Config OK");
	oled.log(config.instrument.c_str());

	ukulele = new Ukulele(config, &pwm);
	ukulele->begin();
	ukulele->home();
	oled.log("Ready");
}

void loop()
{
	int duration = 1000;
	int pause = 20;
	if (oled.button(BUTTON_A))
	{
		paused = !paused;
		oled.log(paused ? "Paused" : "Playing");
		delay(300); // debounce
		ukulele->home();
	}
	if (!paused)
	{
		ukulele->strum(duration);
		delay(250);
		ukulele->pluck(0, duration);
	}
	delay(500);
}