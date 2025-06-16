#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <FS.h>
#include <SPIFFS.h>

#include "../lib/ConfigLoader/ConfigLoader.h"
#include "../lib/ActuatorFactory/ActuatorFactory.h"
#include "../lib/TimingEngine/TimingEngine.h"
#include "../lib/OledDisplay/OledDisplay.h"
#include "../lib/ServoStrummer/ServoStrummer.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
std::vector<ServoStrummer *> strummers;
TimingEngine timingEngine;
OledDisplay oled;

const int STRUM_SWING_DEGREES = 30;
const int STRUM_DURATION_MS = 150;

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

	strummers = ActuatorFactory::createStrummers(config.actuators, &pwm);
	oled.log("Strummers...");
	for (auto *s : strummers)
	{
		oled.log(s->name().c_str());
		s->begin();
		s->home();
	}
	oled.log("Ready");
}

void loop()
{
	int duration = 200;
	// oled.log(String("Main ") + strummers[0]->name().c_str());
	strummers[0]->strum(duration);
	strummers[1]->strum(duration);
	strummers[2]->strum(duration);
	strummers[3]->strum(duration);
	// delay(1000); // Slow down log spam for visibility
}