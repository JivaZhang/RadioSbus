#include "main.h"
#include "stm32f2xx_hal.h"
#include "UserHardwareInit.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "usart.h"
#ifdef TRACE_ENABLE
#include "trcRecorder.h"
#endif
/**
* @brief  Start the RTOS Kernel with executing the specified thread.
* @param  thread_def    thread definition referenced with \ref osThread.
* @param  argument      pointer that is passed to the thread function as start argument.
* @retval status code that indicates the execution status of the function
* @note   MUST REMAIN UNCHANGED: \b osKernelStart shall be consistent in every CMSIS-RTOS.
*/
/**
* @brief  
* @param  
* @param  
* @retval 
* @note   
*/
/*-----------------------------------------------------------*/
void main(void)
{
#ifdef DEBUG
  debug();
#endif
  
  //��ʼ������Ӳ��
  PreSetupHardware();
  vTraceEnable(TRC_START);
  //��ʼ��freertosϵͳ
  FREERTOS_Init();
  
  /* ��ʼ���� */
  osKernelStart();
  
  for(;;)
  {
      
  }
}
void assert_failed(uint8_t* file, uint32_t line)
{
    
}