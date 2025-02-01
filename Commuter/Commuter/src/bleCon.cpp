#include "bleCon.h"



BLECharacteristic *pCharacteristic; //创建一个BLE特性pCharacteristic
bool deviceConnected = false;       //连接否标志位
uint8_t txValue = 0;                //TX的值
long lastMsg = 0;                   //存放时间的变量
String rxload = "BlackWalnutLabs";  //RX的预置值

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//服务器回调

void MyServerCallbacks::onConnect(BLEServer *pServer){
    deviceConnected = true;
}
void MyServerCallbacks::onDisconnect(BLEServer *pServer){
    deviceConnected = false;
}


//特性回调
void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic){
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0)
    {
      rxload = "";
      for (int i = 0; i < rxValue.length(); i++)
      {
        rxload += (char)rxValue[i];
        Serial.print(rxValue[i]);
      }
      Serial.println("");
    }
}


void setupBLE(String BLEName)
{
    return; //temp
  const char *ble_name = BLEName.c_str(); //将传入的BLE的名字转换为指针
  BLEDevice::init(ble_name);              //初始化一个蓝牙设备

  BLEServer *pServer = BLEDevice::createServer(); // 创建一个蓝牙服务器
  pServer->setCallbacks(new MyServerCallbacks()); //服务器回调函数设置为MyServerCallbacks

  BLEService *pService = pServer->createService(SERVICE_UUID); //创建一个BLE服务

  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY); 
//创建一个(读)特征值 类型是通知
  pCharacteristic->addDescriptor(new BLE2902());
//为特征添加一个描述

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
 //创建一个(写)特征 类型是写入
  pCharacteristic->setCallbacks(new MyCallbacks());
//为特征添加一个回调

  pService->start();                  //开启服务
  pServer->getAdvertising()->start(); //服务器开始广播
  Serial.println("Waiting a client connection to notify...");
}
void setup()
{

}

void loop()
{
  long now = millis(); //记录当前时间
  if (now - lastMsg > 1000)
  { //每隔1秒发一次信号
    if (deviceConnected && rxload.length() > 0)
    {
      String str = rxload;
      if (str=="10086\r\n")
      {
        const char *newValue = str.c_str();
        pCharacteristic->setValue(newValue);
        pCharacteristic->notify();
      }
    }
    lastMsg = now; //刷新上一次发送数据的时间
  }
}