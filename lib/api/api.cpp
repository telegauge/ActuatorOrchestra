#include "api.h"
#include "../Ukulele/Ukulele.h"
#include "../OledDisplay/OledDisplay.h"

#define API_PREFIX "@ "

void init_api(WebServer &server, Ukulele *ukulele, OledDisplay *oled, bool *is_paused)
{
	server.on("/api/strum", HTTP_POST, [ukulele, oled, &server]()
						{
        oled->log(API_PREFIX "strum");
        ukulele->strum();
        server.send(200, "text/plain", "Strummed"); });
	server.on("/api/pluck", HTTP_POST, [ukulele, oled, &server]()
						{
        int idx = server.hasArg("string") ? server.arg("string").toInt() : 0;
        oled->log((String(API_PREFIX "pluck ") + idx).c_str());
        ukulele->pluck(idx);
        server.send(200, "text/plain", "Plucked"); });
	server.on("/api/fret", HTTP_POST, [ukulele, oled, &server]()
						{
        int fret = server.hasArg("fret") ? server.arg("fret").toInt() : 0;
        String p = server.hasArg("pressed") ? server.arg("pressed") : "";
        oled->log((String(API_PREFIX "fret ") + fret + " " + p).c_str());
        std::vector<bool> pressed;
        for (size_t i = 0; i < p.length(); ++i) pressed.push_back(p[i] == '1');
        ukulele->fret(fret, pressed);
        server.send(200, "text/plain", "Fretted"); });
	server.on("/api/pause", HTTP_POST, [oled, is_paused, &server]()
						{
        oled->log(API_PREFIX "pause");
        *is_paused = true;
				oled->toolbar("paused");

				server.send(200, "text/plain", "Paused"); });
	server.on("/api/play", HTTP_POST, [oled, is_paused, &server]()
						{
        oled->log(API_PREFIX "play");
        *is_paused = false;
				oled->toolbar("playing");

				server.send(200, "text/plain", "Playing"); });
	server.on("/api/info", HTTP_GET, [ukulele, oled, &server]()
						{
        oled->log(API_PREFIX "info");
        String json = "{";
        json += "\"instrument\":\"ukulele\",";
        json += "\"strings\":" + String(ukulele->numStrings());
        json += "}";
        server.send(200, "application/json", json); });
}