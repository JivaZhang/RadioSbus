/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "usart.h"
#include "mavlink.h"
#include "radio.h"
#include "sbus.h"
#include "trcRecorder.h"
/* Macro ---------------------------------------------------------------------*/
#define START_TASK_STACK_SIZE 128  //uint 32-bits
    
/* Variables -----------------------------------------------------------------*/
osThreadId startTaskHandle;


/* Extern Variables ----------------------------------------------------------*/
EXTERN_VARABLE_DECLEAR_SBUS;
EXTERN_VARABLE_DECLEAR_RADIO;
/* Function prototypes -------------------------------------------------------*/
void StartTask(void const * argument);

/* Extern Function prototypes ------------------------------------------------*/

/* Init FreeRTOS */
void FREERTOS_Init(void) {
  
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(startTask, StartTask, osPriorityNormal, 0, START_TASK_STACK_SIZE);
  startTaskHandle = osThreadCreate(osThread(startTask), NULL);
  
  /* USER CODE BEGIN RTOS_THREADS */ 
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}       

/* StartDefaultTask function */
void StartTask(void const * argument)
{
    /*create radio  receive task*/
    radioMsgRxQHandle = xQueueCreate(RADIO_MSG_RX_QUEUE_SIZE, RADIO_MSG_RX_PACK_LEN);
    if(NULL != radioMsgRxQHandle)
    {
        osThreadDef(RadioMsgRxTask, RadioMsgRxTask, osPriorityNormal, 0, RADIO_MSG_RX_TASK_STACK_SIZE);
        radioMsgRxTaskHandle = osThreadCreate(osThread(RadioMsgRxTask), NULL);  
    }
    
    /*create ground station transfer task*/
    radioMsgTxQHandle = xQueueCreate(RADIO_MSG_TX_QUEUE_SIZE, RADIO_MSG_TX_PACK_LEN);
    osThreadDef(RadioMsgTxTask, RadioMsgTxTask, osPriorityNormal, 0, RADIO_MSG_TX_TASK_STACK_SIZE);
    radioMsgTxTaskHandle = osThreadCreate(osThread(RadioMsgTxTask), NULL);  
    
    /*create ground station transfer task*/
    sbusMsgRxQHandle = xQueueCreate(SBUS_MSG_RX_QUEUE_SIZE, SBUS_MSG_RX_PACK_LEN);
    sbusMsgTxQHandle = xQueueCreate(SBUS_MSG_TX_QUEUE_SIZE, SBUS_MSG_TX_PACK_LEN);
    serialMsgRxQHandle = xQueueCreate(SERIAL_MSG_RX_QUEUE_SIZE, SERIAL_MSG_RX_PACK_LEN);
    serialMsgTxQHandle = xQueueCreate(SERIAL_MSG_TX_QUEUE_SIZE, SERIAL_MSG_TX_PACK_LEN);
    
/*���ж�***************************************************/
    EnableUsartAllInterrupt();
    /* Infinite loop */
    for(;;)
    {
        osDelay(1000);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
