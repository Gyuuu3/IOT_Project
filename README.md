# ESP32 + 双STM32 多节点物联网控制系统

[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.x-blue)](https://docs.espressif.com/projects/esp-idf)
[![STM32](https://img.shields.io/badge/STM32-F103-green)](https://www.st.com)
[![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-orange)](https://www.freertos.org)
[![MQTT](https://img.shields.io/badge/Cloud-华为云IoT-red)](https://www.huaweicloud.com/product/iothub.html)

## 项目简介

本项目是一套**基于 FreeRTOS 的三节点分布式物联网原型系统**，搭建「**ESP32 网关 + STM32 传感器节点 + STM32 执行器节点**」多机架构。

实现功能闭环：

- 下位机 STM32 采集传感器数据、执行设备控制、上报按键事件
- 多设备通过 **UART 自定义帧协议** 与 ESP32 双向通信
- ESP32 负责协议转发、WiFi 联网、MQTT 上云，实现**云端远程控制 + 设备数据上报**

适合嵌入式多机通信、RTOS 任务调度、物联网网关开发学习。

## 系统架构

```
          UART通信
STM32传感器节点 ◄───────►
                          \
                           ESP32-S3(网关) ◄─── MQTT ─── 华为云IoT
                          /
STM32执行器节点 ◄───────►
```

- **ESP32-S3**：网关核心，负责协议解析、多设备调度、云端数据交互
- **STM32 传感器板**：温湿度、超声波测距、按键事件主动上报
- **STM32 执行器板**：LED、PWM 调光、电机、舵机设备控制

## 功能特性

### 1. STM32 传感器节点

- DHT11 温湿度采集
- HC-SR04 超声波距离测量
- 按键触发事件主动上报
- 支持单指令查询、全量数据上报

### 2. STM32 执行器节点

- LED 开关控制 / PWM 亮度调节
- 双电机正转、反转、转速调速、停止
- 双舵机角度控制、复位控制

### 3. ESP32 网关 & 云端功能

- 双 UART 独立管理两路 STM32 设备通信
- 自定义串口协议帧解析、数据组包
- MQTT 连接华为云 IoT 平台
- 云端 JSON 指令下发解析、设备状态云端上报

## 硬件引脚定义

### STM32 执行器板 (F103)

| 外设 | 引脚 | 说明 |
|------|------|------|
| LED1 | PA1 | 普通 GPIO 开关控制 |
| LED2 | PB0 | TIM PWM 亮度调节 |
| 电机1 | PA6 / PB12 / PB13 | PWM 调速 + 方向控制 |
| 电机2 | PA7 / PB14 / PB15 | PWM 调速 + 方向控制 |
| 舵机1 | PB8 | TIM PWM 角度控制 |
| 舵机2 | PB9 | TIM PWM 角度控制 |
| 串口通信 | PA2 / PA3 | USART2 与 ESP32 通信 |

### STM32 传感器板 (F103)

| 外设 | 引脚 | 说明 |
|------|------|------|
| DHT11 | PA8 | 单总线时序采集 |
| 超声波 | PB12 / PB13 | Trig / Echo 测距 |
| 按键 | PA0 | 事件触发上报 |
| 状态灯 | PA1 | 设备状态指示 |
| 串口通信 | PA2 / PA3 | USART2 与 ESP32 通信 |

## 项目目录结构

```
IOT_Project/
├── esp32_template/        # ESP32-S3 网关工程 (ESP-IDF)
│   ├── components/        # 配置、驱动、中间件(协议/缓存/日志)
│   ├── main/              # FreeRTOS 多任务：MQTT/串口/设备管理任务
│   └── Doc/               # 开发日志、问题记录
├── stm_actuator/          # STM32 执行器 FreeRTOS 工程
│   ├── Core/              # 业务任务逻辑
│   ├── Drivers/           # 电机/舵机/PWM 扩展驱动
│   ├── Middleware/        # 串口协议封装
│   └── FreeRTOS/          # 实时操作系统调度
└── stm_sensor/            # STM32 传感器 FreeRTOS 工程
    ├── Core/              # 采集任务、按键任务
    ├── Drivers/           # DHT11/超声波驱动
    ├── Middleware/        # 数据组包、协议解析
    └── FreeRTOS/          # 实时操作系统调度
```

## 自定义通信协议

项目自主设计**固定帧头帧尾串口协议**，实现多设备可靠通信：

- 帧格式：`AA + 设备ID + 命令码 + 数据域 + BB`
- 支持：设备寻址、命令下发、数据应答、事件上报
- 两端统一协议解析层，解耦业务与通信

## 云端通信格式

### 云端下发控制指令（JSON）

```json
{"content":"led 2 on 60"}
```

支持 LED、电机、舵机、传感器查询指令。

### 设备上报数据（JSON）

```json
{"type":"sensor","temp":25.10,"humid":71.00,"dist":69.84}
```

## 核心技术亮点

### 1. 多节点 RTOS 架构设计

三设备任务拆分、优先级管理、独立业务任务解耦，实现嵌入式项目分层。

### 2. 自主实现多机串口通信协议

实现网关统一调度、设备寻址、双向应答，理解嵌入式通信分层逻辑。

### 3. 底层驱动 BUG 定位与修复

- 解决 ESP32 `esp_intr_alloc` 中断源参数混淆问题，修复串口偶发不收数据、丢包隐患
- 优化 UART 接收架构，舍弃不稳定自建中断 + 双层缓冲区，改用 IDF 原生稳定驱动
- 解决项目开发中任务栈溢出、MQTT 指针异常宕机等典型嵌入式问题

### 4. 完整端到端物联网链路

外设采集 → 多机串口通信 → 网关协议转发 → MQTT 云端上报，链路完整闭环。

## 快速编译运行

### ESP32 端

1. 配置 WiFi、华为云 MQTT 参数
2. IDF 编译烧录：

```bash
idf.py set-target esp32s3
idf.py build flash monitor
```

### STM32 端

1. Keil 打开对应工程
2. 编译下载至执行器 / 传感器开发板
3. 交叉接线、共地供电即可组网运行

## 开发总结

本项目完整落地 **STM32 RTOS 开发、多机串口协议、ESP32 网关开发、MQTT 物联网上云** 。


## 作者

GitHub: [Gyuuu3](https://github.com/Gyuuu3)

## 许可证

仅用于个人学习、技术展示、校招项目答辩使用。
