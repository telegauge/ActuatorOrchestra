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

bool pressed[] = {false, false, false, false};

void setup()
{
	Serial.begin(115200);
	delay(500); // wait for the servos to home, and the terminal to catch up
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

	ukulele = new Ukulele(config, &pwm, &oled);
	ukulele->begin();
	ukulele->home();

	oled.log("Ready");
}

bool Bools[][4] = {
		{false, false, false, false}, // 0
		{false, false, false, true},	// 1
		{false, false, true, false},	// 2
		{false, false, true, true},		// 3
		{false, true, false, false},	// 4
		{false, true, false, true},		// 5
		{false, true, true, false},		// 6
		{false, true, true, true},		// 7
		{true, false, false, false},	// 8
		{true, false, false, true},		// 9
		{true, false, true, false},		// 10
		{true, false, true, true},		// 11
		{true, true, false, false},		// 12
		{true, true, false, true},		// 13
		{true, true, true, false},		// 14
		{true, true, true, true}			// 15
};

void loop()
{
	Serial.println("Loop");
	// oled.log("Loop");
	int ms = millis();
	int duration = 1000;
	int pause = 1000;
	if (oled.button(BUTTON_A))
	{
		Serial.printf("Button A %d\n", pressed[BUTTON_A]);
		if (!pressed[BUTTON_A])
		{
			pressed[BUTTON_A] = true;
			paused = !paused;
			oled.log(paused ? "Paused" : "Playing");
			delay(300); // debounce
			ukulele->home();
		}
		else
		{
			pressed[BUTTON_A] = false;
		}
	}

	if (oled.button(BUTTON_B))
	{
		if (!pressed[BUTTON_B])
		{
			pressed[BUTTON_B] = true;
			Serial.printf("Strumming %d\n", ms);
			for (int i = 0; i < 30; i++)
			{
				ukulele->command("strum", ms + 100 * i, {});
			}
			ukulele->command("home", ms + 100 + 150 * 30, {});
			oled.log("Strum!");
		}
	}
	else
	{
		pressed[BUTTON_B] = false;
	}

	if (!paused)
	{
		// ukulele->test();
		// ukulele->loop(ms);
		// // ukulele->strum(duration);

		for (int i = 0; i < 16; i++)
		{
			// char buf[32];
			// snprintf(buf, sizeof(buf), "Pos %d", i);
			// oled.log(buf);
			// Serial.printf("Pos %d\n", i);
			std::vector<bool> fret_positions(Bools[i], Bools[i] + 4);
			ukulele->fret(0, fret_positions);
			delay(pause);
			ukulele->strum(100);
			delay(pause);
		}

		delay(20);
	}
	// delay(500);
	yield();
	delay(500);
}