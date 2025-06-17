#include "Plucker.h"

#define SERVO_MIN 150
#define SERVO_MAX 600
#define SERVO_RANGE_DEGREES 90

#define HOME_POSITION 90
#define OVERSHOOT_DEGREES 5

Plucker::Plucker(uint8_t pin, Adafruit_PWMServoDriver *pwm, const std::string &name)
		: _pin(pin), _pwm(pwm), _name(name) {}

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
	// Serial.printf("Plucking %d º%d\n", _pin, HOME_POSITION + _direction * swing_degrees);
	actuate(HOME_POSITION + _direction * swing_degrees);
	if (duration_ms > 0)
	{
		delay(duration_ms);
		actuate(HOME_POSITION - _direction * OVERSHOOT_DEGREES);
	}
	_direction = -_direction;
}

void Plucker::home()
{
	Serial.printf("Plucker Homing %s\n", _name.c_str());
	actuate(HOME_POSITION + _direction * OVERSHOOT_DEGREES);
}

std::string Plucker::name()
{
	return _name;
}