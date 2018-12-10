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
  * @brief  ����ringbuffer
  * @param  size��������ringbuffer�Ĵ�С������Ϊ2��N�η��������ڼ�������ʱ�����Ч��
  * @retval -1: malloc����ʧ�ܡ�0:��ȷ���ء��������󣺲���2��N���ݣ�����1
*/
int32_t ring_buffer_new(uint32_t size,struct ring_buffer **rb)
{
    //������ݵĴ�С�Ƿ�2��N���ݣ����Ǿͷ��ز�������
    if(!is_power(size)) 
    {
        return 1;
    }
    struct ring_buffer* r = malloc(sizeof(uint32_t)*4 + size+1);  //������Ŀռ����󲿷�����һ���ֽ�0xFE ��Ϊ��飬��ֹԽ�硣
    if (!r)
    {
      return -1; //�����ڴ�ʧ��
    }
    
    r->len = size;
    r->in = 0;
    r->out = 0;
    r->lock = RING_BUUFER_IDLE;  //ringbufferʹ�õ��������ڲ���
    memset((void*)r->buf,0,size); 
    r->buf[size] = RING_BUFFER_CHECK_VALUE;//0xFE
    //rb->buf Ϊ��һ������ʹ�õ��ڴ����ֵ��ʹ��ʱ��ע�ⲻҪԽ�硣s
    //sequence & ��array length��1�� = array index������һ����8�ۣ�3&��8��1��=3��HashMap�����������ʽ����λ����Ԫ�صģ����ַ�ʽ��ȡģ���ٶȸ���
    *rb = r;
    return 0;
}
//�ͷ�ringbuffer
void ring_buffer_delete(struct ring_buffer* rb)
{
    ASSERT(rb);
    free(rb);
}
//��λring_buffer
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

//����ringbuffer�д��ڵ��ֽ���
uint32_t ring_buffer_available(struct ring_buffer * rb)
{
    ASSERT(rb);
    return (rb->in - rb->out)&(rb->len - 1);
}
//����ringbuffer�е�ʣ��Ŀռ�
uint32_t ring_buffer_space(struct ring_buffer * rb)
{
    ASSERT(rb);
    return (rb->len - ((rb->in - rb->out)&(rb->len - 1)));
}
//�ж�ringbuffer�Ƿ�Ϊ��
bool ring_buffer_empty(struct ring_buffer * rb)
{
    ASSERT(rb);
    return rb->in == rb->out;
}
//�ж�ringbuffer�Ƿ�����
bool ring_buffer_full(struct ring_buffer *rb)
{
    ASSERT(rb);
    return rb->out == (rb->in + rb->len);
}

/**
  * @brief  ���һ���ֽ����ݵ�ringbuffer
  * @param  rb��ringbuffer ��ָ�룬data��Ҫ��ӵ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-ringbuffer��
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
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬pdata�����ݵĴ洢��ַ
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-ringbuffer��
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
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬pdata��д���ݵ��׵�ַ��num��д����ֽ���
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
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
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬buffer���洢���ݵ�buffer��num����ȡ���ֽ�����ʵ�ʶ�ȡ���ֽ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
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
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬buffer���洢���ݵ�buffer��num����ȡ���ֽ�����ʵ�ʶ�ȡ���ֽ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
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
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬buffer���洢���ݵ�buffer��num����ȡ���ֽ�����ʵ�ʶ�ȡ���ֽ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
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

//��������Ƿ���2��N����
static  inline uint32_t is_power(uint32_t n)
{   
    
    return (n & (n - 1)) == 0 ? 1:0;  
}  

/**
  * @brief  ringbuffer ����д���ݵ�ʱ����Ҫ��д�Ĵ�������ÿ��д���ݵ���ʼ��ַ�ͳ��ȡ�
  * @param  rb��ringbuffer ��ָ�룬vec��¼ÿ��д���ݵ���ʼ��ַ�ͳ��ȣ�lenд����ֽ���
  * @retval ����ֵΪ��Ҫд�Ĵ�����
*/   
static uint8_t reserve_out(struct ring_buffer *rb,PRING_BUF_INFO_VEC vec, uint32_t len)
{
    uint32_t n = ring_buffer_available(rb);

    if (len > n) {
        len = n;
    }
    
    
    vec->vec[0].data = (uint8_t*)&rb->buf[rb->out];

    n = rb->len - rb->out;//����out��buffer���Ŀռ䡣
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
  * @brief  ringbuffer ����д���ݵ�ʱ����Ҫ��д�Ĵ�������ÿ��д���ݵ���ʼ��ַ�ͳ��ȡ�
  * @param  rb��ringbuffer ��ָ�룬vec��¼ÿ��д���ݵ���ʼ��ַ�ͳ��ȣ�lenд����ֽ���
  * @retval ����ֵΪ��Ҫд�Ĵ�����
*/   
static uint8_t reserve_in(struct ring_buffer *rb,PRING_BUF_INFO_VEC vec, uint32_t len)
{
    uint32_t n = ring_buffer_space(rb);

    if (len > n) {
        len = n;
    }

    vec->vec[0].data = (uint8_t*)&rb->buf[rb->in];

    n = rb->len - rb->in;//����out��buffer���Ŀռ䡣
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
  * @brief  ringbufferout �ύʱ�����in ����
  * @param  rb��ringbuffer ��ָ�룬len��д����ֽ���
  * @retval 0: ˵��ringbufferд�����ݳ����˷�Χ��1����ȷ����
*/ 
static bool commit(struct ring_buffer *rb,uint32_t len)
{
    if (len > ring_buffer_space(rb)) {
        return 0; //˵��ringbufferд�����ݳ����˷�Χ
    }
    //sequence & ��array length��1�� = array index������һ����8�ۣ�3&��8��1��=3��HashMap�����������ʽ����λ����Ԫ�صģ����ַ�ʽ��ȡģ���ٶȸ���
    rb->in = (len + rb->in) & (rb->len -1);
    return 1;    
}

/**
  * @brief  ringbuffer�ύʱ�����out ����
  * @param  rb��ringbuffer ��ָ�룬len���������ֽ���
  * @retval 0: ˵��ringbuffer�������ݳ����˷�Χ��1����ȷ���� 
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

