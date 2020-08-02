2020年新工科联盟-Xilinx暑期学校（Summer School）项目
=========
摇摇乐（陀螺仪+AWS）
==================
介绍 
-----
### 项目概要
* 本项目基于SEA-S7开发平台，利用板载陀螺仪读取姿态数据，通过ESP32的WIFI联网，由ESP32上传到AWS的云端服务器节点上，在服务器端进行显示进行摇摇乐的实现，呈现直观的计步小程序。

### 工具版本
* 【Vivado2018.3】（https://china.xilinx.com) 

  【Arduino 1.8.13】（https://www.arduino.cc/en/Main/Software）

### 板卡型号 
* xc7s15ftgb196-1
    
### 设计步骤 
* 解压工程文件，使用**vivado2018.3**进行综合实现，编译生成Gyro_Demo_Verilog.bit文件

* 板载的陀螺仪通过IIC与陀螺仪驱动模块传递原始的陀螺仪姿态数据。

* 驱动模块将原始数据经过处理后存于RAM，利用RAM与QSPI模块，Esp32可以通过QSPI接口访问RAM中的数据。

* **Arduino**程序编写，对开发板进行烧写，使用串口监视器进行陀螺仪数据观测，编写数据处理及计数模块

* Esp32连接**AWS IoT**，使用MQTT订阅主题观察数据情况，将板载陀螺仪数据上传至IoT云端。
* 利用IoT云端 Web API，从AWS平台获取数据并处理，用于呈现直观的计步器小程序。

    
