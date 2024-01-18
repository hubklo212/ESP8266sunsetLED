#ifndef _SUNSETLED_H

#include <string>
#include <chrono>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
using std::string;

std::string convertToString(const String& arduinoString);
uint32_t strToSec(string timeString);
uint32_t timeToSunset(string timeString, NTPClient timeClient);


#endif