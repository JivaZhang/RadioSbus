#include "stm32f2xx_hal.h"
#include "usart.h"
#include "freertosconfig.h"
#include "cmsis_os.h"
#include "mavlink.h"
#include "sbus.h"
#include "radio.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart6;

/* Extern Variables ----------------------------------------------------------*/
EXTERN_VARABLE_DECLEAR_RADIO;
EXTERN_VARABLE_DECLEAR_SBUS;
extern void Error_Handler(void);
//const struct GroupInfo usart_var_info[] = 
//{
//  {STORAGE_USART_CONFIG,"baudrate",1,baudRate,115200},
//  {STORAGE_USART_CONFIG,"wordlength",2,wordLength,UART_WORDLENGTH_8B},
//  {STORAGE_USART_CONFIG,"stopbits",3,stopBits,UART_STOPBITS_1},
//  {STORAGE_USART_CONFIG,"parity",4,parity,UART_PARITY_NONE},
//  {STORAGE_NONE,0,0,0,0},
//};
void  Usart1UartInit(void)
{
  /************串口的初始化API会对参数做检查********************/
//    assert_param(udCfg);
//    huart1.Instance = USART1;
//    huart1.Init.BaudRate = udCfg->baudRate;
//    huart1.Init.WordLength = udCfg->wordLength;
//    huart1.Init.StopBits = udCfg->stopBits;
//    huart1.Init.Parity = udCfg->parity;
    
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 57600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/***************************************************/
void  Usart2UartInit(void)
{
//    assert_param(udCfg);
//    huart2.Instance = USART1;
//    huart2.Init.BaudRate = udCfg->baudRate;
//    huart2.Init.WordLength = udCfg->wordLength;
//    huart2.Init.StopBits = udCfg->stopBits;
//    huart2.Init.Parity = udCfg->parity;
    
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

/***************************************************/
void  Usart3UartInit(void)
{
//    assert_param(udCfg);
//    huart3.Instance = USART1;
//    huart3.Init.BaudRate = udCfg->baudRate;
//    huart3.Init.WordLength = udCfg->wordLength;
//    huart3.Init.StopBits = udCfg->stopBits;
//    huart3.Init.Parity = udCfg->parity;
    
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 57600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}
/***************************************************/
void  Usart4UartInit(void)
{
//    assert_param(udCfg);
//    huart3.Instance = USART1;
//    huart3.Init.BaudRate = udCfg->baudRate;
//    huart3.Init.WordLength = udCfg->wordLength;
//    huart3.Init.StopBits = udCfg->stopBits;
//    huart3.Init.Parity = udCfg->parity;
    
    huart4.Instance = UART4;
    huart4.Init.BaudRate = 100000;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_2;
    huart4.Init.Parity = UART_PARITY_EVEN;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler();
    }
}
/***************************************************/
void  Usart5UartInit(void)
{
//    assert_param(udCfg);
//    huart3.Instance = USART1;
//    huart3.Init.BaudRate = udCfg->baudRate;
//    huart3.Init.WordLength = udCfg->wordLength;
//    huart3.Init.StopBits = udCfg->stopBits;
//    huart3.Init.Parity = udCfg->parity;
    
    huart5.Instance = UART5;
    huart5.Init.BaudRate = 115200;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
        Error_Handler();
    }
}
/***************************************************/
void  Usart6UartInit(void)
{
//    assert_param(udCfg);
//    huart6.Instance = USART1;
//    huart6.Init.BaudRate = udCfg->baudRate;
//    huart6.Init.WordLength = udCfg->wordLength;
//    huart6.Init.StopBits = udCfg->stopBits;
//    huart6.Init.Parity = udCfg->parity;
    
    huart6.Instance = USART6;
    huart6.Init.BaudRate = 115200;
    huart6.Init.WordLength = UART_WORDLENGTH_8B;
    huart6.Init.StopBits = UART_STOPBITS_1;
    huart6.Init.Parity = UART_PARITY_NONE;
    huart6.Init.Mode = UART_MODE_TX_RX;
    huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart6.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart6) != HAL_OK)
    {
        Error_Handler();
    }    
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART1)
  {

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    //HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    NVIC_SetPriority(USART1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  }
  else if(uartHandle->Instance==USART2)
  {

    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    //HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    //HAL_NVIC_EnableIRQ(USART2_IRQn);
    
    NVIC_SetPriority(USART2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    //__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    //HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    NVIC_SetPriority(USART3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    //__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE|UART_IT_TXE);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==UART4)
  {
    /* Peripheral clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PA0     ------> USART4_TX
    PA1     ------> USART4_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    //HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    NVIC_SetPriority(UART4_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
    //__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
    /* Peripheral clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PC12     ------> USART5_TX
    PD2     ------> USART5_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    /* Peripheral interrupt init */
    //HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    NVIC_SetPriority(UART5_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
    //__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();
  
    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    //HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    NVIC_SetPriority(USART6_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
    //__HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {

    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  else if(uartHandle->Instance==USART2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART2_IRQn);

  }
  else if(uartHandle->Instance==USART3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();
  
    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART6_IRQn);

  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
} 

/**
* @brief This function handles USART2 global interrupt.
*/

void USART1_IRQHandler(void)
{
    portBASE_TYPE xStatus = pdFALSE;
    uint32_t isrflags   = READ_REG(huart1.Instance->SR);
    uint32_t cr1its     = READ_REG(huart1.Instance->CR1);
    uint32_t cr3its     = READ_REG(huart1.Instance->CR3);
    uint32_t errorflags = 0x00U;
    char c;
    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    
    if(errorflags == RESET)
    {
        /* UART in mode Receiver -------------------------------------------------*/
        if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
        {
            c = huart1.Instance->DR;
            xQueueSendToBackFromISR(radioMsgRxQHandle,&c,&xStatus);
            __HAL_UART_CLEAR_FLAG(&huart1, USART_SR_RXNE);
        }
               
        if(((isrflags & UART_IT_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
        {
            __HAL_UART_CLEAR_FLAG(&huart1, USART_SR_TC);
            if( xQueueReceiveFromISR( radioMsgTxQHandle, &c, &xStatus ) == pdTRUE )
            {
                huart1.Instance->DR = (c & (uint8_t)0xFFU);
            }
            else
            {
                __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);//Give signal to usart handler to start a 2o bytes' data transfer
            }
        }
    }
    else
    {
        errorflags = huart1.Instance->DR;
        __HAL_UART_CLEAR_FLAG(&huart1, USART_SR_ORE);
        __HAL_UART_CLEAR_FLAG(&huart1, USART_SR_RXNE);
        __HAL_UART_CLEAR_FLAG(&huart1, USART_SR_FE);
        __HAL_UART_CLEAR_FLAG(&huart1, USART_SR_NE);
    }
}


/**
* @brief This function handles USART6 global interrupt.
*/
void USART2_IRQHandler(void)
{
    portBASE_TYPE xStatus = pdFALSE;
    uint32_t isrflags   = READ_REG(huart2.Instance->SR);
    uint32_t cr1its     = READ_REG(huart2.Instance->CR1);
    uint32_t cr3its     = READ_REG(huart2.Instance->CR3);
    uint32_t errorflags = 0x00U;
    char c;
    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
    
    if(errorflags == RESET)
    {
        /* UART in mode Receiver -------------------------------------------------*/
        if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
        {
            c = huart2.Instance->DR;
            xQueueSendToBackFromISR(radioMsgRxQHandle,&c,&xStatus);
        }
        __HAL_UART_CLEAR_FLAG(&huart2, USART_SR_RXNE);
    }
    else
    {
        errorflags = huart2.Instance->DR;
        __HAL_UART_CLEAR_FLAG(&huart2, USART_SR_ORE);
        __HAL_UART_CLEAR_FLAG(&huart2, USART_SR_RXNE);
        __HAL_UART_CLEAR_FLAG(&huart2, USART_SR_FE);
        __HAL_UART_CLEAR_FLAG(&huart2, USART_SR_NE);
    }
}

/**
* @brief This function handles USART3 global interrupt.
*/
void USART3_IRQHandler(void)
{
    char c;
    portBASE_TYPE xStatus = pdFALSE;
    uint32_t isrflags   = READ_REG(huart3.Instance->SR);
    uint32_t cr1its     = READ_REG(huart3.Instance->CR1);
    uint32_t cr3its     = READ_REG(huart3.Instance->CR3);
    uint32_t errorflags = 0x00U;
    c = huart3.Instance->DR;
    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE  |USART_SR_NE));
    
    if(errorflags == RESET)
    {
        /* UART in mode Receiver -------------------------------------------------*/
        if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
        {
            c = huart3.Instance->DR;
            xQueueSendToBackFromISR(serialMsgRxQHandle,&c,&xStatus);
            __HAL_UART_CLEAR_FLAG(&huart3, USART_SR_RXNE);
        }
        
        if(((isrflags & UART_IT_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
        {
            __HAL_UART_CLEAR_FLAG(&huart3, USART_SR_TXE);
            if( xQueueReceiveFromISR( serialMsgTxQHandle, &c, &xStatus ) == pdTRUE )
            {
                huart3.Instance->DR = (c & (uint8_t)0xFFU);
            }
            else
            {
                __HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE);//Give signal to usart handler to start a 2o bytes' data transfer
            }
        }
    }
    else
    {
        errorflags = huart3.Instance->DR;
        __HAL_UART_CLEAR_FLAG(&huart3, USART_SR_ORE);
        __HAL_UART_CLEAR_FLAG(&huart3, USART_SR_RXNE);
        __HAL_UART_CLEAR_FLAG(&huart3, USART_SR_FE);
        __HAL_UART_CLEAR_FLAG(&huart3, USART_SR_NE);
      

    }
}

uint32_t error_cnt = 0;
uint32_t good_cnt = 0;
/**
* @brief This function handles USART3 global interrupt.
*/

void UART4_IRQHandler(void)
{
    portBASE_TYPE xStatus = pdFALSE;
    uint32_t isrflags   = READ_REG(huart4.Instance->SR);
    uint32_t cr1its     = READ_REG(huart4.Instance->CR1);
    uint32_t cr3its     = READ_REG(huart4.Instance->CR3);
    uint32_t errorflags = 0x00U;
    char c; //huart4.Instance->DR & ((uint8_t)0x007FU);
    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(USART_SR_PE /*| USART_SR_FE*/ | USART_SR_ORE /*| USART_SR_NE*/));
    
    if(errorflags == RESET)
    {
        /* UART in mode Receiver -------------------------------------------------*/
        if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)) 
        {
            c = huart4.Instance->DR;
            xQueueSendToBackFromISR( sbusMsgRxQHandle,&c,&xStatus);
            __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_RXNE);
        }
                
        if(((isrflags & UART_IT_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
        {
            __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_TXE);
            if( xQueueReceiveFromISR( sbusMsgTxQHandle, &c, &xStatus ) == pdTRUE )
            {
                huart4.Instance->DR = (c & (uint8_t)0xFFU);
            }
            else
            {
                __HAL_UART_DISABLE_IT(&huart4, UART_IT_TXE);//Give signal to usart handler to start a 2o bytes' data transfer
            }
        }
        good_cnt++;
    }
    else
    {
        error_cnt++;
        
        __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_ORE);
        __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_RXNE);
        __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_FE);
        __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_PE);
        __HAL_UART_CLEAR_FLAG(&huart4, USART_SR_NE);
        errorflags = huart4.Instance->DR;
    }
  
}



