#include "func.h"

funcTree funcTREE;
AlertStatus Alert;
funcElem curElem;
TFTdisp CurTFT;

short func_init_11_3(funcElem* tmp){
    if(tmp==nullptr)return 2;
    String names_ly3Func[]={"setHour","setMin","setSec","setAct","setSound"};
    short (*ly2Func[])(void)={&setAlertHour,&setAlertMin,&setAlertSec,&setAlertAct,&setAlertSound};
    void (*ly2Disp[])(void)={&Alertpreview,&Alertpreview,&Alertpreview,&Alertpreview,&Alertpreview};
    return tmp->defPeerStruct(5,tmp->parent,names_ly3Func,ly2Func,ly2Disp);
}
short func_init1_1(funcElem* tmp){
    //定义同辈
    if(tmp==nullptr)return 2;
    String names_ly2Func[]={"FBI","light","alert","music"};
    short (*ly2Func[])(void)={&SendFuncFBI,&SendFuncLight,nullptr,&playMusic};
    void (*ly2Disp[])(void)={&FBIpreview,&Lightpreview,&Alertpreview,&Musicpreview};
    if(tmp->defPeerStruct(4,tmp->parent,names_ly2Func,ly2Func,ly2Disp))return 1;
    //定义下一层
    funcElem* tmp2,* tmp3;
    if(tmp->locatePeer(3,tmp2)||tmp2->createChild(tmp3))return 2;
    return func_init_11_3(tmp3);
}
short func_init2(funcElem* tmp){
    //定义同辈
    if(tmp==nullptr)return 2;
    String names_ly2Func[]={"WiFi","webServer","Bluetooth"};
    short (*ly2Func[])(void)={&setWiFi,&setServer,&setBLU};
    void (*ly2Disp[])(void)={&WiFipreview,&Serverpreview,&BLUpreview};
    return (tmp->defPeerStruct(3,tmp->parent,names_ly2Func,ly2Func,ly2Disp));
}
short func_init_13_2(funcElem* tmp){
    if(tmp==nullptr)return 2;
    String names_ly3Func[]={"setHour","setMin","setSec"};
    short (*ly2Func[])(void)={&setTimeHour,&setTimeMin,&setTimeSec};
    void (*ly2Disp[])(void)={&Timepreview,&Timepreview,&Timepreview};
    return tmp->defPeerStruct(3,tmp->parent,names_ly3Func,ly2Func,ly2Disp);
}
short func_init3(funcElem* tmp){
    //定义同辈
    if(tmp==nullptr)return 2;
    String names_ly2Func[]={"setOpreation","setTime","applyTime"};
    short (*ly2Func[])(void)={&setOpr,nullptr,&setCLUTime};
    void (*ly2Disp[])(void)={&Oprpreview,&Timepreview,nullptr};
    return (tmp->defPeerStruct(3,tmp->parent,names_ly2Func,ly2Func,ly2Disp));
        //定义下一层
    funcElem* tmp2,* tmp3;
    if(tmp->locatePeer(2,tmp2)||tmp2->createChild(tmp3))return 2;
    return func_init_13_2(tmp3);
}


short func_init(){
    funcTREE.root=new funcElem;
    funcElem* tmp=funcTREE.root,*tmp2,*tmp3;
    //1.功能 2.互联 3.设置
    short (*ly1Func[])(void)={nullptr,nullptr,nullptr};
    void (*ly1Disp[])(void)={nullptr,nullptr,showBuildingprevew};
    String names[]={"functions","connections","settings"};
    if(funcTREE.root->defPeerStruct(3,nullptr,names,ly1Func,ly1Disp))return 1;
    if(tmp->locatePeer(1,tmp2)||tmp2->createChild(tmp3))return 1;
    if(func_init1_1(tmp3))return 2;
    if(tmp->locatePeer(2,tmp2)||tmp2->createChild(tmp3))return 1;
    if(func_init2(tmp3))return 2;
    return 0;
}



short funcElem::locatePeer(short target,funcElem* toElem){
    if(target<=0||target>this->parSum)return 2;
    toElem=this;
    while(target!=toElem->parInd)toElem=(target>toElem->parInd)?toElem->front:toElem->rear;
    return 0;
}
short funcElem::locateNeibPeer(short dir,funcElem* toElem){
    if(dir==1)toElem=this->rear;
    else toElem=this->front;
}//1表示向右，其他表示向左
short funcElem::locateAncDes(short ind,funcElem* toElem){
    if(!ind){
        toElem=this->child;
        return (toElem==nullptr)?2:0;
    }
    for(;ind>0;ind--){
        toElem=this->parent;
        if(toElem==nullptr)return 2;
    }
    return 0;
}
funcElem* funcElem::oprCodeToMove(short ind ){
    if(ind==1)return this->front;
    if(ind==2)return this->rear;
    if(ind==3&&this->parent!=nullptr)return this->parent; 
    if(ind==4&&this->child!=nullptr)return this->child;
    if(ind==5&&this->action!=nullptr)this->action();
    return this;
}
short funcElem::createChild(funcElem* toElem){
    toElem=new funcElem;
    if(toElem)return 1;
    this->child=toElem;
    toElem->parent=this;
    return 0;
}
short funcElem::defPeerStruct(short parSum,funcElem* parent,String names[],short (*actions[])(void),void (*preDisps[])(void)){
    if(this==nullptr)return 1;
    this->name=names[0];
    this->action=actions[0];
    this->preDisp=(preDisps[0]==nullptr)?&showEmptyPreview:preDisps[0];
    funcElem*tmp=this;
    for(short cur=1;cur<parSum;cur++){
        tmp->rear=new funcElem;
        if(!tmp->rear)return 2;
        tmp->rear->front=tmp;
        tmp=tmp->rear;
        tmp->name=names[cur];
        tmp->action=actions[cur];
    }
    tmp->rear=this;
    this->front=tmp;
}

