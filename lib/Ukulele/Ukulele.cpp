#include "Ukulele.h"

Ukulele::Ukulele(const InstrumentConfig &config, Adafruit_PWMServoDriver *pwm)
{
	for (const auto &a : config.actuators)
	{
		if (a.type == "strummer")
		{
			strummers.push_back(new ServoStrummer(a.pin, pwm, a.name));
		}
	}
}

Ukulele::~Ukulele()
{
	for (auto *s : strummers)
		delete s;
}

void Ukulele::begin()
{
	for (auto *s : strummers)
		s->begin();
}

void Ukulele::home()
{
	for (auto *s : strummers)
		s->home();
}

void Ukulele::strum(int duration_ms)
{
	for (auto *s : strummers)
	{
		s->pluck(duration_ms);
		delay(10);
	}
}

void Ukulele::pluck(int idx, int duration_ms)
{
	if (idx >= 0 && idx < (int)strummers.size())
		strummers[idx]->pluck(duration_ms);
}

int Ukulele::numStrings() const
{
	return strummers.size();
}

std::string Ukulele::stringName(int idx) const
{
	return (idx >= 0 && idx < (int)strummers.size()) ? strummers[idx]->name() : "";
}