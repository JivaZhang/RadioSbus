#include "cmsis_os.h"
#include "mavlink.h"
#include "stdint.h"
#include "usart.h"
#include "usermavlink.h"
#include "sbus.h"
#include "radio.h"
#define RADIO_FRAME_HEAD 'H'
#define RADIO_FRAME_TAIL 'T'
#define RADIO_MAVLINK_CHAN 0
#define RADIO_DEBUG_LEVEL 2
#define SBUS_DEBUG_LEVEL 0

typedef enum
{
    PARSER_IDLE = 0,
    PARSER_GOT_FRAME_HEAD = 1,
    PARSER_GOT_SBUS_FRAME_HEAD = 2,
    PARSER_GOT_PAYLOAD = 3,
    PARSER_GOT_SBUS_FRAME_TAIL = 4,
    PARSER_GOT_FRAME_TAIL = 5,
}RadioParserStatus_e;




typedef struct radioMsgParser
{
     union 
    {
        struct radioMsg radioMsg;
        uint8_t radioMsgBuffer[27];
    };
    uint8_t recBytes;
    RadioParserStatus_e parserStatus;
}radioMsgParser_st;

struct radioMsgParser sRadioMsgParser;
xQueueHandle radioMsgRxQHandle;    //地面站接受到的飞控的消息处理队列
osThreadId radioMsgRxTaskHandle;   //地面站接受到的飞控的消息处理任务handle
xQueueHandle radioMsgTxQHandle;    //地面站接受到的飞控的消息处理队列
osThreadId radioMsgTxTaskHandle;   //地面站接受到的飞控的消息处理任务handle  
mavlink_message_t radioMavlinkMsg;
mavlink_status_t radioMavlinkMsgStatus;
#if RADIO_DEBUG_LEVEL == 1
uint8_t serialRxBuffer[256] = {0};
uint8_t serialRxBufferIndex = 0;
#endif
#if SBUS_DEBUG_LEVEL == 1
uint8_t sbusRxBuffer[256] = {0};
uint8_t sbusRxBufferIndex = 0;
#endif
/*外部变量*************************************/
EXTERN_VARABLE_DECLEAR_SBUS;
EXTERN_VARABLE_DECLEAR_UART;

/*函数声明***************************************/
void ResetRadioParser(struct radioMsgParser * parser);
void SendMessage2Sbus(radioMsg_st * msg);
void SendMessage2Serial(struct radioMsgParser * msg);
parserResult_e RadioParserChar(uint8_t c,radioMsgParser_st * pMsgParser);
void SendMsg2Radio(uint8_t * buffer,uint8_t cnt);
void SendMavlinkMsg2Radio(mavlink_message_t * msg);
void RepackSbusMsg(sbusMsg_st * sbusMsg , radioMsg_st * radioMsg);

/*函数定义********************************************/
void ResetRadioParser(struct radioMsgParser * parser)
{
    parser->recBytes = 0;
    parser->parserStatus = PARSER_IDLE;
}
void SendMessage2Sbus(radioMsg_st * msg)
{
    
    uint16_t length;
    portBASE_TYPE xStatus;
    uint8_t *pMsg;
    const portTickType xTicksToWait = 2/portTICK_RATE_MS;
    length = 25;//the size of sbus msg is bytes;

    /*发送********************************************/
    pMsg = (uint8_t*)(&msg->sbusFrameHead);
    while(length--)
    {
        xStatus = xQueueSendToBack( sbusMsgTxQHandle,(void*)pMsg, xTicksToWait);
        if( xStatus == pdPASS)
        {
           __HAL_UART_ENABLE_IT(&huart4, UART_IT_TXE);
           pMsg++;
        }
    }    
}


