#ifndef __OPR_H
#define __OPR_H
//这里负责操作的落实
#define _ENABLE 1
#define _DISABLE 0



struct oprElem{
    short index=0;
    oprElem* next=nullptr;
    void init(short ind){this->index=ind;}
    //0无效操作 1上 2下 3左 4右 5确认 6forcestop 7reset 8res1 9res2 10res3 11res4 12res5
    short readClass();
    //0无效值 1上下左右与确定 2应急操作 3功能操作
    static const short classMAP[13];
};
struct oprQueue{
    oprElem *head=nullptr;
    oprElem *end=nullptr;
    bool clear();
    bool pushNew(short ind);
    bool isHead();
    bool isHead(short type);
    bool pop();
    bool popTill();
    bool popTill(short type);
    bool popTillNormal();
    short popRead();
    short headType();
    short headIndex();
};
extern oprQueue oprQUE;


//操作区-键盘操作
const int INTKeyUpF1Pin=1;
const int INTKeyDownF2Pin=2;
const int INTKeyLeftF3Pin=3;
const int INTKeyRightF4Pin=4;
const int INTKeyConfF5Pin=5;
const int INTKeyEmerPin=6;
const int KeyShiftPin=7;
const int INTKeyPins[]={INTKeyUpF1Pin,INTKeyDownF2Pin,INTKeyLeftF3Pin,INTKeyRightF4Pin,
                        INTKeyConfF5Pin,INTKeyEmerPin};
//好心疼
void INTKeyConf();
void cmdINTKey(bool);

void KeyHandler(void);


#endif
