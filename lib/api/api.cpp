#include "api.h"
#include "../Ukulele/Ukulele.h"
#include "../OledDisplay/OledDisplay.h"

#define API_PREFIX "@ "

void sendCORS(WebServer &server, int code, const char *type, const String &body)
{
	server.sendHeader("Access-Control-Allow-Origin", "*");
	server.send(code, type, body);
}

void init_api(WebServer &server, Ukulele *ukulele, OledDisplay *oled, bool *is_paused)
{
	server.on("/api/strum", HTTP_POST, [ukulele, oled, &server]()
						{
        oled->log(API_PREFIX "strum");
        ukulele->strum(0);
        sendCORS(server, 200, "text/plain", "Strummed"); });
	server.on("/api/pluck", HTTP_POST, [ukulele, oled, &server]()
						{
        int idx = server.hasArg("string") ? server.arg("string").toInt() : 0;
        oled->log((String(API_PREFIX "pluck ") + idx).c_str());
        ukulele->pluck(idx);
        sendCORS(server, 200, "text/plain", "Plucked"); });
	server.on("/api/fret", HTTP_POST, [ukulele, oled, &server]()
						{
        int fret = server.hasArg("fret") ? server.arg("fret").toInt() : 0;
        String pressed = server.hasArg("pressed") ? server.arg("pressed") : "";
        oled->log(( fret + " " + pressed).c_str());
        ukulele->fret(fret, pressed);
        sendCORS(server, 200, "text/plain", "Fretted"); });
	server.on("/api/pause", HTTP_POST, [oled, is_paused, &server]()
						{
        oled->log(API_PREFIX "pause");
        *is_paused = true;
				oled->toolbar("paused");
        sendCORS(server, 200, "text/plain", "Paused"); });
	server.on("/api/play", HTTP_POST, [oled, is_paused, &server]()
						{
        oled->log(API_PREFIX "play");
        *is_paused = false;
				oled->toolbar("playing");
        sendCORS(server, 200, "text/plain", "Playing"); });
	server.on("/api/info", HTTP_GET, [ukulele, oled, &server]()
						{
        oled->log(API_PREFIX "info");
        String json = "{";
        json += "\"instrument\":\"ukulele\",";
        json += "\"strings\":" + String(ukulele->numStrings());
        json += "}";
        sendCORS(server, 200, "application/json", json); });
}