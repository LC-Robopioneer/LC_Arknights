#include "tsk_main.h"
#define fabs(x) ((x) < 0 ? -(x) : (x))

M3508_Motor M3508_Motor_1;
PID pid_speed;
PID pid_angle;
int16_t isInit = 0;

float output_current = 0;
float error_angle = 0;
float error_speed = 0;
float Actual_Angle = 0;

// 过零处理相关变量
float last_encoder = 0;
float total_angle = 0;  // 累计角度（包含多圈）
uint8_t encoder_init = 0;

// 过零处理函数
float Handle_Encoder_Over_Zero(float current_encoder, float target_encoder)
{
    float error = target_encoder - current_encoder;
    
    // 当误差大于半圈时，认为需要过零处理
    if(error > 4096.0f) {
        // 目标角度比当前角度大很多，实际应该反向小角度到达
        return current_encoder + 8192.0f;  // 将当前角度加一圈
    }
    else if(error < -4096.0f) {
        // 目标角度比当前角度小很多，实际应该正向小角度到达
        return current_encoder - 8192.0f;  // 将当前角度减一圈
    }
    
    return current_encoder;  // 不需要过零处理
}

// 角度累加函数（处理多圈）
void Update_Total_Angle(float current_encoder)
{
    static float last_encoder = 0;
    
    if(!encoder_init) {
        last_encoder = current_encoder;
        encoder_init = 1;
        return;
    }
    
    // 计算编码器变化量
    float delta = current_encoder - last_encoder;
    
    // 处理正向过零（从8191到0）
    if(delta < -4096.0f) {
        delta += 8192.0f;
    }
    // 处理反向过零（从0到8191）
    else if(delta > 4096.0f) {
        delta -= 8192.0f;
    }
    
    // 更新累计角度（单位：度）
    total_angle += (delta / 8191.0f) * 360.0f;
    last_encoder = current_encoder;
}

void Init_All(void)
{
    Serial_RxDMA_Init(&huart3);
    CAN_Init(&hcan1, CAN_FILTER(13) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE, 0x201, 0, Motor_Call_Back);
    M3508_Init(&M3508_Motor_1);
    BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);
    
    PID_Init(&pid_speed, 0.8, 0.2, 0.15, 12000, -12000);
    PID_Init(&pid_angle, 0.072, 0.008, 0.883, 10000, -10000);
    
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, serial_rx_packet, SERIAL_PACKET_SIZE);
}

void Task_Loop(void)
{
    static uint32_t last_control_time = 0;
    uint32_t current_time = HAL_GetTick();
    
    if(isInit == 0)
    {
        Init_All();
        isInit = 1;
    }
    
    // 控制频率限制：5ms一次
    if(current_time - last_control_time < 5) {
        return;
    }
    last_control_time = current_time;
    
    if(serial_rx_flag == 1)
    {
        serial_rx_flag = 0;
        PID_ParseSerialPack(&pid_speed, &pid_angle, serial_rx_packet);
    }
    
    // 更新累计角度
    Update_Total_Angle(Tx_Encoder);
    
    // 角度控制模式（带过零处理）
    if(serial_angle_flag == 1)
    {	
        // 将目标角度转换为编码器值
        float target_encoder = (target_angle / 360.0f) * 8191.0f;
        
        // 应用过零处理
        float processed_encoder = Handle_Encoder_Over_Zero(Tx_Encoder, target_encoder);
        
        // 计算处理后的角度
        Actual_Angle = (processed_encoder / 8191.0f) * 360.0f;
        error_angle = fabs(Actual_Angle - target_angle);
        
        // 提前减速区
        float slowdown_factor = 1.0f;
        if(error_angle < 80.0f) {
            slowdown_factor = error_angle / 80.0f;
            if(slowdown_factor < 0.3f) slowdown_factor = 0.3f;  // 最小减速因子
        }
        
        // 角度环计算目标速度（使用处理后的编码器值）
        float target_speed_from_angle = PID_Caculate(&pid_angle, processed_encoder, target_encoder);
        target_speed_from_angle *= slowdown_factor;
        
        // 速度环计算电流
        output_current = PID_Caculate(&pid_speed, Tx_Omega, target_speed_from_angle);
        
        M3508_Set_Current(&hcan1, &M3508_Motor_1, output_current, 0);
        
        // 到达目标判断
        //static uint8_t stable_count = 0;
        if(error_angle < 30.0f && fabs(Tx_Omega) < 10.0f) {
           // stable_count++;
            //if(stable_count > 5) {
                serial_angle_flag = 0;
                output_current = 0;
                M3508_Set_Current(&hcan1, &M3508_Motor_1, 0, 0);
               // stable_count = 0;
            }
        } 
        
//
    // 速度控制模式
    else if(serial_speed_flag == 1)
    {
        output_current = PID_Caculate(&pid_speed, Tx_Omega, target_speed);
        M3508_Set_Current(&hcan1, &M3508_Motor_1, output_current, 0);
        error_speed = fabs(Tx_Omega - target_speed);
    }
}