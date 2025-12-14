#ifndef USER_TSK_CONFIG_AND_CALLBACK_H
#define USER_TSK_CONFIG_AND_CALLBACK_H

#include "drv_bsp.h"
#include "dvc_M3508.h"
#include "usart.h"
#include "tim.h"
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void Task_Init();
    void Task_Loop();
    
#ifdef __cplusplus
};
#endif

#endif // USER_TSK_CONFIG_AND_CALLBACK_H
