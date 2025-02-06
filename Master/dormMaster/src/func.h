#ifndef __FUNC_H
#define __FUNC_H
#include "ArduinoJson.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//这里负责菜单结构和显示结构
#include "proc.h"

#define _ENABLE 1
#define _DISABLE 0

void showEmptyPreview();
struct funcElem{
    funcElem* parent=nullptr;
    funcElem* child=nullptr;
    short layer=0;
    funcElem* front=nullptr;
    funcElem* rear=nullptr;
    short parInd=0;
    short parSum=0;
    short (*action)(void)=nullptr;
    void (*preDisp)(void)=&showEmptyPreview;
    String name;

    //void defElem(short layer,short parInd,short parSum,funcElem* front,funcElem* rear,
    //            funcElem* child,funcElem* parent,String name);
    //void killRearSon();
    //short defPeerStruct(short parSum,funcElem* parent);
    short defPeerStruct(short parSum,funcElem* parent,String names[],short (*actions[])(void),void (*preDisps[])(void));
    short createChild(funcElem* toElem);
    short locatePeer(short target,funcElem* toElem);
    short locateNeibPeer(short dir,funcElem* toElem);//1表示向右，其他表示向左
    short locateAncDes(short ind,funcElem* toElem);//又是一个自创单词，专门表示不同辈，ancestor of descendant 0表示1的子节点，正数表示向上回溯的级数
    funcElem* oprCodeToMove(short ind);
};
struct funcTree{
    funcElem* root=nullptr;
};
extern funcTree funcTREE;

short func_init();
funcElem *curElem;



struct TFTdisp{
    public:
    LiquidCrystal_I2C tft;
    bool activate=false;
    void setTFT(bool);
    void resetTFT();
    static short lastLayer;
    void refTFT(funcElem);//refresh
    private:
    void drawlayer(short);
    void drawindex(short);
    

};
extern TFTdisp CurTFT;
void initCurTFT();

struct timeStatus{
    timeStatus(){this->hour=0;this->min=0;this->sec=0;}
    timeStatus(short hr, short mi, short sc):hour(hr),min(mi),sec(sc){}
    short hour=0;
    short min=0;
    short sec=0;
    String timeToJsonString();
};
struct AlertStatus:public timeStatus{
    short activation=0;
    short sound=1;

    short setAlertHour();
    short setAlertMin();
    short setAlertSec();
    short setAlertAct();
    short setAlertSound();
};
extern AlertStatus Alert;

#endif
