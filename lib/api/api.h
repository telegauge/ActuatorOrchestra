#ifndef API_H
#define API_H

#include <WebServer.h>
#include <WebSocketsServer.h>
class Ukulele;
class OledDisplay;

void init_api(WebServer &server, WebSocketsServer &ws, Ukulele *ukulele, OledDisplay *oled, bool *is_paused);
void handle_websocket_event(WebSocketsServer &ws, uint8_t num, WStype_t type, uint8_t *payload, size_t length);

#endif // API_H