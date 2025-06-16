#ifndef ACTUATORFACTORY_H
#define ACTUATORFACTORY_H

#include "../Actuator/Actuator.h"
#include "../Plucker/Plucker.h"
#include "../ConfigLoader/ConfigLoader.h"
#include <Adafruit_PWMServoDriver.h>
#include <vector>

class ActuatorFactory
{
public:
	static std::vector<Actuator *> createActuators(const std::vector<ActuatorConfig> &configs, Adafruit_PWMServoDriver *pwm);
	static std::vector<Plucker *> createStrummers(const std::vector<ActuatorConfig> &configs, Adafruit_PWMServoDriver *pwm);
};

#endif // ACTUATORFACTORY_H