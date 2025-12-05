# 对M3508电机的闭环驱动以及串口绘图项目的总结

## 一、控制流程

### 角度环、速度环控制 : 

__输入目标角度__   →  __角度环PID输出目标速度__ → __速度环PID输出目标电流__ → __通过CAN通信驱动M3508电机__ → **反馈报文给相应的PID **

## 二、代码理解

### 1、尝试使用C++进行类的封装

```c++
class Class_Motor_M3508
{
public:
    Class_Motor_M3508(uint16_t ID, CAN_HandleTypeDef *hcan); //构造函数

    Class_PID Speed_PID{15.0f, 0.0f, 0.1f, 0.01f, 16384.0f, 16384.0f}; // 速度环PID参数
    Class_PID Angle_PID{5.0f, 0.0f, 0.5f, 0.01f, 500.0f, 300.0f}; // 角度环PID参数

    void Set_Current(int16_t current);                            // 设置电机电流
    void Set_Speed(float target_speed);                          // 设置目标速度，使用速度环PID计算电流
    void Set_Angle(float target_angle);                          // 设置目标角度，使用角度环PID计算速度
    void Update_From_Feedback(Struct_CAN_Rx_Buffer *rx_buffer);   // 从反馈数据更新电机状态
    int16_t Get_Actual_Current() const { return actual_current; } // 获取实际电机电流
    uint16_t Get_Speed() const { return speed; }                  // 获取电机速度
    uint16_t Get_Angle() const { return angle; }                  // 获取电机角度
    uint8_t Get_Temperature() const { return temperature; }       // 获取电机温度
    uint16_t Get_ID() const { return motor_ID; }                  // 获取电机ID
	
private:
    uint16_t motor_ID; //电机序号（1~8）
    CAN_HandleTypeDef *can_handler; // CAN句柄
    uint8_t *tx_buffer = nullptr; //发送缓冲区指针
    uint8_t data_index; //数组元素序号

    int16_t actual_current = 0; // 实际电机电流
    uint16_t speed = 0;         // 电机速度
    uint16_t angle = 0;         // 电机角度
    uint8_t temperature = 0;    // 电机温度
};
```



### 2、对CAN通信的一些理解

#### 2.1 CAN通信架构

- 使用经典CAN (Classic CAN)

#### 2.2 核心数据结构

**CAN接收缓冲区结构体**:

```c
struct Struct_CAN_Rx_Buffer {
    CAN_RxHeaderTypeDef Header;  // CAN帧头信息
    uint8_t Data[8];             // 8字节数据缓冲区
};
```

**CAN管理对象结构体**:
```c
struct Struct_CAN_Manage_Object {
    CAN_HandleTypeDef *CAN_Handler;     // CAN句柄指针
    Struct_CAN_Rx_Buffer Rx_Buffer;     // 接收缓冲区
    CAN_Call_Back Callback_Function;    // 回调函数指针
};
```

#### 2.3 过滤器配置机制

过滤器采用掩码模式，支持标准帧(11位ID)和扩展帧(29位ID):

```c
void can_filter_mask_config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, 
                           uint32_t ID, uint32_t Mask_ID)
```

**Object_Para参数位定义**:
- 位[7:3]: 滤波器编号(0-27)
- 位[2]: FIFO选择(0=FIFO0, 1=FIFO1)  
- 位[1]: ID类型(0=标准帧, 1=扩展帧)
- 位[0]: 帧类型(0=数据帧, 1=遥控帧)

#### 2.4 通信流程

1. **初始化阶段**:
   - 配置CAN硬件参数
   - 设置过滤器掩码
   - 启动CAN外设
   - 激活接收中断

2. **数据发送**:
   ```c
   uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, 
                        uint8_t *Data, uint16_t Length)
   ```

3. **数据接收**:
   - 通过中断回调函数接收数据
   - FIFO0/FIFO1分别对应不同的中断处理
   - 调用用户定义的回调函数处理接收数据

#### 2.5 扩展帧支持

支持29位扩展帧的构造和发送:

```c
// 构造扩展帧ID
uint32_t EXT_ID_Set(uint8_t equipment_id, uint16_t data2, uint8_t cmd_id);

// 发送扩展帧数据
uint8_t CAN_Send_EXT_Data(CAN_HandleTypeDef *hcan, uint32_t ID, 
                         uint8_t *Data, uint16_t Length)
```

#### 2.6 多总线支持

项目支持多路CAN总线:
- CAN1: 主要用于M3508电机控制
- CAN2: 备用总线或其他设备通信

每路总线都有独立的发送缓冲区和接收处理机制。

#### 2.7 定时发送机制

提供1ms定时器回调函数，用于周期性发送控制指令:

```c
void TIM_1ms_CAN_PeriodElapsedCallback()
```

可在此函数中实现多路电机的同步控制发送。













