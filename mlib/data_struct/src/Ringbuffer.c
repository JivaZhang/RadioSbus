#include <stdlib.h>
#include <stdint.h>
#include "type.h"
#include <RingBuffer.h>
#include <string.h>
#include "check.h"
char buffer_test[200] = {0};

typedef struct _buf_info_vec
{
    uint8_t  *data;
    uint32_t len;
}BUF_INFO_VEC,*PBUF_INFO_VEC;

typedef struct _ring_buf_info_vec
{
    BUF_INFO_VEC vec[2];
}RING_BUF_INFO_VEC,*PRING_BUF_INFO_VEC;

/*****************************inter function declare start********************/
static inline uint32_t is_power(uint32_t n);
static bool commit(struct ring_buffer *rb,uint32_t len);
static uint8_t reserve_out(struct ring_buffer *rb,PRING_BUF_INFO_VEC vec, uint32_t len);
static uint8_t reserve_in(struct ring_buffer *rb,PRING_BUF_INFO_VEC vec, uint32_t len);
static bool advance(struct ring_buffer *rb,uint32_t n);
#ifdef RING_BUFFER_CHECK_SWITCH
static inline bool ring_buffer_check(struct ring_buffer *rb);
#endif
/*****************************inter function declare end**********************/



/**
  * @brief  创建ringbuffer
  * @param  size：创建的ringbuffer的大小，必须为2的N次方，可以在计算数据时候提高效率
  * @retval -1: malloc数据失败。0:正确返回。参数错误：不是2的N次幂，返回1
*/
int32_t ring_buffer_new(uint32_t size,struct ring_buffer **rb)
{
    //检查数据的大小是否2的N次幂，不是就返回参数错误
    if(!is_power(size)) 
    {
        return 1;
    }
    struct ring_buffer* r = malloc(sizeof(uint32_t)*4 + size+1);  //在申请的空间的最后部分增加一个字节0xFE 作为检查，防止越界。
    if (!r)
    {
      return -1; //申请内存失败
    }
    
    r->len = size;
    r->in = 0;
    r->out = 0;
    r->lock = RING_BUUFER_IDLE;  //ringbuffer使用的锁，用于并发
    memset((void*)r->buf,0,size); 
    r->buf[size] = RING_BUFFER_CHECK_VALUE;//0xFE
    //rb->buf 为第一个可以使用的内存里的值，使用时候注意不要越界。s
    //sequence & （array length－1） = array index，比如一共有8槽，3&（8－1）=3，HashMap就是用这个方式来定位数组元素的，这种方式比取模的速度更快
    *rb = r;
    return 0;
}
//释放ringbuffer
void ring_buffer_delete(struct ring_buffer* rb)
{
    ASSERT(rb);
    free(rb);
}
//复位ring_buffer
void ring_buffer_clear(struct ring_buffer *rb)
{
    rb->in = 0;
    rb->out = 0;
}

#ifdef RING_BUFFER_CHECK_SWITCH
static inline bool ring_buffer_check(struct ring_buffer *rb)
{
    return (rb->buf[rb->len] == RING_BUFFER_CHECK_VALUE);
}
#endif 

//返回ringbuffer中存在的字节数
uint32_t ring_buffer_available(struct ring_buffer * rb)
{
    ASSERT(rb);
    return (rb->in - rb->out)&(rb->len - 1);
}
//返回ringbuffer中的剩余的空间
uint32_t ring_buffer_space(struct ring_buffer * rb)
{
    ASSERT(rb);
    return (rb->len - ((rb->in - rb->out)&(rb->len - 1)));
}
//判断ringbuffer是否为空
bool ring_buffer_empty(struct ring_buffer * rb)
{
    ASSERT(rb);
    return rb->in == rb->out;
}
//判断ringbuffer是否已满
bool ring_buffer_full(struct ring_buffer *rb)
{
    ASSERT(rb);
    return rb->out == (rb->in + rb->len);
}

/**
  * @brief  添加一个字节数据到ringbuffer
  * @param  rb：ringbuffer 的指针，data：要添加的数据
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-ringbuffer满
*/
int32_t ring_buffer_push(struct ring_buffer *rb,char data)
{
#ifdef RING_BUFFER_CHECK_SWITCH
    if(!ring_buffer_check(rb))
    return -1; 
#endif
    
    ASSERT(rb);
    if(ring_buffer_full(rb))
    {
        return 1;
    }
    
    rb->buf[rb->in+1] = data;    
    commit(rb,1);
    return 0;
}

/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，pdata：数据的存储地址
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-ringbuffer空
*/       
int32_t ring_buffer_pop(struct ring_buffer *rb,char *pdata)
{
#ifdef RING_BUFFER_CHECK_SWITCH
    if(ring_buffer_check(rb) == -1)
    return -1; 
#endif
    
    ASSERT(rb);  
    
    if(ring_buffer_empty(rb))
    {
        return 1;
    }
    
    *pdata = rb->buf[rb->out];
    advance(rb,1);
    return 0;
}

