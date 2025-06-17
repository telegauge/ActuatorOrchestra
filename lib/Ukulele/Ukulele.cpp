#include "Ukulele.h"
#include <string>
#include <map>

Ukulele::Ukulele(const InstrumentConfig &config, Adafruit_PWMServoDriver *pwm, OledDisplay *oled_display)
		: oled(oled_display)
{
	for (const auto &a : config.pluckers)
	{
		if (a.type == "plucker")
		{
			pluckers.push_back(new Plucker(a.pin, pwm, a.name));
		}
	}
	for (const auto &f : config.fretters)
	{
		if (f.type == "fretter")
		{
			fretters.push_back(new Fretter(f.pin1, f.pin2, pwm, f.name));
		}
	}
}

Ukulele::~Ukulele()
{
	for (auto *s : pluckers)
		delete s;
	for (auto *f : fretters)
		delete f;
}

void Ukulele::begin()
{
	for (auto *s : pluckers)
		s->begin();
	for (auto *f : fretters)
		f->begin();
}

void Ukulele::loop(int ms)
{
	if (command_queue.size() > 0)
	{
		Command cmd = command_queue.front();
		Serial.printf("Command Queue Size: %s %d %d\n", cmd.name.c_str(), cmd.time_ms, ms);
		if (cmd.time_ms <= ms)
		{
			Serial.printf("Command %s\n", cmd.name.c_str());
			if (cmd.name == "strum")
			{
				strum();
			}
			else if (cmd.name == "home")
			{
				home();
			}
			else if (cmd.name == "fret")
			{
				// fret(cmd.args[0], cmd.args[1]);
			}
			else
			{
				Serial.printf("Unknown command %s\n", cmd.name.c_str());
			}
			command_queue.pop();
		}
	}
}

// Send everything to home position
void Ukulele::home()
{
	for (auto *s : pluckers)
		s->home();
	for (auto *f : fretters)
		f->home();
}

void Ukulele::test()
{
	for (auto *f : fretters)
		f->minmax(1000);
}

// Strum all the strings
void Ukulele::strum(int duration_ms)
{
	for (auto *s : pluckers)
	{
		s->pluck();
		delay(10);
	}
	if (duration_ms > 0)
	{
		delay(duration_ms);
		// home();
	}
}

// Pluck a specific string
void Ukulele::pluck(int idx, int duration_ms)
{
	if (idx >= 0 && idx < (int)pluckers.size())
		pluckers[idx]->pluck(duration_ms);
}

// Fret a specific fret
void Ukulele::fret(int fret_number, const std::vector<bool> &pressed)
{
	// Use fret_number as needed for your logic
	size_t n = fretters.size();
	if (pressed.size() < 2 * n)
		return;

	std::string key_left;
	std::string key_right;

	key_left += pressed[0] ? '1' : '0';
	key_left += pressed[1] ? '1' : '0';
	key_right += pressed[2] ? '1' : '0';
	key_right += pressed[3] ? '1' : '0';
	const std::map<std::string, int> fret_map = {
			{"00", 180},
			{"01", 120},
			{"10", 0},
			{"11", 60}};

	fretters[fret_number]->fret(fret_map.at(key_left), fret_map.at(key_right));

	oled->grid(4, fret_number, pressed[0]);
	oled->grid(3, fret_number, pressed[1]);
	oled->grid(2, fret_number, pressed[2]);
	oled->grid(1, fret_number, pressed[3]);
}

int Ukulele::numStrings() const
{
	return pluckers.size();
}

std::string Ukulele::stringName(int idx) const
{
	return (idx >= 0 && idx < (int)pluckers.size()) ? pluckers[idx]->name() : "";
}

int Ukulele::numFretters() const
{
	return fretters.size();
}

std::string Ukulele::fretterName(int idx) const
{
	return (idx >= 0 && idx < (int)fretters.size()) ? fretters[idx]->name() : "";
}