#ifndef API_H
#define API_H

#include <WebServer.h>
class Ukulele;
class OledDisplay;

void init_api(WebServer &server, Ukulele *ukulele, OledDisplay *oled, bool *is_paused);

#endif // API_H