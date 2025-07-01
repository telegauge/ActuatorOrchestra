#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <FS.h>
// #include <SPIFFS.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <LittleFS.h>

#include "../lib/ConfigLoader/ConfigLoader.h"
#include "../lib/ActuatorFactory/ActuatorFactory.h"
#include "../lib/TimingEngine/TimingEngine.h"
#include "../lib/OledDisplay/OledDisplay.h"
#include "../lib/Ukulele/Ukulele.h"
#include "../lib/api/api.h"

WiFiClient wifi;
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42);
std::vector<Adafruit_PWMServoDriver *> pwms;

String scanI2C();
Ukulele *ukulele = nullptr;
TimingEngine timingEngine;
OledDisplay oled;
WebServer server(80);
WebSocketsServer webSocket(81); // WebSocket server on port 81

const int STRUM_SWING_DEGREES = 30;
const int STRUM_DURATION_MS = 150;
bool paused = true;

bool pressed[] = {false, false, false, false};

#define BATTERY_PIN 35

void setup()
{
	Serial.begin(115200);
	delay(500); // wait for the servos to home, and the terminal to catch up
	oled.begin();

	oled.log("Booting...");

	// FILESYSTEM
	if (!LittleFS.begin(true))
	{
		oled.log("LittleFS Fail");
		Serial.println("LittleFS Mount Failed");
		return;
	}
	oled.log("Files    OK");
	ConfigLoader::listFiles();

	// Load WiFi credentials from /wifi.json
	oled.log("WIFI...");
	WiFiConfig wifiConfig;
	if (!ConfigLoader::loadWiFiConfig("/wifi.json", wifiConfig))
	{
		oled.log("...FAIL");
		Serial.println("Failed to load WiFi config: wifi.json");
		return;
	}
	WiFi.setHostname(wifiConfig.device_name.c_str());
	oled.log(wifiConfig.device_name.c_str());
	WiFi.begin(wifiConfig.ssid.c_str(), wifiConfig.password.c_str());
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	String ip = WiFi.localIP().toString();
	Serial.print("IP: ");
	Serial.println(ip);
	ip.replace("192.168.", "..."); // hide the IP address
	oled.log(ip.c_str());					 // Convert IPAddress to String before logging

	// SERVO
	Serial.printf("Servo 1 %d\n", pwm1.begin());
	Serial.printf("Servo 2 %d\n", pwm2.begin());
	pwm1.setPWMFreq(50);
	pwm2.setPWMFreq(50);
	pwm1.setPWM(1, 0, 1000);
	pwm2.setPWM(1, 0, 1000);
	delay(1000);
	pwm1.setPWM(1, 0, 0);
	pwm2.setPWM(1, 0, 0);

	// Add two PWM servo drivers to the vector of drivers
	pwms.push_back(&pwm1); // Add first PWM driver (0x40)
	pwms.push_back(&pwm2); // Add second PWM driver (0x42)
	oled.log("Servo    OK");

	// String devices = scanI2C();
	// oled.log(devices.c_str());

	JsonDocument config2;
	if (!ConfigLoader::loadConfig("/config.json", config2))
	{
		oled.log("Config  FAIL");
		Serial.println("Failed to load config: config.json");
		return;
	}
	oled.log(config2["name"].as<const char *>());

	// InstrumentConfig config;
	// if (!ConfigLoader::loadConfig("/ukulele.json", config))
	// {
	// 	// TO upload: pio run -t uploadfs
	// 	oled.log("Config FAIL");
	// 	Serial.println("Failed to load config");
	// 	return;
	// }
	oled.log("Config   OK");

	ukulele = new Ukulele(config2, pwms, &oled);
	ukulele->begin();
	ukulele->home();

	init_api(server, webSocket, ukulele, &oled, &paused);
	oled.log("API      OK");

	// oled.toolbar(paused ? "xx" : "yy");
	oled.init();
	server.begin();
	oled.log("Ready!");
}

bool Bools[][4] = {
		{false, false, false, false}, // 0
		{false, false, false, true},	// 1
		{false, false, true, false},	// 2
		{false, false, true, true},		// 3
		{false, true, false, false},	// 4
		{false, true, false, true},		// 5
		{false, true, true, false},		// 6
		{false, true, true, true},		// 7
		{true, false, false, false},	// 8
		{true, false, false, true},		// 9
		{true, false, true, false},		// 10
		{true, false, true, true},		// 11
		{true, true, false, false},		// 12
		{true, true, false, true},		// 13
		{true, true, true, false},		// 14
		{true, true, true, true}			// 15
};
void loop()
{
	server.handleClient();
	webSocket.loop();
}

String scanI2C()
{
	String devices;
	int address;
	int error;
	int nDevices = 0;
	Serial.println("I2C Scanning...");
	for (address = 1; address < 127; address++)
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0)
		{
			char hex_string[10];
			snprintf(hex_string, sizeof(hex_string), "0x%02X", address);
			// Serial.println(hex_string);
			oled.log(hex_string);
			nDevices++;
			devices += String(hex_string) + ",";
		}
	}
	Serial.printf("I2C Scanning... done %d devices\n", nDevices);
	return devices;
}

float readBatteryPercent()
{
	int raw = analogRead(BATTERY_PIN);
	float voltage = raw * (3.3 / 4095.0) * 2; // 2x because of voltage divider
	// Map 3.0V (0%) to 4.2V (100%)
	float percent = (voltage - 3.0) / (4.2 - 3.0) * 100.0;
	Serial.printf("Battery: %d %f %f\n", raw, voltage, percent);
	percent *= 100.0 / 76.0; // calibration
	if (percent < 0)
		percent = 0;
	if (percent > 100)
		percent = 100;
	return percent;
}