#ifndef __SBUS_H__
#define __SBUS_H__

#define SBUS_MSG_RX_QUEUE_SIZE    512
#define SBUS_MSG_RX_PACK_LEN      1
#define SERIAL_MSG_RX_QUEUE_SIZE    512
#define SERIAL_MSG_RX_PACK_LEN      1
#define SBUS_MSG_TX_QUEUE_SIZE    512
#define SBUS_MSG_TX_PACK_LEN      1
#define SERIAL_MSG_TX_QUEUE_SIZE    512
#define SERIAL_MSG_TX_PACK_LEN      1
#define SBUS_FRAME_HEAD 0x0F
#define SBUS_FRAME_TAIL 0x00

#define EXTERN_VARABLE_DECLEAR_SBUS \
extern xQueueHandle sbusMsgRxQHandle;  \
extern xQueueHandle sbusMsgTxQHandle; \
extern xQueueHandle serialMsgRxQHandle; \
extern xQueueHandle serialMsgTxQHandle;   \
extern struct sbusMsgParser gSbusMsgParser;
typedef enum
{
    PARSERING = 0,
    PARSER_OK = 1,
    PARSER_FAILED = 2,
    PARSER_NONE,
}parserResult_e;
typedef enum
{
    IDLE = 0,
    GOT_SBUS_FRAME_HEAD = 2,
    GOT_PAYLOAD = 3,
    GOT_SBUS_FRAME_TAIL = 4,
}sbusParserStatus_e;
typedef struct sbusMsg
{
    uint8_t sbusFrameHead;    //sbus数据头
    uint8_t payload[23];      //数据加一位标志位
    uint8_t sbusFrameTail;    //sbus 数据尾
}sbusMsg_st;

typedef struct sbusMsgParser
{
    union
    {
        sbusMsg_st sbusMsg;
        uint8_t sbusBuffer[25];
    };
    uint8_t recBytes;
    sbusParserStatus_e parserStatus;
}sbusMsgParser_st;

parserResult_e SbusParserChar(uint8_t c,sbusMsgParser_st * pMsgParser);
#endif