#ifndef __BLE_CON_H
#define __BLE_CON_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <String.h>






class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer);
  void onDisconnect(BLEServer *pServer);
};

//特性回调
class MyCallbacks : public BLECharacteristicCallbacks{
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif
