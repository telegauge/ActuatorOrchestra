#ifndef PLUCKER_H
#define PLUCKER_H

#include "../Actuator/Actuator.h"
#include <Adafruit_PWMServoDriver.h>

class Plucker : public Actuator
{
public:
	Plucker(uint8_t pin, Adafruit_PWMServoDriver *pwm, const std::string &name);
	void begin() override;
	void actuate(int degrees) override;
	void home() override;
	void pluck(int duration_ms);
	std::string name() override;

private:
	uint8_t _pin;
	Adafruit_PWMServoDriver *_pwm;
	int _homePosition;
	int _direction = 1;
	std::string _name;
};

#endif // PLUCKER_H