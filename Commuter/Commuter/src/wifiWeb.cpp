#include "wifiWeb.h"


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
    request->send(200,"application/json",jsoncontent);
}

String errorCodeToMsg[]={"Success","fail to obtain state","Unknown","reserved abstract error",
"operation locked","device disconnected","device occupied"
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
    int perParam=perName=="ADMIN"?2:readOrCreate(perName,2);
    if(!perParam)return true;
    if(!request->hasHeader("Authorization"))return false;
    String name=(request->getHeader("Authorization"))->value();
    const char* nameCharA= name.c_str();
    if(!prefs_user.isKey(nameCharA))return false;
    return (prefs_user.getInt(nameCharA)<prefs.getInt(perName))? false:true;
}

