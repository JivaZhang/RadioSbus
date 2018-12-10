#include "fifo.h"
#include <stdlib.h>
#include <string.h>
#ifndef NULL 
#define NULL 0
#endif




// **************************************************************
//函数:FIFO_Init
//参数:FIFO类型的指针地址，队列大小
//返回:>=0初始化成功
//描述:初始化FIFO队列
// **************************************************************
int fifo_init(PFIFO *fifo,unsigned int fifosize)
{
        volatile unsigned int da;
        if(NULL == fifo || 0 == fifosize)
        {
                return FIFO_ERROR_PARAM;
        }
        (*fifo) = malloc(sizeof(FIFO_MGR)+fifosize);
        if((*fifo) == NULL)                
        {
                //已经在堆里面申请了地址
                return FIFO_ERROR_MEM;
        }
        (*fifo)->mgr.size = fifosize;
        (*fifo)->mgr.staraddr = (unsigned int)(fifo + sizeof(FIFO_MGR));               //记录FIFO缓冲区起始地址
        (*fifo)->mgr.endaddr = (unsigned int)(fifo+sizeof(FIFO_MGR) + fifosize -1);        //记录FIFO缓冲区结束地址
        (*fifo)->mgr.front = (*fifo)->mgr.staraddr;  
        memset((*fifo)->buffer,0,(*fifo)->mgr.size);                                        //清除缓冲区里面的数据，可省略
        
        return FIFO_OK;
}
//        DMA1_Channel5->CCR &= ~DMA_CCR5_EN;
//        DMA1_Channel5->CMAR = (unsigned int)(*fifo)->staraddr;                        //配置DMA传输地址
//        DMA1_Channel5->CNDTR = (*fifo)->size;                                                //配置DMA传输数据量
//        da = USART1->DR;
//        da = da;
//        DMA1->IFCR |= 0x000F0000;
//        DMA1_Channel5->CCR |= DMA_CCR5_EN;

//D：清空队列缓冲区函数
//// **************************************************************
////函数:FIFO_Clear
////参数:无
////返回:无
////描述:清空FIFO队列
//// **************************************************************
//int FIFO_Clear(FIFO *fifo)
//{
//        volatile unsigned int da;
//        if(fifo == NULL)
//                return FIFO_ERROR_PARAM;
//        fifo->front = fifo->staraddr;                                                                //将下一读取地址设置为FIFO缓冲开始
//        DMA1_Channel5->CCR &= ~DMA_CCR5_EN;
//        DMA1_Channel5->CMAR = fifo->staraddr;                                                //重新配置DMA地址
//        DMA1_Channel5->CNDTR = fifo->size;                                                        //重新配置DMA传输数据量
//        memset(fifo->buffer,0,fifo->size);
//        da = USART1->DR;
//        da = da;
//        DMA1->IFCR |= 0x000F0000;
//        DMA1_Channel5->CCR |= DMA_CCR5_EN;
//        
//        return FIFO_OK;
//}
//
//E：读取FIFO缓冲区，这个跟标准的环形队列基本没区别
//// **************************************************************
////函数:FIFO_Read
////参数:队列指针，1byte数据指针
////返回:>=0读取成功
////描述:从FIFO队列中读出1byte数据
//// **************************************************************
//int FIFO_Read(FIFO *fifo,INT8U *data)
//{
//        if(fifo == NULL )
//                return FIFO_ERROR_PARAM;
//        if(FIFO_Status(fifo)==0)
//        {
//                return FIFO_ERROR_EMPTY;
//        }
//        *data = (INT8U)(*((INT8U *)(fifo->front)));
//        if(fifo->front == fifo->endaddr)
//    {
//                fifo->front = fifo->staraddr;
//    }
//    else
//    {
//        fifo->front++;
//    }
//        return FIFO_OK;
//}
//
//F：获取缓冲区的数据量
//// **************************************************************
////函数:FIFO_Status
////参数:队列指针
////返回:>0队列中有未读出数据
////描述:获取FIFO队列状态
//// **************************************************************
//int FIFO_Status(FIFO *fifo)
//{
//        INT32S res;
//        INT32S nextsave = (INT32S)fifo->endaddr + 1 - (INT32S)DMA1_Channel5->CNDTR;
//        res = nextsave- (INT32S)(fifo->front);
//        if(res < 0)
//        {
//                res = ( (INT32S)(fifo->endaddr)+1 - (INT32S)(fifo->front) ) + (nextsave - (INT32S)fifo->staraddr);
//        }
////        return res;