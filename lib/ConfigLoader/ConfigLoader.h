#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <ArduinoJson.h>
#include <vector>
#include <string>

struct ActuatorConfig
{
	std::string type;
	int pin;
	int bus = 0;
	std::string name;
	JsonObject options;
};

struct FretterConfig
{
	std::string type;
	int pin_left;
	int pin_right;
	int bus = 0;
	std::string name;
	JsonObject options;
};

struct Config
{
	int id;
	std::string type;
	std::string ip;
	std::string name;
	JsonObject config;
};

struct InstrumentConfig
{
	std::string instrument;
	int strings;
	std::vector<ActuatorConfig> pluckers;
	std::vector<FretterConfig> fretters;
};

class ConfigLoader
{
public:
	static bool loadConfig(const char *filename, InstrumentConfig &config);
	static bool saveConfig(const char *filename, const String &json);
};

#endif // CONFIGLOADER_H