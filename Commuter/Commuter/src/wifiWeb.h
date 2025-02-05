#ifndef __WIFI_WEB_H
#define __WIFI_WEB_H

#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
//#include <WebServer.h>
#include "ESPAsyncWebServer.h"
#include "prefer.h"
#include <string.h>
#include <ArduinoJson.h>
#include "dataOper.h"

extern AsyncWebServer server;

void serverUrlConfig();

void handle_not_found();
void sendNotFound();
void sendNotFound(AsyncWebServerRequest *request);
void sendInnerError();
void sendInnerError(AsyncWebServerRequest *request);
void sendForbidden(AsyncWebServerRequest *request);

void sendJsonToAPIClient(AsyncWebServerRequest *,String);
void sendJsonResToAPIClient(AsyncWebServerRequest *,short);


void sendSPIFFSFile(String path);
void sendSPIFFSFile(String path,AsyncWebServerRequest *request);

bool permissionCheck(AsyncWebServerRequest *request,const char*);






#endif
