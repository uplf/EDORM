
#include <Arduino.h>
#include "wifiWeb.h"
#include "prefer.h"
#include "dataOper.h"
#include "util.h"

const char* ssid     = "哈哈你没网";
const char* password = "yeyeyeye";

IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);


short taskScanner(int* param);
bool FBIEnable=false,LightEnable=false;



void setup()
{

    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);


    prefInit();

    delay(100);

    server.onNotFound([](AsyncWebServerRequest *request){sendNotFound(request);});
    //handler of login-page
    server.on("/",HTTP_GET, 
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/login.html",request);});//0
    //handler of index-page
    server.on("/index", HTTP_GET,
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/index.html",request);});//0
    //handler of file-page
    server.on("/file", HTTP_GET,
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/file.html",request);});//0
    //handler of discription-page
    server.on("/discription", HTTP_GET,
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/discription.html",request);});//0
    //handler of debug-page with permission-auth
    server.on("/debug",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"debug"))sendSPIFFSFile("/debug.html",request);});//0enter
    //handler of request-page with permission-auth    
    server.on("/request",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"request"))sendSPIFFSFile("/request.html",request);});//0enter
    //handler of permission-page with permission-auth
    server.on("/permission",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"permission"))sendSPIFFSFile("/permission.html",request);});
    //handler of device-data-request
    server.on("/API/device",HTTP_POST,
    [](AsyncWebServerRequest *request){sendJsonToAPIClient(request,buildJsonDataResponse_Device());});

    server.on("/API/permission",HTTP_GET,
    [](AsyncWebServerRequest *request){sendJsonToAPIClient(request,buildJsonDataResponse_Permission());});

    server.on("/API/FBIOpen",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"FBI"))sendJsonResToAPIClient(request,handleOperatie(1));});

    server.on("/API/LightOFF",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"light"))sendJsonResToAPIClient(request,handleOperatie(2));});

    server.on("/API/WifiOff",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"WifiOff"))sendJsonResToAPIClient(request,handleOperatie(3));});

    server.on("/API/EtherOff",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"EtherOff"))sendJsonResToAPIClient(request,handleOperatie(4));});

    server.on("/API/AlertOFF",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"AlertOff"))sendJsonResToAPIClient(request,handleOperatie(5));});

    server.on("/API/FORCEStop",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"FORTHStop"))sendJsonResToAPIClient(request,handleOperatie(6));});
    
    server.on("/API/disconnectEther",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"ADMIN"))sendJsonResToAPIClient(request,handleOperatie(7));});
    
    server.on("/API/disconnectDevice",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"ADMIN"))sendJsonResToAPIClient(request,handleOperatie(8));});
    
    server.on("/API/disconnectWiFi",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"ADMIN"))sendJsonResToAPIClient(request,handleOperatie(9));});
    
    server.on("/API/generateUser",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"ADMIN"))sendJsonResToAPIClient(request,handleOperatie(10));});
    
    server.on("/API/alterAdmin",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"ADMIN"))sendJsonToAPIClient(request,handleOperatie(request,1));});
    
    server.on("/API/eraseUsers",HTTP_GET,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"ADMIN"))sendJsonToAPIClient(request,handleOperatie(request,2));});
    

    server.on("/permission/API",HTTP_PUT,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"permission"))sendJsonResToAPIClient(request,handleOperatie(1,request));});

    server.on("/API/editTheme",HTTP_PUT,
    [](AsyncWebServerRequest *request){if(permissionCheck(request,"permission"))sendJsonResToAPIClient(request,handleOperatie(1,request));});

    server.begin();
    Serial.println("HTTP server started");
}



void loop()
{


}

short taskScanner(int* param){

}