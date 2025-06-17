#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include <queue>
#include <string>
#include <vector>

class Actuator
{
public:
	virtual void begin() = 0;
	virtual void loop(int ms) = 0;
	virtual void actuate(int value) = 0;
	virtual void home() = 0;
	virtual std::string name() = 0;
	virtual ~Actuator() {}
};

#endif // ACTUATOR_H