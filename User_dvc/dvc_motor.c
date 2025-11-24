/**
 * @file dvc_motor.c
 * @author lyh 
 * @brief 按照C620电调CAN收发数据手册写的驱动
 * @version 0.1
 * @date 2025-11-26 起稿
 *
 * @copyright RoboPioneer (c) 2024~2025
 *
 */
#include "dvc_motor.h" 

// 使用宏定义来简化配置
#define CAN_STD(id)         ((uint32_t)(id) << 5)
#define CAN_EXT(id)         ((uint32_t)(id) << 3)
#define CAN_IDE_MASK        0x0002U
#define CAN_STD_FRAME       0x0000U           // 标准帧(IDE=0)
#define CAN_EXT_FRAME       0x0004U           // 扩展帧(IDE=1)
#define CAN_DATA_FRAME      0x0000U           // 数据帧(RTR=0)  
#define CAN_REMOTE_FRAME    0x0002U           // 远程帧(RTR=1)

static uint8_t motor_tx_0x200[8] = {0};
static uint8_t motor_tx_0x1FF[8] = {0};

static PID_HandleTypeDef pid1 = {
    .output_max = 5000,     // 16384对应20A, 谁敢让它满转?
    .output_min = -5000,
    .err_int_max = 400,
    .compensation = 0,
    .deadzone = 0,
};

Motor_HandleTypeDef hmotor1 = {
    .hcan = &hcan1,
    .pid = &pid1,
    .id = 1,
};


static PID_HandleTypeDef hpid[9];   // PID句柄数组，下标1~8有效, 输出为映射后的电流值

/**
 * @brief 用于CAN总线发送的数据包
 * @note 0:motor1_H, 1:motor1_L, 2:motor2_H, 3:motor3_L, ...
 */
Motor_HandleTypeDef hmotor[9];


/**
 * @brief 初始化电机及CAN总线过滤器
 */
void Motor_Init(void)
{
    // 初始化电机
    for(uint8_t i=1; i<=8; i++)
    {
        hmotor[i].pid = &hpid[i];
        hpid[i].mode = PID_MODE_POS_NORMAL;
        hmotor[i].id = i;
        hmotor[i].hcan = &hcan1;
        hmotor[i].gear_ratio = 19;
    }

    // 数据帧结构 
    // 31             20 19          16 15     3   2   1     0
    // +----------------+---------------+-------+--------------+
    // |    标准帧ID     |     保留             |               |
    // |    [10:0]      |     (未使用)          |               |
    // +----------------+---------------+-------+               +
    // |                  扩展帧ID              | RTR | IDE | 0 |
    // 31--------------------------------------3---2----1----0--
    CAN_FilterTypeDef CAN_Filter = {0};
    CAN_Filter.FilterBank = 0;
    CAN_Filter.FilterFIFOAssignment = CAN_FilterFIFO0;
    CAN_Filter.FilterIdHigh = CAN_STD(0x200);       // 10 0000 0000 低四位1~8
    CAN_Filter.FilterIdLow = CAN_STD_FRAME | CAN_DATA_FRAME;
    CAN_Filter.FilterMaskIdHigh = CAN_STD(0x3F0);   // 11 1111 0000 低四位随便
    CAN_Filter.FilterMaskIdLow = 0xFF;              // RTR IDE 完全匹配  
    // CAN_Filter.FilterMaskIdHigh = 0x00;   
    // CAN_Filter.FilterMaskIdLow = 0x00;              // 全通  
    CAN_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_Filter.FilterScale = CAN_FILTERSCALE_32BIT;
//    CAN_Filter.FilterActivation = DISABLE;
    CAN_Filter.FilterActivation = ENABLE;
    HAL_CAN_ConfigFilter(&hcan1, &CAN_Filter);
	HAL_CAN_Init(&hcan1);
    
    HAL_CAN_Start(&hcan1);

    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    // __HAL_CAN_ENABLE_IT(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
}

/**
 * @brief 直接设置C620电机电流
 * @param hmotor
 * @param current 目标电流, -20 ~ 20A
 * @note 1~ 4号电机标识符为0x200, 5 ~8号为0x1FF
 */
HAL_StatusTypeDef Motor_SetCurrentDirect(Motor_HandleTypeDef *hmotor, float current)
{
    // 参数检查
    if (hmotor == NULL || hmotor->hcan == NULL) 
    {
        return HAL_ERROR;
    }

    int16_t current_map;    // 电流映射到-16384 ~ 16384 
    
    // 电流限幅
    if(current > 20) current = 20;
    if(current < -20) current = -20;

    // 电流映射
    current_map = (int16_t)(current * 16384 / 20.0f);

    return Motor_SetCurrentMap(hmotor, current_map);
}

/**
 * @brief 设置C620电机电流映射到16384后
 * @param hmotor 
 * @param current 目标电流, -20 ~ 20A
 * @note 1~ 4号电机标识符为0x200, 5 ~8号为0x1FF
 */
