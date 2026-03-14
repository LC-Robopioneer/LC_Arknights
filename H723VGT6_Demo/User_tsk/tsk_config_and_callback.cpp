/**
 * @file tsk_config_and_callback.cpp
 * @author yssickjgd (1345578933@qq.com)
 * @brief 临时任务调度测试用函数, 后续用来存放个人定义的回调函数以及若干任务
 * @version 0.1
 * @date 2023-08-29 0.1 23赛季定稿
 * @date 2023-01-17 1.1 调试到机器人层
 *
 * @copyright USTC-RoboWalker (c) 2023-2024
 *
 */

/**
 * @brief 注意, 每个类的对象分为专属对象Specialized, 同类可复用对象Reusable以及通用对象Generic
 *
 * 专属对象:
 * 单对单来独打独
 * 比如交互类的底盘对象, 只需要交互对象调用且全局只有一个, 这样看来, 底盘就是交互类的专属对象
 * 这种对象直接封装在上层类里面, 初始化在上层类里面, 调用在上层类里面
 *
 * 同类可复用对象:
 * 各调各的
 * 比如电机的对象, 底盘可以调用, 云台可以调用, 而两者调用的是不同的对象, 这种就是同类可复用对象
 * 电机的pid对象也算同类可复用对象, 它们都在底盘类里初始化
 * 这种对象直接封装在上层类里面, 初始化在最近的一个上层专属对象的类里面, 调用在上层类里面
 *
 * 通用对象:
 * 多个调用同一个
 * 比如底盘陀螺仪对象, 底盘类要调用它做小陀螺, 云台要调用它做方位感知, 因此底盘陀螺仪是通用对象.
 * 这种对象以指针形式进行指定, 初始化在包含所有调用它的上层的类里面, 调用在上层类里面
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "tsk_config_and_callback.h"
#include "dvc_dwt.h"
#include "drv_can.h"
#include "drv_tim.h"
#include "drv_bsp.h"
#include "ita_robot.h"
#include "dvc_power_dji_a.h"

/* Private macros ------------------------------------------------------------*/
//static float vbat = 0;
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
Class_Chariot chariot;
// 全局初始化完成标志位
bool init_finished = false;
uint32_t flag = 0;

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief FDCAN1回调函数
 *
 * @param FDCAN_RxMessage FDCAN1收到的消息
 */
