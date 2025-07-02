#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <ArduinoJson.h>
#include <vector>
#include <string>

// struct BaseConfig
// {
// 	int id;
// 	int config_version;
// 	std::string type;
// 	std::string ip;
// 	std::string name;
// 	JsonObject config;
// };

struct WiFiConfig
{
	String ssid;
	String password;
	String device_name;
};

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
	// static bool loadConfig(const char *filename, InstrumentConfig &config);
	static bool loadConfig(const char *filename, JsonDocument &config);
	static bool saveConfig(const char *filename, const String &json);
	static void listFiles();
	static bool loadWiFiConfig(const char *filename, WiFiConfig &config);
};

#endif // CONFIGLOADER_H