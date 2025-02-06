#include "opr.h"
#include <Arduino.h>

const short oprElem::classMAP[13]={0,1,1,1,1,1,2,2,3,3,3,3,3};
short oprElem::readClass(){return (this->index<0||this->index>12)?0:oprElem::classMAP[this->index];}

bool oprQueue::clear(){
    if(this->head==nullptr)return true;
    oprElem *tmp=this->head->next;
    while(tmp!=nullptr){
        delete this->head;
        this->head=tmp;
        tmp=tmp->next;
    }
    delete this->head;
    this->head=nullptr;
    this->end=nullptr;
    return true;
}
bool oprQueue::pushNew(short ind){
    oprElem* tmp=new oprElem;
    if(!tmp)return false;
    if(this->head==nullptr){
        this->head=tmp;
        this->end=tmp;
        return true;
    }
    this->end->next=tmp;
    this->end=tmp;
    return true;
}
bool oprQueue::isHead(){
    return(this->head==nullptr||this->head->index==0)? false:true;
}
bool oprQueue::isHead(short type){
    return(this->head==nullptr||this->head->index!=type)? false:true;
}
bool oprQueue::pop(){
    if(this->head==nullptr)return false;
    oprElem* tmp=this->head->next;
    delete this->head;
    if(tmp==nullptr)this->end=nullptr;
    this->head=tmp;
    return true;
}
bool oprQueue::popTill(){
    while(!this->pop()){
        if(this->head!=nullptr&&this->head->index)return true;
    }
    return false;
}
bool oprQueue::popTill(short type){
    while(!this->isHead(type)){
        if(!this->pop())return false;
    }
    return true;
}
bool oprQueue::popTillNormal(){
    while(this->head!=nullptr&&this->head->index!=6){
        if(this->head->index==0)pop();
        else return true;
    }
    return false;
}
short oprQueue::popRead(){
    short x=this->headIndex();
    this->pop();
    return x;
}
short oprQueue::headType(){
    return this->isHead()?this->head->readClass():0;
}
short oprQueue::headIndex(){
    return this->isHead()?this->head->index:0;
}

oprQueue oprQUE;


//键盘区
void INTKeyConf(){
    for(int i=0;i<=5;i++)pinMode(INTKeyPins[i],INPUT);
    pinMode(KeyShiftPin,INPUT);
}

void cmdINTKey(bool cmd){
    if(cmd){
        for(int i=0;i<=5;i++)attachInterrupt(digitalPinToInterrupt(INTKeyPins[i]),KeyHandler,FALLING);
    }else {for(int i=0;i<=5;i++)detachInterrupt(digitalPinToInterrupt(INTKeyPins[i]));}
}

void IRAM_ATTR KeyHandler(){
    bool shift=digitalRead(KeyShiftPin)==LOW;
    if(digitalRead(INTKeyConfF5Pin)==LOW)oprQUE.pushNew((shift)?7:6);
    if(digitalRead(INTKeyUpF1Pin)==LOW)oprQUE.pushNew((shift)?8:1);
    if(digitalRead(INTKeyDownF2Pin)==LOW)oprQUE.pushNew((shift)?9:2);
    if(digitalRead(INTKeyLeftF3Pin)==LOW)oprQUE.pushNew((shift)?10:3);
    if(digitalRead(INTKeyRightF4Pin)==LOW)oprQUE.pushNew((shift)?11:4);
    if(digitalRead(INTKeyConfF5Pin)==LOW)oprQUE.pushNew((shift)?12:5);
}