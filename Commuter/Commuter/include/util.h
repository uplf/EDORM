
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
#define OPRKEY 13
#define FBI_PIN 0
#define FBI_OPEN_ANALOG 128
#define LIGHT_PIN 2
#define LIGHT_OFF_ANALOG 128

// 定义 I2S 引脚
#define I2S_DOUT      25  // 数据输出引脚
#define I2S_BCLK      27  // 位时钟引脚
#define I2S_LRC       26  // 左右时钟引脚



//eth
#include <SPI.h>
#include <soc/spi_pins.h>

#define DEBUG_ETHERNET_GENERIC_PORT         Serial
// Debug Level from 0 to 4
#define _ETG_LOGLEVEL_                      2
// Default to use W5100. Must change to false for W5500, W5100S, for faster SPI clock

// #define USE_W5100                           false
#define ETHERNET_USE_ESP32
#define BOARD_TYPE      "ESP32"
#define W5500_RST_PORT   21

#define USING_SPI2              false
#define PIN_MISO          MISO
#define PIN_MOSI          MOSI
#define PIN_SCK           SCK
#define PIN_SS            SS

#define SHIELD_TYPE       "W5x00 using Ethernet_Generic Library on SPI"

#ifndef USE_THIS_SS_PIN
#define USE_THIS_SS_PIN   15   //22    // For ESP32
#endif

///////////////////////////////////////////////////////////

// W5100 chips can have up to 4 sockets.  W5200 & W5500 can have up to 8 sockets.
// Use EthernetLarge feature, Larger buffers, but reduced number of simultaneous connections/sockets (MAX_SOCK_NUM == 2)
#define ETHERNET_LARGE_BUFFERS

//////////////////////////////////////////////////////////

 //#include "Ethernet_Generic.h"

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20

extern byte mac[][NUMBER_OF_MAC];

// Select the IP address according to your local network
extern IPAddress ip;

// Google DNS Server IP
extern  IPAddress myDns;






#endif