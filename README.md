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
* Espressif ESP32-D0WDQ6

### 芯片型号
* xc7s15ftgb196-1
    
### 设计步骤 
* 解压工程文件，使用**vivado2018.3**进行综合实现，编译生成Gyro_Demo_Verilog.bit文件

* 板载的陀螺仪通过IIC与陀螺仪驱动模块传递原始的陀螺仪姿态数据。

* 驱动模块将原始数据经过处理后存于RAM，利用RAM与QSPI模块，Esp32可以通过QSPI接口访问RAM中的数据。

* **Arduino**程序编写，对开发板进行烧写，使用串口监视器进行陀螺仪数据观测，编写数据处理及计数模块

* Esp32连接**AWS IoT**，使用MQTT订阅主题观察数据情况，将板载陀螺仪数据上传至IoT云端。
* 利用IoT云端 Web API，从AWS平台获取数据并处理，用于呈现直观的计步器小程序。
### 项目框图
![](https://github.com/yyl-2/2020--Xilinx-Summer-School-/raw/master/Images/block.png)

### 设计特色
* 摇摇乐的设计类似手机的计步器，大多数的计步器使用加速度数据，基于阈值来检测步伐，检测技术，不论是硬件还是软件，都不能满足高精度的定位系统，尤其是在缓慢步行的情况下。低速行走时，重力加速度几
乎为固定值，而且加速度计反应迟缓，再加上这些算法不能采用分级的阈值。因而此次项目设计我们设想采用陀螺仪来计数，并且使用降噪滤波法，尽量减小误差，这样能够在室内定位中识别出人类步行状态，相对于
重力加速度更加精确。

### 设想未来应用方向
* 计步准确是非常关键的一项指标，有必要提高其准确性和可靠性，提出一种智能手机基于单点陀螺计步器为人群提供计步定位系统而发展出来的一个功能部分。通过测试不同的活动和不同的步行速度，能得到满意的
结果，即使在非常缓慢的步行速度，步数检测仍具有非常高的精度。不管是平地还是斜坡、楼梯，基于陀螺仪的计步器都能容易用作室内定位系统和导航系统的精准计步功能。




    
