# EDORM 设计蓝图

## 概述

EDORM是一个宿舍物联网系统，可以完成一些简单的功能。具体有哪些功能呢，请看特征，不过在此之前请参考用到的词是什么意思。

### 用词

#### 设备

- ` 处理器` 参与控制的设备。
- `系统设备` 代码中有固定体现的所有设备。
- `迷你服务器` 处理器中带有服务器功能的设备。
- `终端` 手机电脑等作为客户端使用的设备。
- `commutor`  `master` 处理器设备名称，详见下文。
- `舍友` `设计用户` 北三910的人。
- `互联网` 校园网。

##### 蓝图

- `@` 指还在设想。

### 特征

#### 物联网思想

利用蓝牙、WiFi、以太网完成互联网-迷你服务器-终端之间的通讯和控制。

其中处理器有commutor和master，它们介绍如下：

- commutor(ESP32)
  1. 连接以太网和蓝牙并作为WiFi热点。
  2. 操作类设备的中枢和公共服务的接入口，也负责互联网访问。
- master(ESP32-S3)
  1. 连接WiFi并作为蓝牙主机(错开是为了减少蓝牙调整对WiFi的影响，不知道这有没有道理)
  2. 桌面服务和维护命令接入口、传感器数据传入操作中枢的中间节点。
- 其他(留给感兴趣的舍友)
  1. 连接蓝牙，若希望配置web服务器则可以连接WiFi。
  2. 可以参考已有功能做出拓展。

不同通讯的任务包括：

- WiFi连接
  1. 由commutor发起，共有4个设备配额，其中，所有带有web服务器的设备接入WiFi，其他配额给到终端（一个以上）。
  2. 负责少参数的指令传输（处理器-处理器、处理器-终端）、数据收集设备master的数据传输、处理器API请求与回复的转达、Web信息的处理与重定向。
- 蓝牙连接
  1. 由master发起，共有4个设备配额，一般只供非终端设备连接。
  2. 负责数据到master的传输。
- 以太网
  1. commutor连接到校园网，可以访问互联网上的服务，对它的访问需要以commutor作为中转。
  2. 负责系统设备和终端对互联网API的访问，@和与互联网相连的服务器对本系统的访问与控制。

> [!NOTE]
>
> WiFi的AP与STA模式的配置、蓝牙配置、利用W5500访问以太网、ETHClient的应用。



#### Web服务器

迷你服务器设备通过Web服务器的配置与终端通信，为舍友类用户提供操作界面、数据呈现、命令收集。

搭载web服务器的的设备包括：

- commutor(ESP32)

  主要功能的Web服务器、重定向等任务。

- master(ESP32-S3)

  本身有些“桌面端”应用（见后），可以作为前述应用的Web服务器、向commutor发送配置命令。

- 其他(留给感兴趣的舍友)

> [!note]
>
> 异步Web服务器的配置（与统一化事件处理）、HTTP请求与相应、JSON数据的构造与解析、html页面的设计和JS的逻辑、网页SPIFFS存储、preference的应用。





