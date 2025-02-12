#include "comm.h"
//WiFi
const char* ssid     = "哈哈你没网";
const char* password = "yeyeyeye";

IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

const char* commutorIP="192.168.4.1";
const char* commutorURL="http://192.168.4.1:80";
EthernetClient EthClient;
HTTPClient HttpClient;


void serverUrlConfig(){
    server.onNotFound([](AsyncWebServerRequest *request){sendNotFound(request);});
    //handler of login-page
    server.on("/",HTTP_GET, 
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/index.html",request);});//0
    //handler of index-page
    server.on("/operator", HTTP_GET,
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/operator.html",request);});//0
    //handler of file-page
    server.on("/debug", HTTP_GET,
    [](AsyncWebServerRequest *request){sendSPIFFSFile("/debug.html",request);});//0
    //handler of debug-page with permission-auth
    server.on("/back",HTTP_GET,
    [](AsyncWebServerRequest *request){request->redirect(commutorIP);});//0enter
    //obtain device data. from commutor
    server.on("/API/device",HTTP_POST,
    [](AsyncWebServerRequest *request){sendJsonToAPIClient(request,buildJsonDataResponse_Device());});
    //obtain device data. from this server
    server.on("/API/device",HTTP_POST,
    [](AsyncWebServerRequest *request){sendJsonToAPIClient(request,buildJsonDataResponse_Device());});
    //perform speci
    server.on("/API/operation",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonToAPIClient(request,handleOperate(request,1)):sendForbidden(request);});
    //obtain debug data
    server.on("/MSAPI/cmd",HTTP_PUT,
    [](AsyncWebServerRequest *request){permissionCheck(request,"MASTER")?sendJsonResToAPIClient(request,handleOperate(4,request)):sendForbidden(request);});
    //perform debug cmd
    server.on("/API/operation",HTTP_GET,
    [](AsyncWebServerRequest *request){permissionCheck(request,"ADMIN")?sendJsonToAPIClient(request,handleOperate(request,1)):sendForbidden(request);});
    

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