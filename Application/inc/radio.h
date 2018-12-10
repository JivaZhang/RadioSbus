#ifndef __RADIO_H__
#define __RADIO_H__

#define RADIO_MSG_RX_QUEUE_SIZE    512
#define RADIO_MSG_RX_PACK_LEN      1
#define RADIO_MSG_RX_TASK_STACK_SIZE   256
#define RADIO_MSG_TX_QUEUE_SIZE    512
#define RADIO_MSG_TX_PACK_LEN      1
#define RADIO_MSG_TX_TASK_STACK_SIZE   256

/************************************/
#define EXTERN_VARABLE_DECLEAR_RADIO \
extern xQueueHandle radioMsgRxQHandle; \
extern osThreadId radioMsgRxTaskHandle; \
extern xQueueHandle radioMsgTxQHandle; \
extern osThreadId radioMsgTxTaskHandle; 



typedef struct radioMsg
{
    uint8_t frameHead;     //把sbus数据打包的帧头
    uint8_t sbusFrameHead;    //sbus数据头
    uint8_t payload[23];      //数据加一位校验位
    uint8_t sbusFrameTail;    //sbus 数据尾
    uint8_t frameEnd;      //把sbus数据打包的帧尾
}radioMsg_st;


/*函数声明***************************************************************/
/**
* @brief  从数传接受到消息的处理函数
* @param  
* @param  
* @retval 
* @note   目前是把接受到的数据直接发送到飞控
*/
void RadioMsgRxTask(void const * argument);

/*函数声明***************************************************************/
/**
* @brief  发送消息到数传
* @param  
* @param  
* @retval 
* @note   
*/
void RadioMsgTxTask(void const * argument);

/***********************************/
#endif