parserResult_e RadioParserChar(uint8_t c,radioMsgParser_st * pMsgParser)
{
    parserResult_e ret = PARSER_FAILED;
    pMsgParser->radioMsgBuffer[pMsgParser->recBytes++] = c;
    switch(pMsgParser->parserStatus)
    {
    case PARSER_IDLE:
      if(c == RADIO_FRAME_HEAD)
      {
          pMsgParser->parserStatus = PARSER_GOT_FRAME_HEAD;
          ret = PARSERING;
      }
      break;
    case PARSER_GOT_FRAME_HEAD:
      if(c == SBUS_FRAME_HEAD)
      {
          pMsgParser->parserStatus = PARSER_GOT_SBUS_FRAME_HEAD;
          ret = PARSERING;
      }
      break;
    case PARSER_GOT_SBUS_FRAME_HEAD:
      ret = PARSERING; 
      if(pMsgParser->recBytes == 25)
      {
          pMsgParser->parserStatus = PARSER_GOT_PAYLOAD;
      }
      break;
    case PARSER_GOT_PAYLOAD:
      if(c == SBUS_FRAME_TAIL)
      {
          pMsgParser->parserStatus = PARSER_GOT_SBUS_FRAME_TAIL;
          ret = PARSERING;
      }
      break;
    case PARSER_GOT_SBUS_FRAME_TAIL:
      if(c == RADIO_FRAME_TAIL)
      {
          pMsgParser->parserStatus = PARSER_GOT_FRAME_TAIL;
          ret = PARSER_OK;
      }
      break;  
    default:
      break;
    }
    return ret;
}

void RepackSbusMsg(sbusMsg_st * sbusMsg , radioMsg_st * radioMsg)
{
    radioMsg->frameHead = (uint8_t)RADIO_FRAME_HEAD;
    radioMsg->frameEnd = (uint8_t)RADIO_FRAME_TAIL;
    radioMsg->sbusFrameHead = sbusMsg->sbusFrameHead;
    radioMsg->sbusFrameTail = sbusMsg->sbusFrameTail;    
    for(uint8_t i = 0; i< 23; i++)
    {
        radioMsg->payload[i] = sbusMsg->payload[i];
    }
}
void DecodeRadioSbusMsg(sbusMsg_st * sbusMsg , radioMsg_st * radioMsg)
{
    sbusMsg->sbusFrameHead = radioMsg->sbusFrameHead;
    sbusMsg->sbusFrameTail = radioMsg->sbusFrameTail;    
    for(uint8_t i = 0; i< 23; i++)
    {
        sbusMsg->payload[i] = radioMsg->payload[i];
    }    
}
void SendMsg2Radio(uint8_t * buffer,uint8_t cnt)
{
    portBASE_TYPE xStatus;
    uint8_t *pMsg;
    
    const portTickType xTicksToWait = 0/portTICK_RATE_MS;
    pMsg = buffer;
    while(cnt--)
    {
        xStatus = xQueueSendToBack( radioMsgTxQHandle,(void*)pMsg, xTicksToWait);
        if( xStatus == pdPASS)
        {
           __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
           pMsg++;
        }
    }    
}
void SendMavlinkMsg2Radio(mavlink_message_t * msg)
{
    uint16_t length;
    portBASE_TYPE xStatus;
    uint8_t *pMsg;
    
    const portTickType xTicksToWait = 2/portTICK_RATE_MS;
    length = msg->len + 6;//the size of msg header except checksum is 6 bytes;
    pMsg = (uint8_t*)(&(msg->magic));
    
    /*发送msg头和payload********************************************/
    pMsg = (uint8_t*)(&(msg->magic));
    while(length--)
    {
        xStatus = xQueueSendToBack( radioMsgTxQHandle,(void*)pMsg, xTicksToWait);
        if( xStatus == pdPASS)
        {
           __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
           pMsg++;
        }
    }    
    
    /*发送checksum********************************/
    pMsg = (uint8_t*)(&msg->checksum);
    xStatus = xQueueSendToBack( radioMsgTxQHandle,(void*)pMsg, xTicksToWait);
    pMsg++;
    if( xStatus == pdPASS)
    {
       __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
    }
    xStatus = xQueueSendToBack( radioMsgTxQHandle,(void*)pMsg, xTicksToWait);
    if( xStatus == pdPASS)
    {
       __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
    }    
}
void SendMessage2Serial(struct radioMsgParser * msg)
{
    uint16_t length;
    portBASE_TYPE xStatus;
    uint8_t *pMsg;
    
    const portTickType xTicksToWait = 0/portTICK_RATE_MS;
    length = msg->recBytes;//msg->len + 8;//the size of msg except payload is 8 bytes;
    pMsg = (uint8_t*)msg->radioMsgBuffer;
    while(length--)
    {
        xStatus = xQueueSendToBack( serialMsgTxQHandle,(void*)pMsg, xTicksToWait);
        if( xStatus == pdPASS)
        {
           __HAL_UART_ENABLE_IT(&huart3, UART_IT_TXE);
           pMsg++;
        }
    }
}
/*函数定义 -----------------------------------------------------------*/
void RadioMsgRxTask(void const * argument)
{
    /*用于保存从队列中接收到的数据*/
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 10/portTICK_RATE_MS;
    char c;
    parserResult_e received = PARSER_NONE;
    for(;;)
    {
        if(uxQueueMessagesWaiting( radioMsgRxQHandle ) != 0)
        {
            xStatus = xQueueReceive( radioMsgRxQHandle, &c, xTicksToWait);
            if(xStatus == pdPASS)
            {
                received = RadioParserChar(c, &sRadioMsgParser);
                switch(received)
                {
                case PARSER_OK:   //接受到一帧sbus数据  
                  SendMessage2Sbus(&sRadioMsgParser.radioMsg);
                  ResetRadioParser(&sRadioMsgParser);
                  break;
                case PARSER_FAILED: //接受到一组radio数据
                  SendMessage2Serial(&sRadioMsgParser);
                  ResetRadioParser(&sRadioMsgParser);
                  break;
                case PARSERING:
                default:
                  break;
                }
            }    
        }
    }
}


