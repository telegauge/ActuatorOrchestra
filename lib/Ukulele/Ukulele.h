#ifndef UKULELE_H
#define UKULELE_H

#include <vector>
#include <string>
#include "../ConfigLoader/ConfigLoader.h"
#include "../Plucker/Plucker.h"
#include "../Fretter/Fretter.h"
#include "../Instrument/Instrument.h"
#include "../OledDisplay/OledDisplay.h"
#include <Adafruit_PWMServoDriver.h>

class Ukulele : public Instrument
{
public:
	Ukulele(const InstrumentConfig &config, std::vector<Adafruit_PWMServoDriver *> &pwm, OledDisplay *oled);
	~Ukulele();
	void begin() override;
	void loop(int ms) override;
	void home() override;

	void strum();
	void pluck(int string_num);
	void fret(int fret_num, const String &fingers);
	void chord(const String &pressed);

	int numStrings() const;
	std::string stringName(int idx) const;
	int numFretters() const;
	std::string fretterName(int idx) const;

private:
	std::vector<Plucker *> pluckers;
	std::vector<Fretter *> fretters;
	OledDisplay *oled;
};

#endif // UKULELE_H