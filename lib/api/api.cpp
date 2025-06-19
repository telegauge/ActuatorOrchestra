#include "api.h"
#include "../Ukulele/Ukulele.h"
#include "../OledDisplay/OledDisplay.h"
#include <ArduinoJson.h>

extern float readBatteryPercent();

#define API_PREFIX "@ "

// Global pointers for WebSocket callbacks
static Ukulele* g_ukulele = nullptr;
static OledDisplay* g_oled = nullptr;
static bool* g_is_paused = nullptr;

void sendCORS(WebServer &server, int code, const char *type, const String &body)
{
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(code, type, body);
}

// Shared command handlers
void handle_strum(int delay_ms) {
	g_oled->log(API_PREFIX "strum");
	g_ukulele->strum(0, delay_ms);
}

void handle_pluck(int idx) {
	g_oled->log((String(API_PREFIX "pluck ") + idx).c_str());
	g_ukulele->pluck(idx);
}

void handle_fret(int fret, const String &pressed) {
	g_oled->log((String(API_PREFIX "fret ") + fret).c_str());
	g_ukulele->fret(fret, pressed);
}

void handle_chord(const String &chord, const String &pressed) {
	for (int i = 0; i < pressed.length(); i += 4) {
		String chunk = pressed.substring(i, i + 4);
		g_ukulele->fret(i / 4, chunk);
	}
	g_oled->log((String(API_PREFIX "chord ") + chord).c_str());
}

void handle_pause() {
	g_oled->log(API_PREFIX "pause");
	*g_is_paused = true;
	g_oled->toolbar("paused");
}

void handle_play() {
	g_oled->log(API_PREFIX "play");
	*g_is_paused = false;
	g_oled->toolbar("playing");
}

String handle_info() {
	g_oled->log(API_PREFIX "info");
	String json = "{";
	json += "\"instrument\":\"ukulele\",";
	json += "\"strings\":" + String(g_ukulele->numStrings());
	json += ",\"fretters\":" + String(g_ukulele->numFretters());
	json += "}";
	return json;
}

String handle_battery() {
	float percent = readBatteryPercent();
	char buf[8];
	snprintf(buf, sizeof(buf), "%d", (int)percent);
	g_oled->log((String(API_PREFIX "BAT ") + buf + "%").c_str());
	String json = "{";
	json += "\"percent\":" + String(percent);
	json += "}";
	return json;
}

void handle_websocket_event(WebSocketsServer &ws, uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
	switch(type) {
		case WStype_DISCONNECTED:
			g_oled->log(API_PREFIX "WS Disconnected");
			break;
		case WStype_CONNECTED:
			g_oled->log(API_PREFIX "WS Connected");
			break;
		case WStype_TEXT: {
			StaticJsonDocument<200> doc;
			DeserializationError error = deserializeJson(doc, payload);
			if (error) return;

			const char* cmd = doc["cmd"];
			if (!cmd) return;

			if (strcmp(cmd, "strum") == 0) {
				handle_strum(doc["delay"] | 50);
			}
			else if (strcmp(cmd, "pluck") == 0) {
				handle_pluck(doc["string"] | 0);
			}
			else if (strcmp(cmd, "fret") == 0) {
				String pressed = doc["pressed"] | "";
				handle_fret(doc["fret"] | 0, pressed);
			}
			else if (strcmp(cmd, "chord") == 0) {
				String chord = doc["chord"] | "";
				String pressed = doc["pressed"] | "";
				handle_chord(chord, pressed);
			}
			else if (strcmp(cmd, "pause") == 0) {
				handle_pause();
			}
			else if (strcmp(cmd, "play") == 0) {
				handle_play();
			}
			else if (strcmp(cmd, "info") == 0) {
				String response = handle_info();
				ws.sendTXT(num, response);
			}
			else if (strcmp(cmd, "battery") == 0) {
				String response = handle_battery();
				ws.sendTXT(num, response);
			}
			break;
		}
		default:
			break;
	}
}

void init_api(WebServer &server, WebSocketsServer &ws, Ukulele *ukulele, OledDisplay *oled, bool *is_paused)
{
	// Store pointers for WebSocket callbacks
	g_ukulele = ukulele;
	g_oled = oled;
	g_is_paused = is_paused;

	// Setup WebSocket server
	ws.onEvent([&ws](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
		handle_websocket_event(ws, num, type, payload, length);
	});
	ws.begin();

	// REST API endpoints
	server.on("/api/strum", HTTP_POST, [&server]() {
		int delay = server.hasArg("delay") ? server.arg("delay").toInt() : 50;
		handle_strum(delay);
		sendCORS(server, 200, "text/plain", "Strummed");
	});

	server.on("/api/pluck", HTTP_POST, [&server]() {
		int idx = server.hasArg("string") ? server.arg("string").toInt() : 0;
		handle_pluck(idx);
		sendCORS(server, 200, "text/plain", "Plucked");
	});

	server.on("/api/fret", HTTP_POST, [&server]() {
		int fret = server.hasArg("fret") ? server.arg("fret").toInt() : 0;
		String pressed = server.hasArg("pressed") ? server.arg("pressed") : "";
		handle_fret(fret, pressed);
		sendCORS(server, 200, "text/plain", "Fretted");
	});

	server.on("/api/chord", HTTP_POST, [&server]() {
		String chord = server.hasArg("chord") ? server.arg("chord") : "";
		String pressed = server.hasArg("pressed") ? server.arg("pressed") : "";
		handle_chord(chord, pressed);
		sendCORS(server, 200, "text/plain", "Fretted " + chord);
	});

	server.on("/api/pause", HTTP_POST, [&server]() {
		handle_pause();
		sendCORS(server, 200, "text/plain", "Paused");
	});

	server.on("/api/play", HTTP_POST, [&server]() {
		handle_play();
		sendCORS(server, 200, "text/plain", "Playing");
	});

	server.on("/api/info", HTTP_GET, [&server]() {
		String json = handle_info();
		sendCORS(server, 200, "application/json", json);
	});

	server.on("/api/battery", HTTP_GET, [&server]() {
		String response = handle_battery();
		sendCORS(server, 200, "text/plain", response);
	});
}