#include "cmsis_os.h"
#include "mavlink.h"
#include "stdint.h"
#include "radio.h"
#include "usart.h"
#include "sbus.h"

xQueueHandle sbusMsgRxQHandle;    //收到sbus数据的队列指针
xQueueHandle sbusMsgTxQHandle;    //发送sbus数据的队列指针
xQueueHandle serialMsgRxQHandle;    //地面站接受到的飞控的消息处理队列 
xQueueHandle serialMsgTxQHandle;    //地面站接受到的飞控的消息处理队列 
struct sbusMsgParser gSbusMsgParser;


char debug_c = 0;
parserResult_e SbusParserChar(uint8_t c,sbusMsgParser_st * pMsgParser)
{
    parserResult_e ret;// = PARSER_FAILED;
    switch(pMsgParser->parserStatus)
    {
    case IDLE:
      if(c == SBUS_FRAME_HEAD)
      {
          pMsgParser->sbusBuffer[pMsgParser->recBytes++] = c;
          pMsgParser->parserStatus = GOT_SBUS_FRAME_HEAD;
          ret = PARSERING;
      }
      else
      {
          ret = PARSER_FAILED;
      }
      break;
    case GOT_SBUS_FRAME_HEAD:
      pMsgParser->sbusBuffer[pMsgParser->recBytes++] = c;
      if(pMsgParser->recBytes == 24)
      {
          pMsgParser->parserStatus = GOT_PAYLOAD;
      }
      ret = PARSERING;
      break;
    case GOT_PAYLOAD:
        pMsgParser->sbusBuffer[pMsgParser->recBytes++] = c;
        if(c == 0x00 || c == 0x04 || c == 0x14 || c == 0x24|| c == 0x34)
        {
            ret = PARSER_OK;              
        }
        else
        {
            ret = PARSER_FAILED;
            debug_c =   c;
        } 
        pMsgParser->recBytes = 0;
        pMsgParser->parserStatus = IDLE;

      break;
    default:
      break;
    }
    return ret;
}