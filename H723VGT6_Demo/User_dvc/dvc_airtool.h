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

#ifndef DVC_AIRVALVE_H
#define DVC_AIRVALVE_H

/* Includes ------------------------------------------------------------------*/

#include "drv_bsp.h"

/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 气阀状态
 *
 */
enum Enum_AIRVALVE_Status
{
    AIRVALVE_Status_None = 0,
    AIRVALVE_Status_Open,
    AIRVALVE_Status_Close,
};

/**
 * @brief Specialized, 气阀
 *
 */
class Class_AIRVALVE
{
public:
    void Init(GPIO_TypeDef *__GPIO_Port_Open, uint16_t __GPIO_Pin_Open, GPIO_TypeDef *__GPIO_Port_Close, uint16_t __GPIO_Pin_Close);

    inline Enum_AIRVALVE_Status Get_Status();

    void AIRVALVE_Open();

    void AIRVALVE_Close();

protected:
    // 初始化相关常量
    GPIO_TypeDef *GPIO_Port_Open;

    GPIO_TypeDef *GPIO_Port_Close;

    uint16_t GPIO_Pin_Open;

    uint16_t GPIO_Pin_Close;

    // 常量

    // 内部变量

    // 读变量

    // 气阀状态
    Enum_AIRVALVE_Status AIRVALVE_Status = AIRVALVE_Status_None;

    // 写变量

    // 读写变量

    // 内部函数

};


enum Enum_AIRPUMP_Status
{
    AIRPUMP_Status_None = 0,
    AIRPUMP_Status_Open,
    AIRPUMP_Status_Close,
};

/**
 * @brief Specialized, 气泵
 *
 */
class Class_AIRPUMP
{
public:
    void Init(GPIO_TypeDef *__GPIO_Port_Open, uint16_t __GPIO_Pin_Open);

    inline Enum_AIRPUMP_Status Get_Status();

    void AIRPUMP_Open();

    void AIRPUMP_Close();

protected:
    // 初始化相关常量
    GPIO_TypeDef *GPIO_Port_Open;

    uint16_t GPIO_Pin_Open;

    // 常量

    // 内部变量

    // 读变量

    // 气泵状态
    Enum_AIRPUMP_Status AIRPUMP_Status = AIRPUMP_Status_None;

    // 写变量

    // 读写变量

    // 内部函数

};

/* Exported variables --------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/

/**
 * @brief 获取气阀状态
 *
 * @return Enum_AIRVALVE_Status 气阀状态
 */
inline Enum_AIRVALVE_Status Class_AIRVALVE::Get_Status()
{
    return (AIRVALVE_Status);
}

/**
 * @brief 获取气泵状态
 *
 * @return Enum_AIRPUMP_Status 气泵状态
 */
inline Enum_AIRPUMP_Status Class_AIRPUMP::Get_Status()
{
    return (AIRPUMP_Status);
}

#endif

/************************ COPYRIGHT(C) ROBOPIONEER **************************/
