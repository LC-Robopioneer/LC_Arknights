/**
 * @file dvc_pmt45.cpp
 * @author Lucy
 * @brief PMT45光电门
 * @version 0.1
 * @date 2025-03-18
 *
 * @copyright ROBOPIONEER (c) 2025
 *
 */

 #ifndef DVC_PMT45_H
 #define DVC_PMT45_H
 
 /* Includes ------------------------------------------------------------------*/
 
 #include "stm32h7xx_hal.h"
 #include "drv_gpio_exti.h"
 
 /* Exported macros -----------------------------------------------------------*/
 
 /* Exported types ------------------------------------------------------------*/
 
 /**
  * @brief Reusable, 光电门核心
  * 使用时请继承
  *
  */
 class Class_PMT45
 {
 public:
    void Init(GPIO_TypeDef *__GPIOx, uint16_t __GPIO_Pin);

    void PMT45_EXTI_Callback();
 protected:
    // 初始化相关常量

    //GPIO句柄
    GPIO_TypeDef *GPIOx;
    //GPIO引脚号
    uint16_t GPIO_Pin;

    // 常量

    // 内部变量

    // 读变量

    // 写变量

    // 读写变量

    // 内部函数
 };
 
 /* Exported variables --------------------------------------------------------*/
 
 /* Exported function declarations --------------------------------------------*/

 #endif
 
 /************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
