#include "ServoStrummer.h"

#define SERVO_MIN 150					 // Min pulse length out of 4096
#define SERVO_MAX 600					 // Max pulse length out of 4096
#define SERVO_RANGE_DEGREES 90 // Assume 90 degrees for mapping

ServoStrummer::ServoStrummer(uint8_t pin, Adafruit_PWMServoDriver *pwm, const std::string &name)
		: _pin(pin), _pwm(pwm), _homePosition(90), _name(name) {}

void ServoStrummer::begin()
{
	_pwm->begin();
	_pwm->setPWMFreq(50); // Standard servo frequency (50 Hz)
}

void ServoStrummer::actuate(int degrees)
{
	int pulse = map(degrees, 0, 180, SERVO_MIN, SERVO_MAX);

	_pwm->setPWM(_pin, 0, pulse);
}

void ServoStrummer::strum(int duration_ms)
{
	int swing_degrees = 20;		 // how far out to swing
	int overshoot_degrees = 2; // how far back to home, plus a bit extra to mute the string
	actuate(_homePosition + _direction * swing_degrees);
	// delay(duration_ms);
	// actuate(_homePosition - _direction * overshoot_degrees);
	_direction = -_direction;
}

void ServoStrummer::home()
{
	Serial.printf("Homing %s\n", _name.c_str());
	actuate(_homePosition);
}

std::string ServoStrummer::name()
{
	return _name;
}