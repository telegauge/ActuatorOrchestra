#include <Arduino.h>
#include "ConfigLoader.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

void ConfigLoader::listFiles()
{
	Serial.println("Listing files:");
	File root = LittleFS.open("/");
	File file = root.openNextFile();
	while (file)
	{
		Serial.print(file.name());
		Serial.println(":");
		while (file.available())
		{
			Serial.write(file.read());
		}
		Serial.println("\n---");
		file = root.openNextFile();
	}
	Serial.println("Done");
}

bool ConfigLoader::loadConfig(const char *filename, BaseConfig &config)
{
	Serial.println("Loading config: " + String(filename));
	File file = LittleFS.open(filename, "r");
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
		Serial.println("Error: " + String(error.c_str()));
		return false;
	}

	config.id = doc["id"];
	config.config_version = doc["config_version"];
	config.type = doc["type"].as<std::string>();
	config.ip = doc["ip"].as<std::string>();
	config.name = doc["name"].as<std::string>();
	config.config = doc["config"];
	return true;
}

bool ConfigLoader::loadConfig(const char *filename, InstrumentConfig &config)
{
	File file = LittleFS.open(filename, "r");
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
		Serial.println("Error: " + String(error.c_str()));
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
	File file = LittleFS.open(filename, "w");
	if (!file)
		return false;
	file.print(json);
	file.close();
	return true;
}

bool ConfigLoader::loadWiFiConfig(const char *filename, WiFiConfig &config)
{
	File file = LittleFS.open(filename, "r");
	if (!file)
		return false;
	StaticJsonDocument<256> doc;
	deserializeJson(doc, file);
	file.close();
	if (!doc.containsKey("ssid") || !doc.containsKey("password"))
		return false;
	config.ssid = doc["ssid"].as<String>();
	config.password = doc["password"].as<String>();
	config.device_name = doc["device_name"].as<String>();
	return true;
}
