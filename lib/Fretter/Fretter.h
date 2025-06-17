#ifndef FRETTER_H
#define FRETTER_H

#include "../Actuator/Actuator.h"
#include <Adafruit_PWMServoDriver.h>
#include <string>

class Fretter : public Actuator
{
public:
	Fretter(int pin1, int pin2, Adafruit_PWMServoDriver *pwm, const std::string &name);
	void begin() override;
	void actuate(int left_value, int right_value);
	void home() override;
	void fret(int left_degrees, int right_degrees);
	void actuate(int value) override;
	std::string name() override;

private:
	int _pin1, _pin2;
	Adafruit_PWMServoDriver *_pwm;
	std::string _name;
	int _homePosition = 90;
};

#endif // FRETTER_H