#include "BSP.h"
#include "main.h"

void BSP_Init(uint32_t Status)
{
	BSP_DC24_LU((Enum_BSP_DC24_Status)((Status & BSP_DC24_LU_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
	BSP_DC24_LD((Enum_BSP_DC24_Status)((Status & BSP_DC24_LD_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
	BSP_DC24_RU((Enum_BSP_DC24_Status)((Status & BSP_DC24_RU_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
	BSP_DC24_RD((Enum_BSP_DC24_Status)((Status & BSP_DC24_RD_ON) == 0 ? BSP_DC24_Status_DISABLED : BSP_DC24_Status_ENABLED));
}


void BSP_DC24_LU(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, (GPIO_PinState)(Status));
}
void BSP_DC24_LD(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, (GPIO_PinState)(Status));
}
void BSP_DC24_RU(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, (GPIO_PinState)(Status));
}
void BSP_DC24_RD(Enum_BSP_DC24_Status Status)
{
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, (GPIO_PinState)(Status));
}
