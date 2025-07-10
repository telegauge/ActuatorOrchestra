#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <FS.h>
// #include <SPIFFS.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <LittleFS.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "../lib/ConfigLoader/ConfigLoader.h"
#include "../lib/ActuatorFactory/ActuatorFactory.h"
#include "../lib/TimingEngine/TimingEngine.h"
#include "../lib/OledDisplay/OledDisplay.h"
#include "../lib/Ukulele/Ukulele.h"
#include "../lib/api/api.h"

WiFiClient wifi;
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x42);
std::vector<Adafruit_PWMServoDriver *> pwms;

String scanI2C();
Ukulele *ukulele = nullptr;
TimingEngine timingEngine;
OledDisplay oled;
WebServer server(80);
WebSocketsServer webSocket(81); // WebSocket server on port 81

#define BEATS_PER_MINUTE 120
unsigned long g_beat = 0;
int g_elapsed_ms = 0;

const int STRUM_SWING_DEGREES = 30;
const int STRUM_DURATION_MS = 150;
bool paused = true;

bool pressed[] = {false, false, false, false};

#define BATTERY_PIN 35

struct PluckRequest
{
	int idx;
	unsigned long request_time;
};

QueueHandle_t pluckQueue;

void TimingTask(void *pvParameters)
{
	PluckRequest req;
	static unsigned long last_pluck_time = 0;
	while (true)
	{
		if (xQueueReceive(pluckQueue, &req, 0) == pdTRUE)
		{
			unsigned long exec_time = millis();
			unsigned long interval = last_pluck_time ? (exec_time - last_pluck_time) : 0;
			last_pluck_time = exec_time;
			Serial.printf("Pluck %d ms:%lu\n", req.idx, interval);
			if (ukulele)
				ukulele->pluck(req.idx);
		}
		timingEngine.update();
		vTaskDelay(1); // 1ms tick
	}
}

void setup()
{
	Serial.begin(115200);
	delay(500); // wait for the servos to home, and the terminal to catch up
	oled.begin();

	Wire.begin();

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

	pluckQueue = xQueueCreate(16, sizeof(PluckRequest));
	timingEngine.start();
	xTaskCreatePinnedToCore(
			TimingTask,		// Task function
			"TimingTask", // Name
			2048,					// Stack size
			NULL,					// Parameters
			1,						// Priority
			NULL,					// Task handle
			1							// Core (0 or 1)
	);
	oled.log("Timing   OK");

	// SERVO
	Serial.printf("Servo 1 %d\n", pwm1.begin());
	Serial.printf("Servo 2 %d\n", pwm2.begin());
	Serial.printf("Servo 3 %d\n", pwm3.begin());
	pwm1.setPWMFreq(50);
	pwm2.setPWMFreq(50);
	pwm3.setPWMFreq(50);

	// Add three PWM servo drivers to the vector of drivers
	pwms.push_back(&pwm1); // 0x40
	pwms.push_back(&pwm2); // 0x41
	pwms.push_back(&pwm3); // 0x42
	oled.log("Servo    OK");

	JsonDocument config;
	if (!ConfigLoader::loadConfig("/config.json", config))
	{
		Serial.println("Loading default config: config_default.json");
		if (!ConfigLoader::loadConfig("/config_default.json", config))
		{
			oled.log("Config  FAIL");
			Serial.println("Failed to load config: config.json");
			return;
		}
	}
	oled.log(config["name"].as<const char *>());

	oled.log("Config   OK");

	ukulele = new Ukulele(config, pwms, &oled);
	ukulele->begin();
	ukulele->home();

	init_api(server, webSocket, ukulele, &oled, &paused, &timingEngine);
	oled.log("API      OK");

	// oled.toolbar(paused ? "xx" : "yy");
	oled.init();
	server.begin();
	oled.log("Ready!");
}

void loop()
{
	server.handleClient();
	webSocket.loop();

	// timingEngine.update(); // Now handled by FreeRTOS task

	// static unsigned long lastBeat = 0;
	// unsigned long now = millis();
	// if (now - lastBeat >= 60000 / BEATS_PER_MINUTE)
	// {
	// 	g_beat++;
	// 	lastBeat = now;
	// 	String beat = "[----]";
	// 	beat[g_beat % 4 + 1] = '*';
	// 	String display = String(beat);
	// 	// display += " ";
	// 	display += String(g_beat);
	// 	oled.toolbar(display.c_str());
	// }
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