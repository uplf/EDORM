#ifndef __PROC_H
#define __PROC_H
//这里存放事件处理函数

//
    short SendFuncFBI(void);
    short SendFuncLight(void);
    short setAlertHour();
        short setAlertMin();
        short setAlertSec();
        short setAlertAct();
        short setAlertSound();
        short playMusic();
    short setWiFi();
    short setBLU();
    short setServer();

    short setOpr();
    short setTime();
        short setTimeHour();
        short setTimeMin();
        short setTimeSec();
    short setCLUTime();

void FBIpreview(void);
void Lightpreview(void);
void Alertpreview(void);
void Musicpreview(void);
void WiFipreview(void);
void BLUpreview(void);
void Oprpreview(void);
void Timepreview(void);

void Serverpreview(void);
void showBuildingprevew(void);







#endif
