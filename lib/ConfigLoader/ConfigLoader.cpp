#include <Arduino.h>
#include "ConfigLoader.h"
#include <FS.h>
#include <SPIFFS.h>

bool ConfigLoader::loadConfig(const char *filename, InstrumentConfig &config)
{
	File file = SPIFFS.open(filename, "r");
	if (!file)
	{
		return false;
	}
	size_t size = file.size();
	std::unique_ptr<char[]> buf(new char[size + 1]);
	file.readBytes(buf.get(), size);
	buf[size] = '\0';
	file.close();

	StaticJsonDocument<1024> doc;
	DeserializationError error = deserializeJson(doc, buf.get());
	if (error)
	{
		return false;
	}

	config.instrument = doc["instrument"].as<std::string>();
	config.strings = doc["strings"];
	config.actuators.clear();
	for (JsonObject act : doc["actuators"].as<JsonArray>())
	{
		ActuatorConfig aconf;
		aconf.type = act["type"].as<std::string>();
		aconf.pin = act["pin"];
		aconf.name = act["name"].as<std::string>();
		aconf.options = act["options"];
		config.actuators.push_back(aconf);
	}
	return true;
}