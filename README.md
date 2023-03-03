# OffLineProgrammer
* 基于沁恒CH32V208的离线烧录器

## 完成部分
* 自带Keil生成的烧录算法文件(.FLM文件)解析
* GD25Q128存储烧录算法和编程文件
* 使用USB虚拟CDC和容量设备
* 文件系统Fatfs移植
* SWD烧录算法移植

## 未完成部分
* USB实现CMSIS-DAP
* USB不实现容量设备，转为Qt上位机合并FLM文件以及烧录程序，通过自定义USB设备，在把文件放到SPIFlash里
* 使用蓝牙远程调试(速度不够在考虑WiFi芯片)
* 屏幕显示