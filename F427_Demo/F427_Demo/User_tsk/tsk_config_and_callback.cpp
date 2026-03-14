
#include "tsk_config_and_calback.h"
 
 void task_1ms_TIM6_Callback(void)
 {
    g_Power_Channel_LU_Status ==1 ? BSP_DC24_LU(BSP_DC24_Status_ENABLED) : BSP_DC24_LU(BSP_DC24_Status_DISABLED);
    g_Power_Channel_LD_Status ==1 ? BSP_DC24_LD(BSP_DC24_Status_ENABLED) : BSP_DC24_LD(BSP_DC24_Status_DISABLED);
    g_Power_Channel_RU_Status ==1 ? BSP_DC24_RU(BSP_DC24_Status_ENABLED) : BSP_DC24_RU(BSP_DC24_Status_DISABLED);
    g_Power_Channel_RD_Status ==1 ? BSP_DC24_RD(BSP_DC24_Status_ENABLED) : BSP_DC24_RD(BSP_DC24_Status_DISABLED);
 }

 void task_init(void)
 {
    TIM_Init(&htim6,task_1ms_TIM6_Callback);

     //启动TIM6中断模式
    HAL_TIM_Base_Start_IT(&htim6);

    // BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);

    CAN_Init(&hcan1, Power_Control_Callback);
    CAN_Filter_Mask_Config(&hcan1, CAN_FILTER(0) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0x100, 0x7fff);

    g_is_init_finished=true;
 }

 void task_loop(void)
 {
    
 }

 


