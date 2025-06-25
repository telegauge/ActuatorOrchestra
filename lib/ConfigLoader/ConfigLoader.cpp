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

	StaticJsonDocument<2048> doc;
	DeserializationError error = deserializeJson(doc, buf.get());
	if (error)
	{
		return false;
	}

	config.instrument = doc["instrument"].as<std::string>();
	config.strings = doc["strings"];
	config.pluckers.clear();
	config.fretters.clear();
	for (JsonObject act : doc["pluckers"].as<JsonArray>())
	{
		ActuatorConfig aconf;
		aconf.type = act["type"].as<std::string>();
		aconf.pin = act["pin"];
		aconf.bus = act["bus"] | 0;
		aconf.name = act["name"].as<std::string>();
		aconf.options = act["options"];
		config.pluckers.push_back(aconf);
	}
	for (JsonObject fret : doc["fretters"].as<JsonArray>())
	{
		FretterConfig fconf;
		fconf.type = fret["type"].as<std::string>();
		fconf.pin_left = fret["pin_left"];
		fconf.pin_right = fret["pin_right"];
		fconf.bus = fret["bus"] | 0;
		fconf.name = fret["name"].as<std::string>();
		fconf.options = fret["options"];
		config.fretters.push_back(fconf);
	}
	return true;
}

bool ConfigLoader::saveConfig(const char *filename, const String &json)
{
	File file = SPIFFS.open(filename, "w");
	if (!file)
		return false;
	file.print(json);
	file.close();
	return true;
}