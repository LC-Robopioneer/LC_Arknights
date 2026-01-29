/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "M3508_drv.h"
#include "C_Can.h"
#include "BSP.h"
#include "TIM.h"
#include "string.h"
#include "stdio.h"
#include "drv_pid.h"
extern M3508_Motor M3508_Motor_1;
MOTOR_POSITION_ID   Motor_Posion=Motor1;


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 uint16_t error =0;
 extern int16_t Rx_Encoder, Rx_Omega, Rx_Torque, Rx_Temperature;
 uint8_t TX_Data[8]; 
 int16_t Tx_Encoder, Tx_Omega, Tx_Torque, Tx_Temperature;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Motor_Call_Back(CAN_Rx_Buffer * can_rx_buffer)
  {
	uint8_t *Rx_Data = can_rx_buffer->Data;
    switch (can_rx_buffer->Header.StdId)
    {
    case (0x201):
    {
        Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
        Rx_Omega = (Rx_Data[2] << 8) | Rx_Data[3];
        Rx_Torque = (Rx_Data[4] << 8) | Rx_Data[5];
        Rx_Temperature = Rx_Data[6];
    }
    break;
    }
  }
//把CAN发到板子上的数据用串口发送到电脑
void CAN_RX_Message_Get(void)
{
	 Tx_Encoder = Rx_Encoder;//角度
     Tx_Omega = Rx_Omega;//转速
     Tx_Torque = Rx_Torque;//扭矩
     Tx_Temperature = Rx_Temperature;//温度
	
	 TX_Data[0]=(Tx_Omega>>8)&0xFF;
	 TX_Data[1]=Tx_Omega&0xFF;
	 
	 TX_Data[2]=(Tx_Encoder>>8)&0xFF;
	 TX_Data[3]=Tx_Encoder&0xFF;
	
	 TX_Data[4]=(Tx_Torque>>8)&0xFF;
	 TX_Data[5]=Tx_Torque&0xFF;
	
	 TX_Data[6]=(Tx_Temperature>>8)&0xFF;
	 TX_Data[7]=Tx_Temperature&0xFF;
	  
	 char buffer[64];
	 sprintf(buffer,"Encoder:%d,Omega:%d,Torque:%d,Temperature:%d\n",Tx_Encoder,Tx_Omega ,Tx_Torque,Tx_Temperature);
	//HAL_UART_Transmit_DMA(&huart2,TX_Data,sizeof(buffer)/sizeof(buffer[0]));如果一上面的形式发送，用字符数组 且uint8_t->char
	 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//0.1s
{
	static int i=0;
	static int temp=0;//周期数
	static int state=0;
	if(htim==&htim4)
	{
	   if(M3508_Motor_1.emergency_stop!=1)
	   {
		  if(state==0)
		{	
		    i+=50;
		}	
		else if (state==1)
		 {
		    i-=50;	
		    if(i<=0)
			{
				state=0;
				temp+=1;
			}
		 }
	
	   if(i>=1000)
		 {
			state=1;
		 }	
	
		 M3508_Set_Current(&hcan1,&M3508_Motor_1,i,0);
	   }
	   else
	   {
		  Delay_ms(5000); //dan
		  M3508_Resume_From_Stop(&hcan1,&M3508_Motor_1,0);  
	   }
	 if(temp>=5)//紧急停止模拟 5个周期后紧急停止  
		{
			M3508_Emergency_Stop(&hcan1,&M3508_Motor_1,1);
			temp=0;
		}	
		
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	PID pid;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  CAN_Init(&hcan1, CAN_FILTER(13) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE,0x201,0x3ff,Motor_Call_Back);
  M3508_Init(&M3508_Motor_1);
  BSP_Init( BSP_DC24_LU_ON| BSP_DC24_LD_ON |BSP_DC24_RU_ON|BSP_DC24_RD_ON);
  PID_Init (&pid,0,0,0,16384,-16384);
  //HAL_TIM_Base_Start(&htim4);
  HAL_TIM_Base_Start_IT(&htim4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//Task_Loop();  
	//M3508_Set_Current(&hcan1,&M3508_Motor_1,3000,0);
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
