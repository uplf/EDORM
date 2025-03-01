#ifndef __WIFI_WEB_H
#define __WIFI_WEB_H

#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>


#include "ESPAsyncWebServer.h"
#include "prefer.h"
#include <string.h>
#include <ArduinoJson.h>
#include "dataOper.h"

//#include "ETH.h"

#include "util.h"
#include "HTTPClient.h"

#include "ESP32Ping.h"




//服务器
extern AsyncWebServer server;

void serverUrlConfig();

void sendNotFound(AsyncWebServerRequest *request);
void sendInnerError(AsyncWebServerRequest *request);
void sendForbidden(AsyncWebServerRequest *request);

void sendJsonToAPIClient(AsyncWebServerRequest *,String);
void sendJsonResToAPIClient(AsyncWebServerRequest *,short);

void sendSPIFFSFile(String path,AsyncWebServerRequest *request);

bool permissionCheck(AsyncWebServerRequest *request,const char*);

//WiFi
void WiFiBegin();
void WiFiEnd();


//ETH
void ETHInit();
// #include "Ethernet_Generic.h"
// extern EthernetClient EthClient;


//HTTP request
extern HTTPClient myHTTPClient;

int HTTPGETreqToString(String URL,String *respString);
int HTTPGETreqMasterToString(String relativeURL,String *respString);


#endif
