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
	delay(1000); // wait for the servos to home, and the terminal to catch up
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
	int duration = 100;
	int pause = 300;
	if (oled.button(BUTTON_A))
	{
		paused = !paused;
		oled.log(paused ? "Paused" : "Playing");
		delay(300); // debounce
		ukulele->home();
	}
	if (!paused)
	{
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, false, false, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, false, false, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, false, true, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, false, true, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, true, false, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, true, false, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, true, true, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {false, true, true, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, false, false, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, false, false, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, false, true, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, false, true, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, true, false, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, true, true, false});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->fret(0, {true, true, true, true});
		// delay(pause);
		// ukulele->strum(duration);
		// ukulele->home();
		Serial.println(" ");
	}
	// delay(500);
}