/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，pdata：写数据的首地址，num：写入的字节数
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_write_block(struct ring_buffer *rb,char *data,uint32_t num,uint32_t *num_wirtten)
{
    ASSERT(rb);
#ifdef RING_BUFFER_CHECK_SWITCH
    if(ring_buffer_check(rb) == -1)
    return -1; 
#endif

    uint8_t write_cnts = 0;
    RING_BUF_INFO_VEC vec;
    uint32_t ret_num = 0;
    write_cnts = reserve_in(rb,&vec,num);
    
    for(uint8_t i = 0; i < write_cnts; i++)
    {
        memcpy(vec.vec[i].data,data+ret_num,vec.vec[i].len);
        ret_num += vec.vec[i].len;
    }
    commit(rb,ret_num);
    *num_wirtten = ret_num;
    
    return 0;
}
/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，buffer：存储数据的buffer，num：读取的字节数，实际读取的字节数。
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_read_block(struct ring_buffer * rb,char* buffer,uint32_t num,uint32_t *num_read)
{
    ASSERT(rb);
#ifdef RING_BUFFER_CHECK_SWITCH
    if(ring_buffer_check(rb) == -1)
    return -1; 
#endif
    
    uint8_t read_cnts = 0;
    RING_BUF_INFO_VEC vec;
    uint32_t ret_num = 0;
    read_cnts = reserve_out(rb,&vec,num);
    
    for(uint8_t i = 0; i < read_cnts; i++)
    {
        memcpy(buffer+ret_num,vec.vec[i].data,vec.vec[i].len);
        ret_num +=vec.vec[i].len;
    }
    
    *num_read = ret_num;
    advance(rb,ret_num);
    return 0;
}
/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，buffer：存储数据的buffer，num：读取的字节数，实际读取的字节数。
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
bool ring_buffer_peek_byte(struct ring_buffer * rb,char* data)
{
    ASSERT(rb);
#ifdef RING_BUFFER_CHECK_SWITCH
    if(ring_buffer_check(rb) == -1)
    return 0; 
#endif
    if(!ring_buffer_empty(rb))
    {
        *data = rb->out;
    }
    return 1;
}
/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，buffer：存储数据的buffer，num：读取的字节数，实际读取的字节数。
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_peek_block(struct ring_buffer * rb,char* buffer,uint32_t num,uint32_t *num_read)
{
    ASSERT(rb);
#ifdef RING_BUFFER_CHECK_SWITCH
    if(ring_buffer_check(rb) == -1)
    return -1; 
#endif
    
    uint8_t read_cnts = 0;
    RING_BUF_INFO_VEC vec;
    uint32_t ret_num = 0;
    read_cnts = reserve_out(rb,&vec,num);
    
    for(uint8_t i = 0; i < read_cnts; i++)
    {
        memcpy(buffer+ret_num,vec.vec[i].data,vec.vec[i].len);
        ret_num +=vec.vec[i].len;
    }
    
    *num_read = ret_num;
    return 0;
}

//检查数据是否是2的N次幂
static  inline uint32_t is_power(uint32_t n)
{   
    
    return (n & (n - 1)) == 0 ? 1:0;  
}  

/**
  * @brief  ringbuffer 计算写数据的时候需要读写的次数，及每次写数据的起始地址和长度。
  * @param  rb：ringbuffer 的指针，vec记录每次写数据的起始地址和长度，len写入的字节数
  * @retval 返回值为需要写的次数。
*/   
static uint8_t reserve_out(struct ring_buffer *rb,PRING_BUF_INFO_VEC vec, uint32_t len)
{
    uint32_t n = ring_buffer_available(rb);

    if (len > n) {
        len = n;
    }
    
    
    vec->vec[0].data = (uint8_t*)&rb->buf[rb->out];

    n = rb->len - rb->out;//计算out到buffer最后的空间。
    if (len <= n) {
        vec->vec[0].len = len;
        return 1;
    }
    vec->vec[0].len = n;
    vec->vec[1].data = (uint8_t*)rb->buf;
    vec->vec[1].len = len - n;

    return 2;
}
/**
  * @brief  ringbuffer 计算写数据的时候需要读写的次数，及每次写数据的起始地址和长度。
  * @param  rb：ringbuffer 的指针，vec记录每次写数据的起始地址和长度，len写入的字节数
  * @retval 返回值为需要写的次数。
*/   
static uint8_t reserve_in(struct ring_buffer *rb,PRING_BUF_INFO_VEC vec, uint32_t len)
{
    uint32_t n = ring_buffer_space(rb);

    if (len > n) {
        len = n;
    }

    vec->vec[0].data = (uint8_t*)&rb->buf[rb->in];

    n = rb->len - rb->in;//计算out到buffer最后的空间。
    if (len <= n) {
        vec->vec[0].len = len;
        return 1;
    }
    vec->vec[0].len = n;
    vec->vec[1].data = (uint8_t*)rb->buf;
    vec->vec[1].len = len - n;

    return 2;
}
/**
  * @brief  ringbufferout 提交时候更新in 索引
  * @param  rb：ringbuffer 的指针，len：写入的字节数
  * @retval 0: 说明ringbuffer写的数据超出了范围。1：正确返回
*/ 
static bool commit(struct ring_buffer *rb,uint32_t len)
{
    if (len > ring_buffer_space(rb)) {
        return 0; //说明ringbuffer写的数据超出了范围
    }
    //sequence & （array length－1） = array index，比如一共有8槽，3&（8－1）=3，HashMap就是用这个方式来定位数组元素的，这种方式比取模的速度更快
    rb->in = (len + rb->in) & (rb->len -1);
    return 1;    
}

/**
  * @brief  ringbuffer提交时候更新out 索引
  * @param  rb：ringbuffer 的指针，len：读出的字节数
  * @retval 0: 说明ringbuffer读的数据超出了范围。1：正确返回 
*/ 
static bool advance(struct ring_buffer *rb,uint32_t len)
{
    if (len > ring_buffer_available(rb)) 
    {
        return 0;
    }
    
    rb->out = (len + rb->out) & (rb->len -1);
    return 1;
}

