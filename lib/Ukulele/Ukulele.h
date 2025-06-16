#ifndef UKULELE_H
#define UKULELE_H

#include <vector>
#include <string>
#include "../ConfigLoader/ConfigLoader.h"
#include "../Plucker/Plucker.h"
#include <Adafruit_PWMServoDriver.h>

class Ukulele
{
public:
	Ukulele(const InstrumentConfig &config, Adafruit_PWMServoDriver *pwm);
	~Ukulele();
	void begin();
	void home();
	void strum(int duration_ms);
	void pluck(int idx, int duration_ms);
	int numStrings() const;
	std::string stringName(int idx) const;

private:
	std::vector<Plucker *> pluckers;
};

#endif // UKULELE_H