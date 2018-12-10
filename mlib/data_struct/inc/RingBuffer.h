#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#define RING_BUFFER_CHECK_VALUE 0xAA
#define ringbuffer_write_lock(rb) (rb->lock = RING_BUFFER_WRITE)
#define ringbuffer_read_lock(rb) (rb->lock = RING_BUFFER_READ)
#define ringbuffer_unlock(rb) (rb->lock = RING_BUUFER_IDLE)
#define ring_buffer_is_read_lock(rb) (rb->lock == RING_BUFFER_READ ? 1:0)
#define ring_buffer_is_write_lock(rb) (rb->lock == RING_BUFFER_WRITE ? 1:0)
//#define RING_BUFFER_CHECK_SWITCH    //ringbuffer的检查的开关，为1会检查ringbuffer的最后一个字节是否为RING_BUFFER_CHECK（0xFE)，以此来判定数据的溢出或者数据被覆盖
typedef enum
{
    RING_BUUFER_IDLE = 0,
    RING_BUFFER_READ = 1,
    RING_BUFFER_WRITE = 2,
    RING_BUFFER_MAX,
}RINGBUFFER_STATUS;
typedef struct ring_buffer
{
    uint32_t len;
    uint32_t out;
    uint32_t in;
    uint32_t lock;
    char     buf[1];
}RING_BUFFER,*PRING_BUFFER;

/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，pdata：写数据的首地址，num：写入的字节数
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_write_block(struct ring_buffer *rb,char *data,uint32_t num,uint32_t *num_wirtten);

/**
  * @brief  创建ringbuffer
  * @param  size：创建的ringbuffer的大小，必须为2的N次方，可以在计算数据时候提高效率
  * @retval -1: malloc数据失败。0:正确返回。参数错误：不是2的N次幂，返回1
*/
int32_t ring_buffer_new(uint32_t size,struct ring_buffer **rb);
void ring_buffer_delete(struct ring_buffer* rb);//free ring_buffer
void ring_buffer_clear(struct ring_buffer *rb);//复位ring_buffer;
uint32_t ring_buffer_available(struct ring_buffer * rb);//返回ringbuffer中存在的字节数
uint32_t ring_buffer_space(struct ring_buffer * rb);//返回ringbuffer中的剩余的空间
bool ring_buffer_empty(struct ring_buffer * rb);//判断ringbuffer是否为空
bool ring_buffer_full(struct ring_buffer *rb);//判断ringbuffer是否已满

/**
  * @brief  添加一个字节数据到ringbuffer
  * @param  rb：ringbuffer 的指针，data：要添加的数据
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-ringbuffer满
*/
int32_t ring_buffer_push(struct ring_buffer *rb,char data);

/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，pdata：数据的存储地址
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-ringbuffer空
*/       
int32_t ring_buffer_pop(struct ring_buffer *rb,char *pdata);

/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，pdata：写数据的首地址，num：写入的字节数
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_write_block(struct ring_buffer *rb,char *data,uint32_t num,uint32_t *num_wirtten);
/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，buffer：存储数据的buffer，num：读取的字节数，实际读取的字节数。
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_read_block(struct ring_buffer * rb,char* buffer,uint32_t num,uint32_t *num_read);

/**
  * @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，buffer：存储数据的buffer，num：读取的字节数，实际读取的字节数。
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
bool ring_buffer_peek_byte(struct ring_buffer * rb,char* data);

/** @brief  ringbuffer 弹出一字节的数据
  * @param  rb：ringbuffer 的指针，buffer：存储数据的buffer，num：读取的字节数，实际读取的字节数。
  * @retval -1: ringbuffer 检查出错，说明ringbuffer被覆盖或者溢出了。0：正确返回:。参数错误：1-超过ringbuffer的空间。
*/   
int32_t ring_buffer_peek_block(struct ring_buffer * rb,char* buffer,uint32_t num,uint32_t *num_read);
#endif