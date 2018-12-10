#ifndef __USART_H__
#define __USART_H__

typedef struct UsartDeviceConfig
{
    uint32_t func;
    uint32_t baudRate;
    uint32_t wordLength;
    uint32_t stopBits;
    uint32_t parity;
}stUsartDevCfg;
#define EXTERN_VARABLE_DECLEAR_UART \
extern UART_HandleTypeDef huart1; \
extern UART_HandleTypeDef huart2; \
extern UART_HandleTypeDef huart3; \
extern UART_HandleTypeDef huart4; \
extern UART_HandleTypeDef huart5; \
extern UART_HandleTypeDef huart6;
/*******************************************************************************
* Function Name  : Usart1UartInit
* Description    : This function handles USART3 init
* Input          : stUsartDevCfg  * udCfg
* Output         : None
* Return         : None
*******************************************************************************/
void  Usart1UartInit(void);

/*******************************************************************************
* Function Name  : Usart2UartInit
* Description    : This function handles USART3 init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Usart2UartInit(void);

/*******************************************************************************
* Function Name  : Usart3UartInit
* Description    : This function handles USART3 init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Usart3UartInit(void);

/*******************************************************************************
* Function Name  : Usart4UartInit
* Description    : This function handles USART3 init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Usart4UartInit(void);


/*******************************************************************************
* Function Name  : Usart5UartInit
* Description    : This function handles USART3 init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Usart5UartInit(void);
 

/*******************************************************************************
* Function Name  : Usart6UartInit
* Description    : This function handles USART3 init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Usart6UartInit(void);


/**
* @brief  打开串口中断
* @param  
* @param  
* @retval 
* @note   
*/
void EnableUsartAllInterrupt(void);
#endif