void USART6_IRQHandler(void)
{
//    portBASE_TYPE xStatus = pdFALSE;
//    uint32_t isrflags   = READ_REG(huart6.Instance->SR);
//    uint32_t cr1its     = READ_REG(huart6.Instance->CR1);
//    uint32_t cr3its     = READ_REG(huart6.Instance->CR3);
//    uint32_t errorflags = 0x00U;
//    char dr;
//    /* If no error occurs */
//    errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
//    
//    if(errorflags == RESET)
//    {     
//        if(((isrflags & UART_IT_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
//        {
//            __HAL_UART_CLEAR_FLAG(&huart6, USART_SR_TC);
//            if( xQueueReceiveFromISR( appMsgTxQHandle, &dr, &xStatus ) == pdTRUE )
//            {
//                huart6.Instance->DR = (dr & (uint8_t)0xFFU);
//            }
//            else
//            {
//                __HAL_UART_DISABLE_IT(&huart6, UART_IT_TXE);//Give signal to usart handler to start a 2o bytes' data transfer
//            }
//        }	
//    }
//    else
//    {
//        errorflags = huart6.Instance->DR;
//        __HAL_UART_CLEAR_FLAG(&huart6, USART_SR_ORE);
//        __HAL_UART_CLEAR_FLAG(&huart6, USART_SR_RXNE);
//        __HAL_UART_CLEAR_FLAG(&huart6, USART_SR_FE);
//        __HAL_UART_CLEAR_FLAG(&huart6, USART_SR_NE);
//    }
}

void EnableUsartAllInterrupt(void)
{
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
}