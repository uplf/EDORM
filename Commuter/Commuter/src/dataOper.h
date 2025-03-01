#ifndef __DATA_OPER_H
#define __DATA_OPER_H

#include "bleCon.h"
#include "wifiWeb.h"
#include "prefer.h"


#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <AudioFileSourceSPIFFS.h>
// 音频组件
extern AudioFileSourceSPIFFS *playfile;
extern AudioGeneratorMP3 *mp3;
extern AudioOutputI2S *out;





String buildJsonStatusResponse(short,String);
String buildJsonDataResponse_Device();
String buildJsonDataResponse_Permission();




struct timeStatus{
    timeStatus(){this->hour=0;this->min=0;this->sec=0;}
    timeStatus(short hr, short mi, short sc):hour(hr),min(mi),sec(sc){}
    short hour=0;
    short min=0;
    short sec=0;
    String timeToJsonString();

};

struct deviceStatus{
    short status=0;
    String deviceStatusToJsonString();
};

struct funcStatus{
    short status=1;
    String funcStatusToJsonString();
    short stateCodeToErrorCode();
};

struct cmdStatus{
    short cmd=1;
    String cmdStatusToJsonString();
    bool pickVaildDef(int);
    short handleCMD();
};
struct valStatus{
    float value=1;
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



void I2S_Init();
void key_Init();

short handleOperate(short);
short handleOperate(short,AsyncWebServerRequest *);
String handleOperate(AsyncWebServerRequest*,short);

short FBIOpenOperator();
short LightOffOperator();
short DeviceRelinkOperator();
short EtherOnOperator();
short AlertOffOperator();
short forceSTOPOperator();
short discEtherOperator();
short discDeviceOperator();
short discWiFiOperator();
short playMusicOperator();

//异步操作函数区
void FBIAsyncHandler(void*);
void LightAsyncHandler(void*);
void WiFiRelinkAsyncHandler(void*);
// FreeRTOS任务句柄
extern TaskHandle_t playTaskHandle;
void MusicAsyncHandler(void*);



short defpermOperator(AsyncWebServerRequest *);
short editThemeOperator(AsyncWebServerRequest *);
short eraseUsersOperator(AsyncWebServerRequest *);
short handleMasterCmd(AsyncWebServerRequest*);
short editMusicUrlOperator(AsyncWebServerRequest *);

String alterAdminOperator(AsyncWebServerRequest *);
String generateUserOperator(AsyncWebServerRequest*);

//其他中间函数
String codeToString(int);
int PERstringToCode(String);

String randomString(int length=6);
int refreshMasterData();




#endif
