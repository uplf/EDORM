#include <Arduino.h>

#include "FS.h"
//#include "SPIFFS.h"  // 确保仍然调用 SPIFFS 对象


void sendFile(const char *path) {
    File file = SPIFFS.open(path, "r"); // 以只读模式打开文件
    if (!file) {
        Serial.println("无法打开文件");
        return;
    }

    Serial.println("开始发送文件...");

    while (file.available()) {
        Serial.write(file.read());  // 逐字节发送
    }

    Serial.println("\n文件发送完成");
    file.close();
}

void setup() {
    Serial.begin(115200); // 设置串口波特率
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS 挂载失败");
        return;
    }

    const char *filePath = "/test.txt";
    if (SPIFFS.exists(filePath)) {
        sendFile(filePath); // 发送文件
    } else {
        Serial.println("文件不存在");
    }
}

void loop() {
}