unsigned int  recBad = 0;
unsigned int recOK = 0;

/*函数定义 -----------------------------------------------------------*/
void RadioMsgTxTask(void const * argument)
{
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 0/portTICK_RATE_MS;
    char c;
    uint8_t received = PARSER_NONE;
    radioMsg_st radioMsg;
    for(;;)
    {
        if(uxQueueMessagesWaiting( serialMsgRxQHandle ) != 0)
        {
            xStatus = xQueueReceive( serialMsgRxQHandle, &c, xTicksToWait);
            if(xStatus == pdPASS)
            {
#if RADIO_DEBUG_LEVEL == 1
                serialRxBuffer[serialRxBufferIndex++] = c;
#elif RADIO_DEBUG_LEVEL == 2
                xStatus = xQueueSendToBack( radioMsgTxQHandle,(void*)&c, xTicksToWait);
                if( xStatus == pdPASS)
                {
                   __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
                }             
#else
                received = mavlink_parse_char((uint8_t)RADIO_MAVLINK_CHAN,c, &radioMavlinkMsg, &radioMavlinkMsgStatus);
                if(received == MAVLINK_FRAMING_OK)
                {
                    SendMavlinkMsg2Radio(&radioMavlinkMsg);
                }  
#endif
            }    
        }
        
        if(uxQueueMessagesWaiting( sbusMsgRxQHandle ) != 0)
        {
            xStatus = xQueueReceive( sbusMsgRxQHandle, &c, xTicksToWait);
            if(xStatus == pdPASS)
            {
#if SBUS_DEBUG_LEVEL == 1
                sbusRxBuffer[sbusRxBufferIndex++] = c;
#else           
                received = SbusParserChar(c, &gSbusMsgParser);
                if(received == PARSER_OK)
                {
                    RepackSbusMsg( &gSbusMsgParser.sbusMsg ,&radioMsg );
                    SendMsg2Radio( (uint8_t*)&radioMsg,sizeof(radioMsg_st) );
                    recOK++;
                }
                if(received == PARSER_FAILED)
                {
                    recBad++;
                }
#endif
            }
        }
    }
}