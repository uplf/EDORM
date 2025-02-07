#include "wifiWeb.h"
//WiFi
const char* ssid     = "哈哈你没网";
const char* password = "yeyeyeye";

IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

const char* masterIP="192.168.1.100";
const char* masterURL="http://192.168.1.100:8080";
EthernetClient EthClient;
HTTPClient HttpClient;


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
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?request->redirect(masterURL):sendForbidden(request);});//0enter
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
    [](AsyncWebServerRequest *request){permissionCheck(request,"FBI")?sendJsonResToAPIClient(request,handleOperate(1)):sendForbidden(request);});

    server.on("/API/LightOFF",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"light")?sendJsonResToAPIClient(request,handleOperate(2)):sendForbidden(request);});

    server.on("/API/DeviceRelink",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"DeviceRelink")?sendJsonResToAPIClient(request,handleOperate(3)):sendForbidden(request);});

    server.on("/API/EtherOn",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"EtherOn")?sendJsonResToAPIClient(request,handleOperate(4)):sendForbidden(request);});

    server.on("/API/AlertOFF",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"AlertOff")?sendJsonResToAPIClient(request,handleOperate(5)):sendForbidden(request);});

    server.on("/API/FORCEStop",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"FORTHStop")?sendJsonResToAPIClient(request,handleOperate(6)):sendForbidden(request);});
    
    server.on("/API/disconnectEther",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperate(7)):sendForbidden(request);});
    
    server.on("/API/disconnectDevice",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperate(8)):sendForbidden(request);});
    
    server.on("/API/disconnectWiFi",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperate(9)):sendForbidden(request);});
    
    server.on("/API/generateUser",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonResToAPIClient(request,handleOperate(10)):sendForbidden(request);});
    
    server.on("/API/alterAdmin",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonToAPIClient(request,handleOperate(request,1)):sendForbidden(request);});
    
    server.on("/API/eraseUsers",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonToAPIClient(request,handleOperate(request,2)):sendForbidden(request);});
    
    server.on("/permission/API",HTTP_PUT,
    [](AsyncWebServerRequest *request){permissionCheck(request,"permission")?sendJsonResToAPIClient(request,handleOperate(1,request)):sendForbidden(request);});

    server.on("/API/editTheme",HTTP_PUT,
    [](AsyncWebServerRequest *request){permissionCheck(request,"permission")?sendJsonResToAPIClient(request,handleOperate(1,request)):sendForbidden(request);});

    server.on("/MSAPI/cmd",HTTP_PUT,
    [](AsyncWebServerRequest *request){permissionCheck(request,"MASTER")?sendJsonResToAPIClient(request,handleOperate(4,request)):sendForbidden(request);});
}

void sendNotFound(AsyncWebServerRequest *request){

    request->send(404,"text/plain","Not Found");
}

void sendInnerError(AsyncWebServerRequest *request){
    request->send(500,"text/plain","Internal Server Error");
}
void sendForbidden(AsyncWebServerRequest *request){
    request->send(403,"text/plain","Forbidden, Check your usergroup");
}


void sendJsonToAPIClient(AsyncWebServerRequest *request,String jsoncontent){
    if(jsoncontent=="error")return sendInnerError(request);
    if(jsoncontent=="forbidden")return sendForbidden(request);
    if(jsoncontent=="notfound")return sendNotFound(request);
    request->send(200,"application/json",jsoncontent);
}

String errorCodeToMsg[]={"Success","fail to obtain state","Unknown","reserved abstract error",
"operation locked","device disconnected","device occupied",
"invaild parameter","invaild user","reserved request error",
"failed to perform"
};
void sendJsonResToAPIClient(AsyncWebServerRequest *request,short result){
    StaticJsonDocument<64> jsonStatus;
    jsonStatus["status"]=result?"failed":"done";
    int HTMLcode=result?500:200;
    if(result){jsonStatus["error_code"]="error code:"+(String)result;
        jsonStatus["error"]=errorCodeToMsg[result];
    }
    String respJSON;
    serializeJson(jsonStatus,respJSON);
    request->send(HTMLcode,"application/json",respJSON);



    /*自动回复全部内容，不知道有没有错
            String body = "";
    // 读取请求体
    request->onData([&body](uint8_t *data, size_t len) {
        body = String((char*)data);
    });

    // 等待数据传输完毕
    request->onEnd([&body, result, request]() {
        StaticJsonDocument<512> jsonDoc;
        DeserializationError error = deserializeJson(jsonDoc, body);

        // 检查 JSON 是否解析成功
        if (error) {
            Serial.println("JSON 解析失败");
            request->send(400, "application/json", "{\"error\": \"Invalid JSON\"}");
            return;
        }

        // 创建一个新的 JSON 响应对象
        StaticJsonDocument<512> responseDoc;

        // **自动复制请求中的所有键值对**
        for (JsonPair kv : jsonDoc.as<JsonObject>()) {
            responseDoc[kv.key()] = kv.value();
        }

        // **添加 `status` 字段**
        responseDoc["status"] = result == 0 ? "success" : "error";

        // **序列化 JSON 并发送响应**
        String responseJson;
        serializeJson(responseDoc, responseJson);
        request->send(200, "application/json", responseJson);
    });
    */
}






void sendSPIFFSFile(String path,AsyncWebServerRequest *request){
    

    if(!SPIFFS.begin(true)){
    sendInnerError(request);
    return;
    }    
    File file = SPIFFS.open(path, "r");
    if (!file) {
        sendNotFound(request);
        file.close();
        return;
    }
    String contentType = "text/html"; // 默认 HTML
    request->send(SPIFFS,path); 
    file.close();

}

bool permissionCheck(AsyncWebServerRequest *request,const char* perName){
    Serial.write(request->url().c_str());
    
    int specialFlag=0;
    if(perName=="ADMIN")specialFlag=2;
    if (perName=="MASTER")specialFlag=3;
    int perParam=(specialFlag?specialFlag:readOrCreate(perName,2));

    if(!perParam)return true;
    if(!request->hasHeader("Authorization"))return false;
    String name=(request->getHeader("Authorization"))->value();
    const char* nameCharA= name.c_str();
    if(!prefs_user.isKey(nameCharA))return false;
    return (prefs_user.getInt(nameCharA)<prefs.getInt(perName))? false:true;
}

void WiFiBegin(){
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
}

void WiFiEnd(){
    WiFi.softAPdisconnect(true);
}


void ETHInit(){
  Ethernet.init (USE_THIS_SS_PIN);
  uint16_t index = millis() % NUMBER_OF_MAC;
  Ethernet.begin(mac[index]);
}


//http request
int HTTPreqToString(String URL,String *respString,int (HTTPClient::*REQ)()=&HTTPClient::GET){
    //避免同时使用
    HttpClient.begin(URL);
    int HTTPcode=(HttpClient.*REQ)();
    if(HTTPcode>0) *respString=HttpClient.getString();
    return HTTPcode;
}
int HTTPreqMasterToString(String relativeURL,String *respString,int (HTTPClient::*REQ)()=&HTTPClient::GET){
    relativeURL=masterURL+(relativeURL.charAt(0)=='/'?relativeURL:('/'+relativeURL));
    return HTTPreqToString(relativeURL,respString,REQ);
}