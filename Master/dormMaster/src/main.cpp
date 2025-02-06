//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "opr.h"
#include "func.h"


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, world!");

}

void loop()
{

}

void init(){
  INTKeyConf();
  cmdINTKey(_ENABLE);
  initCurTFT();
}
void operation(){
  if(!oprQUE.popTill())return;
  //重置指针
  curElem=funcTREE.root;
  CurTFT.setTFT(true);
  CurTFT.resetTFT();
  CurTFT.activate=true;
  CurTFT.refTFT(*curElem);
  while(CurTFT.activate){
    while(!oprQUE.popTill()){}
    switch(oprQUE.headType()){
      case 1:{
        curElem=curElem->oprCodeToMove(oprQUE.popRead());
        CurTFT.refTFT(*curElem);
        break;
      }
      case 2:{

      }
    }
  }
  

}