#ifndef __ALG_SERIAL_H
#define __ALG_SERIAL_H

#include "stm32f4xx_hal.h"
#include "can.h"
#include "gpio.h"
#include "alg_pid.h"
#include "stdlib.h"

#include "dvc_m3508.h"

extern int16_t  Rx_Encoder;
extern int16_t  Rx_Omega;
extern int16_t  Rx_Torque;
extern int16_t  Rx_Temperature;

extern int16_t  Tx_Encoder;
extern int16_t  Tx_Omega;
extern int16_t  Tx_Torque;
extern int16_t  Tx_Temperature;

HAL_StatusTypeDef PID_ParseSerialPack(PID *pid_speed, PID * pid_angle,uint8_t *serial_pack);

#endif






