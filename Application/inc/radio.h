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
    uint8_t frameHead;     //��sbus���ݴ����֡ͷ
    uint8_t sbusFrameHead;    //sbus����ͷ
    uint8_t payload[23];      //���ݼ�һλУ��λ
    uint8_t sbusFrameTail;    //sbus ����β
    uint8_t frameEnd;      //��sbus���ݴ����֡β
}radioMsg_st;


/*��������***************************************************************/
/**
* @brief  ���������ܵ���Ϣ�Ĵ�����
* @param  
* @param  
* @retval 
* @note   Ŀǰ�ǰѽ��ܵ�������ֱ�ӷ��͵��ɿ�
*/
void RadioMsgRxTask(void const * argument);

/*��������***************************************************************/
/**
* @brief  ������Ϣ������
* @param  
* @param  
* @retval 
* @note   
*/
void RadioMsgTxTask(void const * argument);

/***********************************/
#endif