HAL_StatusTypeDef Motor_SetCurrentMap(Motor_HandleTypeDef *hmotor, int32_t current_map)
{
    uint32_t tx_id;
    
    // 电流限幅
    if(current_map > 16384) current_map = 16384;
    if(current_map < -16384) current_map = -16384;

    // 根据电机给ID赋值
    if(1 <= hmotor->id && hmotor->id <= 4)
    {
        tx_id = 0x200;
        motor_tx_0x200[hmotor->id*2 - 1] = (uint8_t)current_map;       // 低八位
        motor_tx_0x200[hmotor->id*2 - 2] = (uint8_t)(current_map >> 8);// 高八位 
        return CAN_SendMsg(hmotor->hcan, tx_id, motor_tx_0x200);
    }
    else if(5 <= hmotor->id && hmotor->id <= 8)
    {
        tx_id = 0x1FF;
        motor_tx_0x1FF[(hmotor->id - 4)*2 - 1] = (uint8_t)current_map;       // 低八位
        motor_tx_0x1FF[(hmotor->id - 4)*2 - 2] = (uint8_t)(current_map >> 8);// 高八位 
        return CAN_SendMsg(hmotor->hcan, tx_id, motor_tx_0x1FF);
    }
    else
    {
        return HAL_ERROR;
    }
    
}

/**
 * @brief 拼接两个uint8_t成一个int16_t
 * @param high 高8位字节
 * @param low 低8位字节
 */
static inline int16_t can_to_int16(uint8_t high, uint8_t low)
{
    return (int16_t)(((uint16_t)high << 8) | low);
}

/**
 * @brief 拼接两个uint8_t成一个uint16_t
 * @param high 高8位字节
 * @param low 低8位字节
 */
static inline uint16_t can_to_uint16(uint8_t high, uint8_t low)
{
    return (uint16_t)(((uint16_t)high << 8) | low);
}

/**
 * @brief 获取电机参数, 自动从can_rx_packet中解析
 * @param hmotor 电机句柄
 * @note 仅赋值电机参数: id, angle, rpm, current, temperature
 * @note 不涉及PID控制器
 */
HAL_StatusTypeDef Motor_GetParam(Motor_HandleTypeDef *hmotor)
{ 
    // 参数检查
    if (hmotor == NULL || hmotor->hcan == NULL) 
    {
        return HAL_ERROR;
    }

    uint32_t rx_id;
    int16_t raw_angle, raw_rpm, raw_curr;
    int8_t raw_temp;

    HAL_StatusTypeDef state = CAN_ReceiveMsg(hmotor->hcan, CAN_RX_FIFO0, can_rx_packet, &rx_id);
    if(state != HAL_OK) return state;

    // 验证CAN ID有效性 (0x201-0x208)
    if (rx_id < 0x201 || rx_id > 0x208) {
        return HAL_ERROR;
    }

    /*获取ID, 电调ID = 标识符 - 0x200 */
    hmotor->id = rx_id - 0x200;

    // 获取原始数据
    // 0角度H 1角度L 2转速H 3转速L 4转矩电流H 5转矩电流L 6温度 7NULL
    raw_angle = can_to_uint16(can_rx_packet[0], can_rx_packet[1]);
    raw_rpm = can_to_int16(can_rx_packet[2], can_rx_packet[3]);
    raw_curr = can_to_int16(can_rx_packet[4], can_rx_packet[5]);
    raw_temp = (int8_t)can_rx_packet[6];

    // 转化为真实数据
    // angle: 0~8192 -> 0~360°
    // rpm: no map
    // current: -16384~16384 -> -20~20A
    // temperature: no map
    hmotor->angle = raw_angle * 360.0f / 8192.0f;
    hmotor->rpm = raw_rpm;
    hmotor->real_current = raw_curr * 20.0f / 16384.0f;
    hmotor->temperature = raw_temp;

    return HAL_OK;
}

/**
 * @brief 设置电机目标电流, 单位A
 * @param motor_id 目标电机编号
 * @param current 目标电流
 */
HAL_StatusTypeDef Motor_SetTargetCurrent(uint8_t motor_id, float target_current)
{
    if(motor_id < 1 || motor_id > 8)
    {
        return HAL_ERROR;
    }

    hmotor[motor_id].target_current = target_current;
    
    return HAL_OK;
}

/**
 * @brief 把数据更新到PID
 * @note 调试用
 */
void Motor_PIDUpdate(Motor_HandleTypeDef *hmotor)
{
    hmotor->pid->actual = hmotor->rpm;
    PID_Update(hmotor->pid);
    hmotor->target_current = hmotor->pid->output;
}

/**
 * @brief 电机设置RPM转速, 闭环控制
 * @note 内置PID控制器
 * @param hmotor 电机句柄
 * @param target_rpm 目标转速
 */
HAL_StatusTypeDef Motor_SetTargetRPM(Motor_HandleTypeDef *hmotor, float target_rpm)
{
    // 参数检查
    if (hmotor == NULL || hmotor->hcan == NULL) 
    {
        return HAL_ERROR;
    }

    // 目标转速限幅
    if(target_rpm > 400) target_rpm = 400;
    if(target_rpm < -400) target_rpm = -400;

    hmotor->pid->target = target_rpm;
    hmotor->pid->actual = hmotor->rpm;
    PID_Update(hmotor->pid);
    Motor_SetCurrentMap(hmotor, hmotor->pid->output);
    
    return HAL_OK;
}
