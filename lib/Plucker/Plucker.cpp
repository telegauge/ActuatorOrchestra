#include "Plucker.h"

#define SERVO_MIN 150
#define SERVO_MAX 600
#define SERVO_RANGE_DEGREES 90

Plucker::Plucker(uint8_t pin, Adafruit_PWMServoDriver *pwm, const std::string &name)
		: _pin(pin), _pwm(pwm), _homePosition(90), _name(name) {}

void Plucker::begin()
{
	_pwm->begin();
	_pwm->setPWMFreq(50);
}

void Plucker::actuate(int degrees)
{
	int pulse = map(degrees, 0, 180, SERVO_MIN, SERVO_MAX);
	_pwm->setPWM(_pin, 0, pulse);
}

void Plucker::pluck(int duration_ms)
{
	int swing_degrees = 20;
	int overshoot_degrees = 2;
	actuate(_homePosition + _direction * swing_degrees);
	_direction = -_direction;
}

void Plucker::home()
{
	Serial.printf("Homing %s\n", _name.c_str());
	actuate(_homePosition);
}

std::string Plucker::name()
{
	return _name;
}