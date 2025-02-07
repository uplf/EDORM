
#include <Arduino.h>
#include "wifiWeb.h"
#include "prefer.h"
#include "dataOper.h"
#include "util.h"


#include "defines.h"





AsyncWebServer server(80);

const char* testUrl[] = {"http://httpbin.org/get",
  "https://jsonplaceholder.typicode.com/todos/1",
  "http://www.google.com",
  "http://www.example.com",
  "https://ipinfo.io/json",
  "http://httpstat.us/200",
  "http://checkip.amazonaws.com/",
  "https://reqres.in/api/users/1",
  "https://catfact.ninja/fact"};
  char serverx[] = "arduino.tips";
/*
short taskScanner(int* param);
bool FBIEnable=false,LightEnable=false;

*/







void setup()
{

    Serial.begin(115200);

    /*
    serverUrlConfig();
    server.begin();
    prefInit();
    ETHInit();
    WiFiBegin();

    Serial.println("HTTP server started");
    */
   
}


void loop()
{ 

}

