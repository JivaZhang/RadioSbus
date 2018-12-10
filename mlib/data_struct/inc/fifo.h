#pragma once
#ifndef __FIFO_H__
#define __FIFO_H__

#define FIFO_OK                  0
#define FIFO_ERROR_PARAM        -1
#define FIFO_ERROR_MEM          -2
#define FIFO_ERROR_FULL         -3
#define FIFO_ERROR_EMPTY        -4
#define FIFO_ERROR_BUSY         -5
typedef struct _fifo_mgr_type
{
    unsigned int size;                        //FIFO lengh
    unsigned int front;                        //FIFO next address
    unsigned int staraddr;                //FIFO buffer start address
    unsigned int endaddr;                        //FIFO buffer end address
}FIFO_MGR,*PFIFO_MGR;

typedef struct _fifo_type
{
      FIFO_MGR mgr;       
      unsigned char buffer[1];                //real size !(confirm when memloc)
}FIFO,*PFIFO;
int fifo_init(PFIFO *fifo,unsigned int fifosize);
#endif //__FIFO_H__
