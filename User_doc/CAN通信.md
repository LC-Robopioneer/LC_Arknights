## CAN报文数据处理
### 数据包
`can_rx_packet` - CAN接收数据包, 共8字节, 均为`uint8_t`类型  
`data_h` - 高8位数据, `uint8_t`类型  
`data_l` - 低8位数据, `uint8_t`类型  
`data_raw` - 拼接后的原始数据, `uint16_t`类型
### 数据拼接
以大端序为例 (C语言)
``` c
data_h = can_rx_packet[0];  // 高8位数据
data_l = can_rx_packet[1];  // 低8位数据

// 数据拼接
// uint8_t类型的数据位操作会隐式转换为int
data_raw = (data_h << 8) | data_l;  

// 更为保险的显式转换
data_raw = ((uint16_t)data_h << 8) | data_l;

// 再根据实际需求把data_raw转换为有符号或无符号的
int16_t data_real = (int16_t)data_raw;
```
### 封装优化
可封装为内联函数
```c
/**
 * @brief 大端序结构, 拼接两个uint8_t字节成一个int16_t
 * @param high 高8位字节
 * @param low 低8位字节
 * @return 拼接后的目标数据
 */
static inline int16_t can_to_int16(uint8_t high, uint8_t low)
{
    return (int16_t)(((uint16_t)high << 8) | low);
}

/**
 * @brief 大端序结构, 拼接两个uint8_t字节成一个uint16_t
 * @param high 高8位字节
 * @param low 低8位字节
 * @return 拼接后的目标数据
 */
static inline int16_t can_to_uint16(uint8_t high, uint8_t low)
{
    return (uint16_t)(((uint16_t)high << 8) | low);
}
```
