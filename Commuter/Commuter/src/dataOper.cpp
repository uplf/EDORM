#include "dataOper.h"

short (*operateMap[])()={NULL,&FBIOpenOperator,&LightOffOperator,&WifiOffOperator,
                            &EtherOffOperator,&AlertOffOperator,&forceSTOPOperator};


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
String buildJsonDataResponse_Permission();


short handleOperatie(short cmd){
    return (*operateMap[cmd])();
}

