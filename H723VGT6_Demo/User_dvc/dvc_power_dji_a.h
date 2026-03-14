 /**
 * @file dvc_power_dji_a.h
 * @author Gcy
 * @brief 通过控制大疆A板24V电源实现气泵控制
 * @date 2026-3-14 
 *
 *
 */
 #ifndef DVC_POWER_DJI_A_H
 #define DVC_POWER_DJI_A_H
 
 #include "stm32h7xx_hal.h"
 #include "drv_can.h"
 
 #define Power_COMMAND_ID 0x100

 #define Power_CHANNEL_LU_ON 0x11  //LU
 #define Power_CHANNEL_LD_ON 0x21  //LD
 #define Power_CHANNEL_RU_ON 0x31  //RU
 #define Power_CHANNEL_RD_ON 0x41  //RD

 #define Power_CHANNEL_LU_OFF 0x10
 #define Power_CHANNEL_LD_OFF 0x20
 #define Power_CHANNEL_RU_OFF 0x30
 #define Power_CHANNEL_RD_OFF 0x40

 
 class CAN_PowerController
  {
    public:
        CAN_PowerController() {}
        bool turn_on_power(uint8_t powerChannel);
        bool turn_off_power(uint8_t powerChannel);
       
    private:
        
        uint8_t channel;

      
  };


 #endif
 

