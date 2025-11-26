#ifndef __BSP_H
#define __BSP_H

#include "stm32f4xx_hal.h"

#define BSP_DC24_LU_ON (1 << 0)
#define BSP_DC24_LD_ON (1 << 1)
#define BSP_DC24_RU_ON (1 << 2)
#define BSP_DC24_RD_ON (1 << 3)


typedef enum 
{
    BSP_DC24_Status_DISABLED = 0,
    BSP_DC24_Status_ENABLED
}Enum_BSP_DC24_Status;


void BSP_Init(uint32_t Status);

void BSP_DC24_LU(Enum_BSP_DC24_Status Status);
void BSP_DC24_LD(Enum_BSP_DC24_Status Status);
void BSP_DC24_RU(Enum_BSP_DC24_Status Status);
void BSP_DC24_RD(Enum_BSP_DC24_Status Status);

#endif




