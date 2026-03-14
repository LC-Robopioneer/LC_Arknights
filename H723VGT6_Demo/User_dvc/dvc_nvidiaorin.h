/**
 * @file dvc_nvidiaorin.h
 * @author Lucy (2478427315@qq.com)
 * @brief orin上位机通信
 * @version 0.1
 * @date 2025-03-01
 *
 * @copyright ROBOPIONEER (c) 2024
 *
 */


 #ifndef DVC_NVIDIAORIN_H
 #define DVC_NVIDIAORIN_H
 
 /* Includes ------------------------------------------------------------------*/
 
 #include "stm32h7xx_hal.h"
 #include "drv_uart.h"
 #include "drv_math.h"
 
 /* Exported macros -----------------------------------------------------------*/
 
 /* Exported types ------------------------------------------------------------*/
 
 /**
  * @brief Orin状态  enum类型四字节
  *
  */
 enum Enum_Orin_Status
 {
     Orin_Status_DISABLE = 0,
     Orin_Status_ENABLE,
 };
 
 /**
  * @brief Orin给控制板的源数据
  *
  */
 struct Struct_Orin_UART_Rx_Data
 {
     uint16_t Frame_Header;
     uint8_t Data_Size;
     float Target_Velocity_X;
     float Target_Velocity_Y;
     float Target_Omega;
	float Yaw_Target_Omega;
	float Pitch_Target_Omega;
     uint8_t Flag_Todo1;
     uint8_t Flag_Todo2;
     uint8_t Flag_Todo3;
     uint8_t Flag_Todo4;
     uint8_t Flag_Todo5;
     uint8_t Flag_Todo6;
     uint8_t CRC_8; 
     uint16_t Frame_Rear;
 }__attribute__((packed));
 
 
 /**
  * @brief 控制板给Orin的源数据
  *
  */
 struct Struct_Orin_UART_Tx_Data
 {
     uint16_t Frame_Header;  //2
     uint8_t Data_Size;      //1
     
     float Now_Velocity_X;   //4
     float Now_Velocity_Y;   //4
     float Now_Omega;    //4
 
     //未来可能会改为枚举类型
     uint8_t Flag_Todo1;     //1
     uint8_t Flag_Todo2;     //1
     uint8_t Flag_Todo3;     //1
     uint8_t Flag_Todo4;     //1
     uint8_t Flag_Todo5;     //1
     uint8_t Flag_Todo6;     //1
 
     uint8_t CRC_8;          //1
     uint16_t Frame_Rear;    //2
 }__attribute__((packed));   
 
 /**
  * @brief Orin类
  *
  */
 class Class_Orin
 {
     public:
                 // 注意这里帧头和帧尾的顺序是反的
         void Init(UART_HandleTypeDef *__huart, uint16_t __Frame_Header = 0xaa55, uint16_t __Frame_Rear = 0x0a0d);
 
         inline Enum_Orin_Status Get_Status();
 
         inline float Get_Target_Velocity_X();
 
         inline float Get_Target_Velocity_Y();
 
         inline float Get_Target_Omega();
	
         inline float Get_Pitch_Target_Omega();
	
         inline float Get_Yaw_Target_Omega();

         inline void Set_Now_Velocity_X(float __Velocity_X);
 
         inline void Set_Now_Velocity_Y(float __Velocity_Y);
 
         inline void Set_Now_Omega(float __Angle_Yaw);
 
         void UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length);
 
         void TIM_1000ms_Alive_PeriodElapsedCallback();
 
         void TIM_10ms_Send_PeriodElapsedCallback();
 
     protected:
     // 初始化相关常量
 
     // 绑定的UART
     Struct_UART_Manage_Object *UART_Manage_Object;
 
     // 数据包头标
     uint16_t Frame_Header;
     // 数据包尾标
     uint16_t Frame_Rear;
     // 发送数据包有效数据长度
     uint8_t Tx_Data_Size = 26;
     // 接收数据包有效数据长度
     uint8_t Rx_Data_Size = 26;
         // 发送数据包长度
         uint8_t Tx_Size = 32;
         // 接收数据包长度
         uint8_t Rx_Size = 32;
 
     // 常量
 
     // 内部变量
 
     // 当前时刻的视觉Orin接收flag
     uint32_t Flag = 0;
     // 前一时刻的视觉Orin接收flag
     uint32_t Pre_Flag = 0;
 
     // 读变量
 
     // Orin状态
     Enum_Orin_Status Orin_Status = Orin_Status_DISABLE;
     // 视觉Manifold对外接口信息
     Struct_Orin_UART_Rx_Data Rx_Data;
 
     // 写变量
 
     // 视觉Manifold对外接口信息
     Struct_Orin_UART_Tx_Data Tx_Data;
 
     // 读写变量
 
     // 内部函数
 
     void Data_Process(uint16_t Length);
 
     void Output();
 
     uint8_t Verify_CRC_8(uint8_t *Message, uint32_t Length);
 
 };
 
 /* Exported variables --------------------------------------------------------*/
 
 /* Exported function declarations --------------------------------------------*/
 
 /**
  * @brief 获取Orin在线状态
  * @return Enum_Orin_Status Orin连接状态
  */
 inline Enum_Orin_Status Class_Orin::Get_Status()
 {
     return Orin_Status;
 }
 
 /**
  * @brief 获取目标当前X轴速度
  * @return float X轴速度 (m/s)
  */
 inline float Class_Orin::Get_Target_Velocity_X()
 {
     return Rx_Data.Target_Velocity_X;
 }
 
 /**
  * @brief 获取目标当前Y轴速度
  * @return float Y轴速度 (m/s)
  */
 inline float Class_Orin::Get_Target_Velocity_Y()
 {
     return Rx_Data.Target_Velocity_Y;
 }
 
 /**
  * @brief 获取目标当前偏航角
  * @return float 偏航角 (度)
  */
 inline float Class_Orin::Get_Target_Omega()
 {
     return Rx_Data.Target_Omega;
 }
 
  /**
  * @brief 获取目标当前偏航角
  * @return float 偏航角 (度)
  */
 inline float Class_Orin::Get_Pitch_Target_Omega()
 {
     return Rx_Data.Pitch_Target_Omega;
 }
 
  /**
  * @brief 获取目标当前偏航角
  * @return float 偏航角 (度)
  */
 inline float Class_Orin::Get_Yaw_Target_Omega()
 {
     return Rx_Data.Yaw_Target_Omega;
 }
 
 /**
  * @brief 设置当前X轴速度
  * @param __Velocity_X X轴速度 (m/s)
  */
 inline void Class_Orin::Set_Now_Velocity_X(float __Velocity_X)
 {
     Tx_Data.Now_Velocity_X = __Velocity_X;
 }
 
 /**
  * @brief 设置当前Y轴速度
  * @param __Velocity_Y Y轴速度 (m/s)
  */
 inline void Class_Orin::Set_Now_Velocity_Y(float __Velocity_Y)
 {
     Tx_Data.Now_Velocity_Y = __Velocity_Y;
 }
 
 /**
  * @brief 设置当前偏航角
  * @param __Angle_Yaw 偏航角 (度)
  */
 inline void Class_Orin::Set_Now_Omega(float __Now_Omega)
 {
     Tx_Data.Now_Omega = __Now_Omega;
 }
 
 #endif
 
 /************************ COPYRIGHT(C) ROBOPIONEER **************************/

