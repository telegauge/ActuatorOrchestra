#include "Ukulele.h"

Ukulele::Ukulele(const InstrumentConfig &config, Adafruit_PWMServoDriver *pwm)
{
	for (const auto &a : config.actuators)
	{
		if (a.type == "strummer")
		{
			pluckers.push_back(new Plucker(a.pin, pwm, a.name));
		}
	}
}

Ukulele::~Ukulele()
{
	for (auto *s : pluckers)
		delete s;
}

void Ukulele::begin()
{
	for (auto *s : pluckers)
		s->begin();
}

void Ukulele::home()
{
	for (auto *s : pluckers)
		s->home();
}

void Ukulele::strum(int duration_ms)
{
	for (auto *s : pluckers)
	{
		s->pluck(duration_ms);
		delay(10);
	}
}

void Ukulele::pluck(int idx, int duration_ms)
{
	if (idx >= 0 && idx < (int)pluckers.size())
		pluckers[idx]->pluck(duration_ms);
}

int Ukulele::numStrings() const
{
	return pluckers.size();
}

std::string Ukulele::stringName(int idx) const
{
	return (idx >= 0 && idx < (int)pluckers.size()) ? pluckers[idx]->name() : "";
}