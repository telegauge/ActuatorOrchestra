#ifndef UKULELE_H
#define UKULELE_H

#include <vector>
#include <string>
#include "../ConfigLoader/ConfigLoader.h"
#include "../Plucker/Plucker.h"
#include "../Fretter/Fretter.h"
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
	void fret(int fret_number, const std::vector<bool> &pressed);
	int numStrings() const;
	std::string stringName(int idx) const;
	int numFretters() const;
	std::string fretterName(int idx) const;

private:
	std::vector<Plucker *> pluckers;
	std::vector<Fretter *> fretters;
};

#endif // UKULELE_H