void Device_FDCAN1_Callback(Struct_FDCAN_Rx_Buffer *FDCAN_RxMessage)
{

    switch (FDCAN_RxMessage->Header.Identifier)
    {
        


        //M2006 电机数据反馈
        case 0x201:
        {
          chariot.Chassis.Motor_Steer[0].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
        case 0x202:
        {
          chariot.Chassis.Motor_Steer[1].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
        case 0x203:
        {
          chariot.Chassis.Motor_Steer[2].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
        case 0x204:
        {
          chariot.Chassis.Motor_Steer[3].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
        default:

        break;
    }
}

/**
 * @brief FDCAN2回调函数
 *
 * @param FDCAN_RxMessage FDCAN2收到的消息  
 */
void Device_FDCAN2_Callback(Struct_FDCAN_Rx_Buffer *FDCAN_RxMessage)
{
    switch (FDCAN_RxMessage->Header.Identifier)
    {
				//Vesc 电调数据反馈
        case 0x931:
        case 0x1031:
        {
          chariot.Chassis.Motor_Wheel[0].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        } 
        case 0x932:
        case 0x1032:
        {
          chariot.Chassis.Motor_Wheel[1].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
        case 0x933:
        case 0x1033:
        {
          chariot.Chassis.Motor_Wheel[2].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
        case 0x934:
        case 0x1034:
        {
          chariot.Chassis.Motor_Wheel[3].FDCAN_RxCpltCallback(FDCAN_RxMessage->Data);
          break;
        }
    }
}

/**
 * @brief FDCAN3回调函数,此函数为本杰明电调通信用，FDCAN配置为扩展ID
 *
 * @param FDCAN_RxMessage FDCAN3收到的消息
 */
void Device_FDCAN3_Callback(Struct_FDCAN_Rx_Buffer *FDCAN_RxMessage)
{
    switch (FDCAN_RxMessage->Header.Identifier)
    {
		// 舵向编码器返回数据处理
    case(0x001):
    {

        break;
    }
    case(0x003):
    {

        break;
    }
    case(0x004):
    {

        break;
    }
    // 舵向电机返回数据处理
    case (0x201):
    {

        break;
    }
    case (0x203):
    {

        break;
    }
    case (0x204):
    {

        break;
    }
			
    case (0x931):
		case (0x1031):
    {

        break;
    }
    case (0x933):
		case (0x1033):
    {

        break;
    }
    case (0x934):
		case (0x1034):
    {

        break;
    }
    }
}

/**
 * @brief UART1		OPS9
 *
 * @param Buffer UART1收到的消息
 * @param Length 长度
 */
void OPS9_UART1_Callback(uint8_t *Buffer, uint16_t Length)
{
}


/**
 * @brief UART5遥控器回调函数
 *
 * @param Buffer UART5收到的消息
 * @param Length 长度
 */
void DR16_UART5_Callback(uint8_t *Buffer, uint16_t Length)
{
	chariot.DR16.DR16_UART_RxCpltCallback(Buffer);
	//底盘 云台 发射机构 的控制策略
	chariot.TIM_Control_Callback();
}

/**
 * @brief UART7串口绘图回调函数
 *
 * @param Buffer UART7收到的消息
 * @param Length 长度
 */
void Serialplot_UART7_Callback(uint8_t *Buffer, uint16_t Length)
{
  //   电机调PID
	

		
}

/**
 * @brief UART8		待用回调函数
 *
 * @param Buffer UART8收到的消息
 * @param Length 长度
 */
void Free_UART8_Callback(uint8_t *Buffer, uint16_t Length)
{
   //todo
}

/**
 * @brief UART9		待用回调函数
 *
 * @param Buffer UART9收到的消息
 * @param Length 长度
 */
void Free_UART9_Callback(uint8_t *Buffer, uint16_t Length)
{
	//todo
}

/**
 * @brief UART10	Orin通信
 *
 * @param Buffer UART10收到的消息
 * @param Length 长度
 */
void Orin_UART10_Callback(uint8_t *Buffer, uint16_t Length)    //测试串口有没有接收到数据，断点设这里面
{
}


/**
 * @brief GPIO外部中断回调函数组
 * 用于处理舵向电机光电门PMT45的外部中断回调
 */

/**
 * @brief GPIO0外部中断回调函数
 * 处理0号舵向电机PMT45的外部中断
 */
void GPIO_EXTI10_Callback()
{		
		chariot.Chassis.Steer_PMT45[0].PMT45_EXTI_Callback_Steer();
}

/**
 * @brief GPIO1外部中断回调函数
 * 处理1号舵向电机PMT45的外部中断
 */
void GPIO_EXTI11_Callback()
{	
	
		chariot.Chassis.Steer_PMT45[1].PMT45_EXTI_Callback_Steer();
}
/**
 * @brief GPIO2外部中断回调函数
 * 处理2号舵向电机PMT45的外部中断
 */
void GPIO_EXTI12_Callback()
{	
    chariot.Chassis.Steer_PMT45[2].PMT45_EXTI_Callback_Steer();
}
/**
 * @brief GPIO3外部中断回调函数
 * 处理3号舵向电机PMT45的外部中断
 */
void GPIO_EXTI14_Callback()
{		
    chariot.Chassis.Steer_PMT45[3].PMT45_EXTI_Callback_Steer();
}
/**
 * @brief TIM4任务回调函数
 *
 */
uint8_t PC10,PC11,PC12,PE14;
void Task100us_TIM4_Callback()
{
		//todo
		PC10 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10);
		PC11 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11);
		PC12 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12);
		PE14 = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14);
}

/**
 * @brief TIM5任务回调函数
 *
 */
uint8_t mod100= 0;
bool flag_power=false;
CAN_PowerController can_power_controller;
uint8_t is_success[4]={0};

void Task1ms_TIM5_Callback()
{
		DWT_Update();
    flag++;

    if(!flag_power)
    {
       flag_power = true;
       is_success[0]=can_power_controller.turn_on_power(1);
      // is_success[1]=can_power_controller.turn_on_power(2);
      // is_success[2]=can_power_controller.turn_on_power(3);
      // is_success[3]=can_power_controller.turn_on_power(4);
    }
       mod100++;
       if(mod100 >= 100)
     {
        mod100 = 0;  
     }
}
/**
 * @brief 初始化任务
 *
 */
void Task_Init()
{
    // 驱动层初始化
    DWT_Init();
    // 点俩灯, 开24V
		BSP_Init(BSP_DC24_L_OFF|BSP_DC24_R_OFF|BSP_DC5_ON, 0.0, 0.0);
    // CAN总线初始化    
		FDCAN_Init(&hfdcan1, Device_FDCAN1_Callback);
		FDCAN_Init(&hfdcan2, Device_FDCAN2_Callback);
        FDCAN_Init(&hfdcan3, Device_FDCAN3_Callback);
	
    // UART初始化
		UART_Init(&huart5,DR16_UART5_Callback,36);
    // 定时器初始化
    TIM_Init(&htim4, Task100us_TIM4_Callback);
    TIM_Init(&htim5, Task1ms_TIM5_Callback);

		// 外部中断初始化(舵轮光电门校准)
    GPIO_EXTI_Init(GPIO_PIN_10, GPIO_EXTI10_Callback);
    GPIO_EXTI_Init(GPIO_PIN_11, GPIO_EXTI11_Callback);
    GPIO_EXTI_Init(GPIO_PIN_12, GPIO_EXTI12_Callback);
    GPIO_EXTI_Init(GPIO_PIN_14, GPIO_EXTI14_Callback);
		
    // 设备层初始化
    
    // 战车层初始化

    // 交互层初始化

    // 机器人战车初始化

    // 使能调度时钟
		HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim5);
    // 标记初始化完成
    init_finished = true;

}

/**
 * @brief 前台循环任务
 *
 */
void Task_Loop()
{

}

/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
