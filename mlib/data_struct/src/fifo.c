#include "fifo.h"
#include <stdlib.h>
#include <string.h>
#ifndef NULL 
#define NULL 0
#endif




// **************************************************************
//����:FIFO_Init
//����:FIFO���͵�ָ���ַ�����д�С
//����:>=0��ʼ���ɹ�
//����:��ʼ��FIFO����
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
                //�Ѿ��ڶ����������˵�ַ
                return FIFO_ERROR_MEM;
        }
        (*fifo)->mgr.size = fifosize;
        (*fifo)->mgr.staraddr = (unsigned int)(fifo + sizeof(FIFO_MGR));               //��¼FIFO��������ʼ��ַ
        (*fifo)->mgr.endaddr = (unsigned int)(fifo+sizeof(FIFO_MGR) + fifosize -1);        //��¼FIFO������������ַ
        (*fifo)->mgr.front = (*fifo)->mgr.staraddr;  
        memset((*fifo)->buffer,0,(*fifo)->mgr.size);                                        //�����������������ݣ���ʡ��
        
        return FIFO_OK;
}
//        DMA1_Channel5->CCR &= ~DMA_CCR5_EN;
//        DMA1_Channel5->CMAR = (unsigned int)(*fifo)->staraddr;                        //����DMA�����ַ
//        DMA1_Channel5->CNDTR = (*fifo)->size;                                                //����DMA����������
//        da = USART1->DR;
//        da = da;
//        DMA1->IFCR |= 0x000F0000;
//        DMA1_Channel5->CCR |= DMA_CCR5_EN;

//D����ն��л���������
//// **************************************************************
////����:FIFO_Clear
////����:��
////����:��
////����:���FIFO����
//// **************************************************************
//int FIFO_Clear(FIFO *fifo)
//{
//        volatile unsigned int da;
//        if(fifo == NULL)
//                return FIFO_ERROR_PARAM;
//        fifo->front = fifo->staraddr;                                                                //����һ��ȡ��ַ����ΪFIFO���忪ʼ
//        DMA1_Channel5->CCR &= ~DMA_CCR5_EN;
//        DMA1_Channel5->CMAR = fifo->staraddr;                                                //��������DMA��ַ
//        DMA1_Channel5->CNDTR = fifo->size;                                                        //��������DMA����������
//        memset(fifo->buffer,0,fifo->size);
//        da = USART1->DR;
//        da = da;
//        DMA1->IFCR |= 0x000F0000;
//        DMA1_Channel5->CCR |= DMA_CCR5_EN;
//        
//        return FIFO_OK;
//}
//
//E����ȡFIFO���������������׼�Ļ��ζ��л���û����
//// **************************************************************
////����:FIFO_Read
////����:����ָ�룬1byte����ָ��
////����:>=0��ȡ�ɹ�
////����:��FIFO�����ж���1byte����
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
//F����ȡ��������������
//// **************************************************************
////����:FIFO_Status
////����:����ָ��
////����:>0��������δ��������
////����:��ȡFIFO����״̬
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