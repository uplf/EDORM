
#include <Arduino.h>
#include "wifiWeb.h"
#include "prefer.h"
#include "dataOper.h"
#include "util.h"
#include "HTTPClient.h"
#include "ETH.h"



const char* ssid     = "哈哈你没网";
const char* password = "yeyeyeye";

IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);


/*
short taskScanner(int* param);
bool FBIEnable=false,LightEnable=false;

void serverUrlConfig(){
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
    [](AsyncWebServerRequest *request){permissionCheck(request,"debug")?sendSPIFFSFile("/debug.html",request):sendForbidden(request);});//0enter
    //handler of request-page with permission-auth    
    server.on("/request",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"request")?sendSPIFFSFile("/request.html",request):sendForbidden(request);});//0enter
    //handler of permission-page with permission-auth
    server.on("/permission",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"permission")?sendSPIFFSFile("/permission.html",request):sendForbidden(request);});
    //handler of device-data-request
    server.on("/API/device",HTTP_POST,
    [](AsyncWebServerRequest *request){sendJsonToAPIClient(request,buildJsonDataResponse_Device());});

    server.on("/API/permission",HTTP_GET,
    [](AsyncWebServerRequest *request){sendJsonToAPIClient(request,buildJsonDataResponse_Permission());});

    server.on("/API/FBIOpen",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"FBI")?sendJsonResToAPIClient(request,handleOperatie(1)):sendForbidden(request);});

    server.on("/API/LightOFF",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"light")?sendJsonResToAPIClient(request,handleOperatie(2)):sendForbidden(request);});

    server.on("/API/DeviceRelink",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"DeviceRelink")?sendJsonResToAPIClient(request,handleOperatie(3)):sendForbidden(request);});

    server.on("/API/EtherOn",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"EtherOn")?sendJsonResToAPIClient(request,handleOperatie(4)):sendForbidden(request);});

    server.on("/API/AlertOFF",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"AlertOff")?sendJsonResToAPIClient(request,handleOperatie(5)):sendForbidden(request);});

    server.on("/API/FORCEStop",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"FORTHStop")?sendJsonResToAPIClient(request,handleOperatie(6)):sendForbidden(request);});
    
    server.on("/API/disconnectEther",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperatie(7)):sendForbidden(request);});
    
    server.on("/API/disconnectDevice",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperatie(8)):sendForbidden(request);});
    
    server.on("/API/disconnectWiFi",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperatie(9)):sendForbidden(request);});
    
    server.on("/API/generateUser",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperatie(10)):sendForbidden(request);});
    
    server.on("/API/alterAdmin",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonToAPIClient(request,handleOperatie(request,1)):sendForbidden(request);});
    
    server.on("/API/eraseUsers",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonToAPIClient(request,handleOperatie(request,2)):sendForbidden(request);});
    

    server.on("/permission/API",HTTP_PUT,
    [](AsyncWebServerRequest *request){permissionCheck(request,"permission")?sendJsonResToAPIClient(request,handleOperatie(1,request)):sendForbidden(request);});

    server.on("/API/editTheme",HTTP_PUT,
    [](AsyncWebServerRequest *request){permissionCheck(request,"permission")?sendJsonResToAPIClient(request,handleOperatie(1,request)):sendForbidden(request);});

}

*/
void setup()
{

    Serial.begin(115200);

    ETH.begin();

    while (ETH.localIP() == IPAddress(0, 0, 0, 0)) {
        delay(1000);
        Serial.write(".");
    }

    Serial.write("done");

    /*
    serverUrlConfig();
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    prefInit();

    delay(100);
    
    server.begin();
    Serial.println("HTTP server started");
    */
    
}

    // 创建 HTTP 客户端对象
HTTPClient http;

// 目标服务器 URL
String serverUrl = "http://example.com/api";  // 替换成你要访问的URL

void loop()
{

  
  // 创建 JSON 对象用于发送
  StaticJsonDocument<200> doc;
  doc["name"] = "John Doe";
  doc["age"] = 30;

  // 将 JSON 对象序列化为字符串
  String jsonData;
  serializeJson(doc, jsonData);
  
  // 设置 HTTP 请求头
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  // 发送 POST 请求，并附带 JSON 数据
  int httpCode = http.POST(jsonData);


}

short taskScanner(int* param){

}