#ifndef __UTIL_H
#define __UTIL_H

#include "dataOper.h"
#define SCANNER_CYCLE 1000
#define AUTO_DEVICE_SCANNER false



extern const char* ssid;
extern const char* password;

extern IPAddress local_ip;
extern IPAddress gateway;
extern IPAddress subnet;

#define FBI_PIN 10
#define FBI_OPEN_ANALOG 128
#define LIGHT_PIN 11
#define LIGHT_OFF_ANALOG 128

extern communitorStatus cmtStatus;
extern masterStatus mstStatus;
extern messagerStatus msgStatus;



#endif