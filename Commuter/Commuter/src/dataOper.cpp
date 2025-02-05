#include "dataOper.h"
#include "util.h"


short (*operateMap[])()={NULL,&FBIOpenOperator,&LightOffOperator,&DeviceRelinkOperator,
                            &EtherOnOperator,&AlertOffOperator,&forceSTOPOperator,
                            &discEtherOperator,&discDeviceOperator,&discWiFiOperator};
short (*operateReqMap[])(AsyncWebServerRequest*)={NULL,&defpermOperator,&editThemeOperator,&eraseUsersOperator};
String(*operateStrMap[])(AsyncWebServerRequest*)={NULL,&alterAdminOperator,&generateUserOperator};


communitorStatus cmtStatus;
masterStatus mstStatus;
messagerStatus msgStatus;

String timeStatus::timeToJsonString(){
    return String(this->hour)+":"+String(this->min)+":"+String(this->sec);
}
String deviceStatus::deviceStatusToJsonString(){
        switch(this->status){
        case 0:return "normal";
        case 1:return "warning";
        case 2:return "error";
        case 3:return "disconnect";
    }
    return "inner-error";
}
String funcStatus::funcStatusToJsonString(){
    switch(this->status){
        case 0:return "ready";
        case 1:return "busy";
        case 2:return "prohibited";
        case 3:return "error";
    }
    return "inner-error";
}
short funcStatus::stateCodeToErrorCode(){
        switch(this->status){
        case 0:return 0;
        case 1:return 6;
        case 2:return 4;
        case 3:return 3;
        default:return 2;
    }
}
String cmdStatus::cmdStatusToJsonString(){
    switch(this->cmd){
        case 0:return "ready";
        case 1:return "alarm";
        case 2:return "forceSTOP";
        case 3:return "debug";
        case 4:return "disconnect";
    }
    return "inner-error";
}
String valStatus::valueToJsonString(String unit=""){
    return String(this->value)+unit;
}




String buildJsonDataResponse_Device(){
    StaticJsonDocument<512> jsonStatus;

    
    JsonObject communitor=jsonStatus.createNestedObject("communitor");
    communitor["ether"]=cmtStatus.ether.funcStatusToJsonString();
    communitor["FBI"]=cmtStatus.FBI.funcStatusToJsonString();
    communitor["light"]=cmtStatus.light.funcStatusToJsonString();
    communitor["status"]=cmtStatus.status.deviceStatusToJsonString();

    JsonObject master=jsonStatus.createNestedObject("master");
    master["status"]=mstStatus.status.deviceStatusToJsonString();
    master["alarm"]=mstStatus.alarm.timeToJsonString();
    master["cmd"]=mstStatus.cmd.cmdStatusToJsonString();
    master["debug"]=mstStatus.status.deviceStatusToJsonString();

    JsonObject messager=jsonStatus.createNestedObject("messager");
    messager["temperature"] = msgStatus.temperature.valueToJsonString();
    messager["humidity"] = msgStatus.humidity.valueToJsonString();
    messager["wind_speed"] = msgStatus.wind_speed.valueToJsonString();
    messager["status"] = msgStatus.status.deviceStatusToJsonString();

    jsonStatus["theme"]=readOrCreate("theme",(String)"welcome");

    String jsonString;
    serializeJson(jsonStatus,jsonString);
    return jsonString;
}

String perMap[]={"door","light","alarm","serial","api","ethernet","device","FORCEStop"};
String prefMAP[]={"FBI","light","AlertOff","debug","request","EtherOn","DeviceRelink","FORCEStop"};

String buildJsonDataResponse_Permission(){
    StaticJsonDocument<256> jsonStatus;
    jsonStatus["theme"]=readOrCreate("theme",(String)"");
    for(short i=0;i<=7;i++)jsonStatus[perMap[i]]=codeToString(readOrCreate((prefMAP[i]).c_str(),2));
    String jsonString;
    serializeJson(jsonStatus,jsonString);
    return jsonString;
}


short handleOperatie(short cmd){
    return (*operateMap[cmd])();
}
short handleOperatie(short cmd,AsyncWebServerRequest *request){
    return (*operateReqMap[cmd])(request);
}
String handleOperatie(AsyncWebServerRequest* request,short cmd){
    return (*operateStrMap[cmd])(request);
}

short FBIOpenOperator(){
    Serial.write("FBI_Func");
    if(!cmtStatus.FBI.status){
        xTaskCreate(FBIAsyncHandler,"FBIOpenTheDoor",2048,NULL,1,NULL);
        return 0;
    }else return cmtStatus.FBI.stateCodeToErrorCode();
}
short LightOffOperator(){
    Serial.write("LIGHTOff_Func");
    if(!cmtStatus.light.status){
        xTaskCreate(LightAsyncHandler,"LightOffTheDoor",2048,NULL,1,NULL);
        return 0;
    }else return cmtStatus.FBI.stateCodeToErrorCode();
}

short DeviceRelinkOperator(){
    xTaskCreate(WiFiRelinkAsyncHandler,"DeviceRelink",2048,NULL,1,NULL);
    return 0;
}

short EtherOnOperator(){return 0;}
short AlertOffOperator(){return 0;}
short forceSTOPOperator(){return 0;}







short defpermOperator(AsyncWebServerRequest* request){
    for(short i=0;i<=7;i++){
        prefs.putInt(prefMAP[i].c_str(),PERstringToCode(request->getParam(perMap[i])->value()));
    }
    return 0;
}
short editThemeOperator(AsyncWebServerRequest *){return 0;}
short eraseUsersOperator(AsyncWebServerRequest *){return 0;}


String alterAdminOperator(AsyncWebServerRequest *){return "to be done";}
String generateUserOperator(AsyncWebServerRequest*){return "to be done";}


void FBIAsyncHandler(void* parameter){
    cmtStatus.FBI.status=1;
    analogWrite(FBI_PIN,FBI_OPEN_ANALOG);
    delay(1000);
    analogWrite(FBI_PIN,0);
    cmtStatus.FBI.status=0;
}
void LightAsyncHandler(void* parameter){
    cmtStatus.light.status=1;
    analogWrite(LIGHT_PIN,LIGHT_OFF_ANALOG);
    delay(1000);
    analogWrite(LIGHT_PIN,0);
    cmtStatus.light.status=0;
}
void WiFiRelinkAsyncHandler(void*){
    delay(1000);
    server.end();
    WiFi.disconnect(true);
    //重新连接蓝牙操作

    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
}



String codeToString(int code){
    switch(code){
        case 0:return "guest";
        case 1:return "internal";
        case 2:return "admin";
        default:return "error";
    }
}
int PERstringToCode(String userGroup){
    if(userGroup=="guest")return 0;
    if(userGroup=="internal")return 1;
    if(userGroup=="admin")return 2;
    return -1;

}


