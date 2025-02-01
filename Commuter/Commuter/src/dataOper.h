#ifndef __DATA_OPER_H
#define __DATA_OPER_H

#include "bleCon.h"
#include "wifiWeb.h"





String buildJsonStatusResponse(short,String);
String buildJsonDataResponse_Device();
String buildJsonDataResponse_Permission();




struct timeStatus{
    timeStatus(short hr, short mi, short sc):hour(hr),min(mi),sec(sc){}
    short hour;
    short min;
    short sec;
    String timeToJsonString();
};

struct deviceStatus{
    short status;
    String deviceStatusToJsonString();
};

struct funcStatus{
    short status;
    String funcStatusToJsonString();
};

struct cmdStatus{
    short cmd;
    String cmdStatusToJsonString();
};
struct valStatus{
    float value;
    String valueToJsonString(String);
};


struct communitorStatus{
    public:
    funcStatus ether;//1
    funcStatus FBI;//2
    funcStatus light;//3
    deviceStatus status;//0
    String indexToString(short,short);
};
struct masterStatus{
    public:

    deviceStatus status; //0
    funcStatus debug;    //1
    cmdStatus cmd;      //2
    timeStatus alarm;
    String indexToString(short,short);
};
struct messagerStatus{
    valStatus temperature;
    valStatus humidity;
    valStatus wind_speed;
    deviceStatus status;
    String indexToString(short,float);
};
extern communitorStatus cmtStatus;
extern masterStatus mstStatus;
extern messagerStatus msgStatus;



short handleOperatie(short);

short FBIOpenOperator();
short LightOffOperator();
short WifiOffOperator();
short EtherOffOperator();
short AlertOffOperator();
short forceSTOPOperator();


//调试区，这里最难





#endif
