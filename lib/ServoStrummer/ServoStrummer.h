#ifndef SERVOSTRUMMER_H
#define SERVOSTRUMMER_H

#include "../Actuator/Actuator.h"
#include <Adafruit_PWMServoDriver.h>

class ServoStrummer : public Actuator
{
public:
	ServoStrummer(uint8_t pin, Adafruit_PWMServoDriver *pwm, const std::string &name);
	void begin() override;
	void actuate(int degrees) override; // value could be angle or strum command
	void home() override;

	void pluck(int duration_ms);

	std::string name() override;

private:
	uint8_t _pin;
	Adafruit_PWMServoDriver *_pwm;
	int _homePosition;
	int _direction = 1; // 1 or -1, alternates each strum
	std::string _name;
};

#endif // SERVOSTRUMMER_H