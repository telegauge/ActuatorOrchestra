#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <ArduinoJson.h>
#include <vector>
#include <string>

struct ActuatorConfig
{
	std::string type;
	int pin;
	std::string name;
	JsonObject options;
};

struct InstrumentConfig
{
	std::string instrument;
	int strings;
	std::vector<ActuatorConfig> actuators;
};

class ConfigLoader
{
public:
	static bool loadConfig(const char *filename, InstrumentConfig &config);
};

#endif // CONFIGLOADER_H