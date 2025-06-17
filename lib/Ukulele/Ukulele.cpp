#include "Ukulele.h"
#include <string>
#include <map>

Ukulele::Ukulele(const InstrumentConfig &config, Adafruit_PWMServoDriver *pwm)
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

void Ukulele::home()
{
	for (auto *s : pluckers)
		s->home();
	for (auto *f : fretters)
		f->home();
}

// Strum all the strings
void Ukulele::strum(int duration_ms)
{
	for (auto *s : pluckers)
	{
		s->pluck();
		delay(20);
	}
	delay(duration_ms);
	home();
	delay(200);
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
			{"00", 0},
			{"01", 60},
			{"10", 120},
			{"11", 180}};

	fretters[fret_number]->fret(fret_map.at(key_left), fret_map.at(key_right));
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