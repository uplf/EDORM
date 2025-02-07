#include "dataOper.h"
#include "util.h"


short (*operateMap[])()={NULL,&FBIOpenOperator,&LightOffOperator,&DeviceRelinkOperator,
                            &EtherOnOperator,&AlertOffOperator,&forceSTOPOperator,
                            &discEtherOperator,&discDeviceOperator,&discWiFiOperator};
short (*operateReqMap[])(AsyncWebServerRequest*)={NULL,&defpermOperator,&editThemeOperator,&eraseUsersOperator,
                                                    &handleMasterCmd};
String(*operateStrMap[])(AsyncWebServerRequest*)={NULL,&alterAdminOperator,&generateUserOperator};


communitorStatus cmtStatus;
masterStatus mstStatus;
messagerStatus msgStatus;

const IPAddress MASTER_ip(192, 168, 1, 100);

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
bool cmdStatus::pickVaildDef(int i){
    if(i>=0&&i<5){
        this->cmd=i;
        return true;
    }return false;

}
short cmdStatus::handleCMD(){
    Serial.write("handling cmd",this->cmd);
    return 0;
}
String valStatus::valueToJsonString(String unit=""){
    return String(this->value)+unit;
}




String buildJsonDataResponse_Device(){
    
    refreshMasterData();

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


short handleOperate(short cmd){
    return (*operateMap[cmd])();
}
short handleOperate(short cmd,AsyncWebServerRequest *request){
    return (*operateReqMap[cmd])(request);
}
String handleOperate(AsyncWebServerRequest* request,short cmd){
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

short EtherOnOperator(){
    ETHInit();
    if(Ethernet.localIP()==IPAddress(0,0,0,0))return 10;
    else return 0;
}
short AlertOffOperator(){return 0;}
short forceSTOPOperator(){return 0;}







short defpermOperator(AsyncWebServerRequest* request){
    for(short i=0;i<=7;i++){
        prefs.putInt(prefMAP[i].c_str(),PERstringToCode(request->getParam(perMap[i])->value()));
    }
    return 0;
}
short editThemeOperator(AsyncWebServerRequest *request){
    String jsonData = request->getParam("json")->value();  // 获取查询参数 "json"
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonData);
    if(error)return 7;
    String newTheme=doc["theme"];
    prefs.putString("theme",newTheme);
    return 0;
}

short eraseUsersOperator(AsyncWebServerRequest *request){
    String curAdmin=prefs_user.getString("ADMIN");
    if(request->getHeader("Authorization")->value()!=curAdmin)return 8;
    prefs_user.clear();
    prefs_user.putString("ADMIN",curAdmin);
    prefs_user.putInt(curAdmin.c_str(),2);
    return 0;
}
short handleMasterCmd(AsyncWebServerRequest* request){
    String jsonData = request->getParam("json")->value();  // 获取查询参数 "json"
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonData);
    if(error)return 7;
    int cmdType=doc["cmdType"];
    //1状态命令 2功能命令 3设置命令
    int cmd=doc["cmd"];
    switch(cmdType){
        case 1:return(mstStatus.cmd.pickVaildDef(cmd))?mstStatus.cmd.handleCMD():7;
        case 2:return handleOperate(cmd);
        case 3:{
            //

            return 0;
        } 
        default:return 7;
    }

}


String alterAdminOperator(AsyncWebServerRequest *request){
    String curAdmin=prefs_user.getString("ADMIN");
    if(request->getHeader("Authorization")->value()!=curAdmin)return "forbidden";
    String newAdmin=randomString(6);
    prefs_user.putString("ADMIN",newAdmin);
    prefs_user.remove(curAdmin.c_str());
    prefs_user.putInt(newAdmin.c_str(),2);

    StaticJsonDocument<64> jsonStatus;
    jsonStatus["status"]="done";
    jsonStatus["adminName"]=newAdmin;
    String respJSON;
    serializeJson(jsonStatus,respJSON);
    return respJSON;
}
String generateUserOperator(AsyncWebServerRequest* request){
    String curAdmin=prefs_user.getString("ADMIN");
    if(request->getHeader("Authorization")->value()!=curAdmin)return "forbidden";
    String newUser=randomString(6);
    prefs_user.putInt(newUser.c_str(),1);

    StaticJsonDocument<64> jsonStatus;
    jsonStatus["status"]="done";
    jsonStatus["newUserName"]=newUser;
    String respJSON;
    serializeJson(jsonStatus,respJSON);
    return respJSON;
}


void FBIAsyncHandler(void* parameter){
    cmtStatus.FBI.status=1;
    analogWrite(FBI_PIN,FBI_OPEN_ANALOG);
    delay(1000);
    analogWrite(FBI_PIN,0);
    cmtStatus.FBI.status=0;
    vTaskDelete(NULL);  // 任务结束
}
void LightAsyncHandler(void* parameter){
    cmtStatus.light.status=1;
    analogWrite(LIGHT_PIN,LIGHT_OFF_ANALOG);
    delay(1000);
    analogWrite(LIGHT_PIN,0);
    cmtStatus.light.status=0;
    vTaskDelete(NULL);  // 任务结束
}
void WiFiRelinkAsyncHandler(void*){
    delay(1000);
    server.end();
    WiFi.disconnect(true);
    //重新连接蓝牙操作

    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    vTaskDelete(NULL);  // 任务结束
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
String randomString(int length){
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // 字母 + 数字
  String randomStr = "";
  
  for (int i = 0; i < length; i++) {
    int index = random(0, sizeof(charset) - 1);  // 从字符集中随机选择一个字符
    randomStr += charset[index];  // 添加字符到字符串
  }

  return randomStr;
}
int refreshMasterData(){
        //这里是有关连接设备的操作,申请成功
    try{
        if(!Ping.ping(MASTER_ip))throw 3;
        String masterREQ;
        if((HTTPreqMasterToString("/API/data",&masterREQ)/100!=2))throw 2;
        StaticJsonDocument<200> MasterDatas;
        DeserializationError deserError = deserializeJson(MasterDatas, masterREQ);
        if(deserError)throw 1;
        JsonObject masterData=MasterDatas["master"];
        mstStatus.status=masterData["status"];
        //传过来注意，需要json从头j到尾
        JsonObject alarmTime=masterData["alarm"];
        mstStatus.alarm.hour=alarmTime["hour"];
        mstStatus.alarm.min=alarmTime["min"];
        mstStatus.alarm.sec=alarmTime["sec"];

        mstStatus.cmd=masterData["cmd"];
        mstStatus.debug=masterData["debug"];

        JsonObject messagerData=MasterDatas["messager"];
        msgStatus.status=messagerData["status"];
        msgStatus.humidity=messagerData["humidity"];
        msgStatus.temperature=messagerData["temperature"];
        msgStatus.wind_speed=messagerData["wind_speed"];
        return 0;
    }catch(int err){
        mstStatus.status.status=3;
        mstStatus.cmd.cmd=4;
        mstStatus.debug.status=3;
        return err;
    }
}

