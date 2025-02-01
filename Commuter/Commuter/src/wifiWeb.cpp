#include "wifiWeb.h"


void handle_not_found(){
    server.send(404, "text/plain", "Not found");
}

void sendNotFound(){
    server.send(404, "text/plain", "File Not Found");
}
void sendNotFound(AsyncWebServerRequest *request){
    request->send(404,"text/plain","Not Found");
}
void sendInnerError(){
    server.send(500,"text/plain","Internal Server Error");
}
void sendInnerError(AsyncWebServerRequest *request){
    request->send(500,"text/plain","Internal Server Error");
}

void sendJsonToAPIClient(AsyncWebServerRequest *request,String jsoncontent){
    request->send(200,"application/json",jsoncontent);
}
void sendJsonResToAPIClient(AsyncWebServerRequest *request,short result){
    StaticJsonDocument<32> jsonStatus;
    jsonStatus["status"]=result?"failed":"done";
    int HTMLcode=result?500:200;
    if(result)jsonStatus["error"]="error code:"+(String)result;
    String respJSON;
    serializeJson(jsonStatus,respJSON);
    request->send(HTMLcode,"application/json",respJSON);
}




void sendSPIFFSFile(String path){
    if(!SPIFFS.begin(true)){
        sendInnerError();
        return;
    }    
    File file = SPIFFS.open(path, "r");
    if (!file) {
        sendNotFound();
        file.close();
        return;
    }
    
    String contentType = "text/html"; // 默认 HTML
    server.streamFile(file, contentType);
    file.close();

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
    int perParam=readOrCreate(perName,2);
    if(!perParam)return true;
    if(!request->hasHeader("Authorization"))return false;
    String name=(request->getHeader("Authorization"))->value();
    const char* nameCharA= name.c_str();
    if(!prefs.isKey(nameCharA))return false;
    return (prefs.getInt(nameCharA)>prefs.getInt(perName))? false:true;
}

