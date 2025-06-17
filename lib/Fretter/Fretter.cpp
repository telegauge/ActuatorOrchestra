#include "Fretter.h"

// #define SERVO_MIN 150
// #define SERVO_MAX 600

#define SERVO_MIN 100
#define SERVO_MAX 500

#define HOME_POSITION 120 // in degrees

Fretter::Fretter(int pin1, int pin2, Adafruit_PWMServoDriver *pwm, const std::string &name)
		: _pin1(pin1), _pin2(pin2), _pwm(pwm), _name(name) {}

void Fretter::begin()
{
	// _pwm->begin();
	// _pwm->setPWMFreq(50);
}

void Fretter::loop(int ms)
{
	Serial.printf("Fretter loop %d\n", ms);
}

// Set both servos to these values
void Fretter::actuate(int left_degrees, int right_degrees)
{
	int left_pulse = map(left_degrees, 0, 180, SERVO_MIN, SERVO_MAX);
	int right_pulse = map(right_degrees, 0, 180, SERVO_MIN, SERVO_MAX);
	_pwm->setPWM(_pin1, 0, left_pulse);
	_pwm->setPWM(_pin2, 0, right_pulse);
}
void Fretter::actuate(int value)
{
	actuate(value, value);
}

void Fretter::home()
{
	Serial.printf("Fretter Homing %s\n", _name.c_str());
	actuate(HOME_POSITION, HOME_POSITION);
}

void Fretter::minmax(int duration_ms)
{
	Serial.printf("Fretter MinMax %s\n", _name.c_str());
	actuate(0, 0);
	delay(duration_ms);
	actuate(180, 180);
	delay(duration_ms);
	actuate(HOME_POSITION, HOME_POSITION);
	delay(duration_ms);
}

void Fretter::fret(int left_degrees, int right_degrees)
{
	// Serial.printf("Fretting: %d º%d %d º%d\n", _pin1, left_degrees, _pin2, right_degrees);
	actuate(left_degrees, right_degrees);
}

std::string Fretter::name()
{
	return _name;
}