#ifndef __COMM_H
#define __COMM_H
//这里负责传输和服务器的事宜

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include "ESPAsyncWebServer.h"
#include <string.h>
#include "opr.h"
#include "HTTPClient.h"

#define _ENABLE true
#define _DISABLE false

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
void setWiFi(bool);


//HTTP request
extern HTTPClient HttpClient;

int HTTPreqToString(String URL,String *respString,int (HTTPClient::*REQ)()=&HTTPClient::GET);
int HTTPreqMasterToString(String relativeURL,String *respString,int (HTTPClient::*REQ)()=&HTTPClient::GET);




#endif
