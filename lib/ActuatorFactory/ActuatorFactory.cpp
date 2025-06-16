#include "ActuatorFactory.h"
#include "../Plucker/Plucker.h"
#include "../ConfigLoader/ConfigLoader.h"

std::vector<Actuator *> ActuatorFactory::createActuators(const std::vector<ActuatorConfig> &configs, Adafruit_PWMServoDriver *pwm)
{
	std::vector<Actuator *> actuators;
	for (const auto &conf : configs)
	{
		if (conf.type == "strummer")
		{
			actuators.push_back(new Plucker(conf.pin, pwm, conf.name));
		}
		// Add more actuator types here as needed
	}
	return actuators;
}

std::vector<Plucker *> ActuatorFactory::createStrummers(const std::vector<ActuatorConfig> &configs, Adafruit_PWMServoDriver *pwm)
{
	std::vector<Plucker *> strummers;
	for (const auto &conf : configs)
	{
		if (conf.type == "strummer")
		{
			strummers.push_back(new Plucker(conf.pin, pwm, conf.name));
		}
	}
	return strummers;
}