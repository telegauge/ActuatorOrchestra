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
	void test();
	void strum(int duration_ms = 0, int delay_ms = 50);
	void pluck(int idx, int duration_ms = 0);
	void chord(const std::string &pressed);
	void fret(int fret_number, const std::vector<bool> &pressed);
	void fret(int fret_number, const String &pressed);

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