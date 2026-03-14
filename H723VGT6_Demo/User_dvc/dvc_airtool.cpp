/**
 * @file dvc_airvalve.cpp
 * @author Lucy
 * @brief 气阀控制
 * @version 0.1
 * @date 2025-03-23 0.1 25赛季定稿
 *
 * @copyright NEUQ-RoboPioneer (c) 2024-2025
 *
 */

/* Includes ------------------------------------------------------------------*/

#include "dvc_airtool.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/**
 * @brief 气阀初始化
 * 
 * @param __GPIO_Port_Open 开启气阀的GPIO端口
 * @param __GPIO_Pin_Open 开启气阀的GPIO引脚
 * @param __GPIO_Port_Close 关闭气阀的GPIO端口
 * @param __GPIO_Pin_Close 关闭气阀的GPIO引脚
 */
void Class_AIRVALVE::Init(GPIO_TypeDef *__GPIO_Port_Open, uint16_t __GPIO_Pin_Open, GPIO_TypeDef *__GPIO_Port_Close, uint16_t __GPIO_Pin_Close)
{
    // GPIO端口配置
    GPIO_Port_Open = __GPIO_Port_Open;

    GPIO_Port_Close = __GPIO_Port_Close;

    // 初始化GPIO  
    GPIO_Pin_Open = __GPIO_Pin_Open;

    GPIO_Pin_Close = __GPIO_Pin_Close;

    // 初始化气阀状态
    AIRVALVE_Close();
}

/**
 * @brief 气阀打开
 *
 */
void Class_AIRVALVE::AIRVALVE_Open()
{
    // 打开气阀
    HAL_GPIO_WritePin(GPIO_Port_Open, GPIO_Pin_Open, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIO_Port_Close, GPIO_Pin_Close, GPIO_PIN_RESET);
    // 更新气阀状态
    AIRVALVE_Status = AIRVALVE_Status_Open;
}

/**
 * @brief 气阀关闭
 *
 */
void Class_AIRVALVE::AIRVALVE_Close()
{
    // 关闭气阀
    HAL_GPIO_WritePin(GPIO_Port_Open, GPIO_Pin_Open, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIO_Port_Close, GPIO_Pin_Close, GPIO_PIN_SET);
    // 更新气阀状态
    AIRVALVE_Status = AIRVALVE_Status_Close;
}

/**
 * @brief 气泵初始化
 * 
 * @param __GPIO_Port_Open 开启气泵的GPIO端口
 * @param __GPIO_Pin_Open 开启气泵的GPIO引脚
 */
void Class_AIRPUMP::Init(GPIO_TypeDef *__GPIO_Port_Open, uint16_t __GPIO_Pin_Open)
{
    // GPIO端口配置
    GPIO_Port_Open = __GPIO_Port_Open;

    // 初始化GPIO  
    GPIO_Pin_Open = __GPIO_Pin_Open;

    // 初始化气泵状态
    AIRPUMP_Close();
}

/**
 * @brief 气泵打开
 *
 */
void Class_AIRPUMP::AIRPUMP_Open()
{
    // 打开气泵
    HAL_GPIO_WritePin(GPIO_Port_Open, GPIO_Pin_Open, GPIO_PIN_SET);
    // 更新气泵状态
    AIRPUMP_Status = AIRPUMP_Status_Open;
}

/**
 * @brief 气泵关闭
 *
 */
void Class_AIRPUMP::AIRPUMP_Close()
{
    // 关闭气泵
    HAL_GPIO_WritePin(GPIO_Port_Open, GPIO_Pin_Open, GPIO_PIN_RESET);
    // 更新气泵状态
    AIRPUMP_Status = AIRPUMP_Status_Close;
}

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
