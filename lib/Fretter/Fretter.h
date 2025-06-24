#ifndef FRETTER_H
#define FRETTER_H

#include "../Actuator/Actuator.h"
#include <Adafruit_PWMServoDriver.h>
#include <string>

class Fretter : public Actuator
{
public:
	Fretter(int pin_left, int pin_right, Adafruit_PWMServoDriver *pwm, const std::string &name);
	void begin() override;
	void actuate(int left_value, int right_value);
	void home() override;
	void minmax(int duration_ms);
	void loop(int ms) override;
	void fret(int left_degrees, int right_degrees);
	void actuate(int value) override;
	std::string name() override;

private:
	int _pin_left, _pin_right;
	Adafruit_PWMServoDriver *_pwm;
	std::string _name;
	int _homePosition = 90;
};

#endif // FRETTER_H