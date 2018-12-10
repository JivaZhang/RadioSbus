#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#define RING_BUFFER_CHECK_VALUE 0xAA
#define ringbuffer_write_lock(rb) (rb->lock = RING_BUFFER_WRITE)
#define ringbuffer_read_lock(rb) (rb->lock = RING_BUFFER_READ)
#define ringbuffer_unlock(rb) (rb->lock = RING_BUUFER_IDLE)
#define ring_buffer_is_read_lock(rb) (rb->lock == RING_BUFFER_READ ? 1:0)
#define ring_buffer_is_write_lock(rb) (rb->lock == RING_BUFFER_WRITE ? 1:0)
//#define RING_BUFFER_CHECK_SWITCH    //ringbuffer�ļ��Ŀ��أ�Ϊ1����ringbuffer�����һ���ֽ��Ƿ�ΪRING_BUFFER_CHECK��0xFE)���Դ����ж����ݵ�����������ݱ�����
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
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬pdata��д���ݵ��׵�ַ��num��д����ֽ���
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
*/   
int32_t ring_buffer_write_block(struct ring_buffer *rb,char *data,uint32_t num,uint32_t *num_wirtten);

/**
  * @brief  ����ringbuffer
  * @param  size��������ringbuffer�Ĵ�С������Ϊ2��N�η��������ڼ�������ʱ�����Ч��
  * @retval -1: malloc����ʧ�ܡ�0:��ȷ���ء��������󣺲���2��N���ݣ�����1
*/
int32_t ring_buffer_new(uint32_t size,struct ring_buffer **rb);
void ring_buffer_delete(struct ring_buffer* rb);//free ring_buffer
void ring_buffer_clear(struct ring_buffer *rb);//��λring_buffer;
uint32_t ring_buffer_available(struct ring_buffer * rb);//����ringbuffer�д��ڵ��ֽ���
uint32_t ring_buffer_space(struct ring_buffer * rb);//����ringbuffer�е�ʣ��Ŀռ�
bool ring_buffer_empty(struct ring_buffer * rb);//�ж�ringbuffer�Ƿ�Ϊ��
bool ring_buffer_full(struct ring_buffer *rb);//�ж�ringbuffer�Ƿ�����

/**
  * @brief  ���һ���ֽ����ݵ�ringbuffer
  * @param  rb��ringbuffer ��ָ�룬data��Ҫ��ӵ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-ringbuffer��
*/
int32_t ring_buffer_push(struct ring_buffer *rb,char data);

/**
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬pdata�����ݵĴ洢��ַ
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-ringbuffer��
*/       
int32_t ring_buffer_pop(struct ring_buffer *rb,char *pdata);

/**
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬pdata��д���ݵ��׵�ַ��num��д����ֽ���
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
*/   
int32_t ring_buffer_write_block(struct ring_buffer *rb,char *data,uint32_t num,uint32_t *num_wirtten);
/**
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬buffer���洢���ݵ�buffer��num����ȡ���ֽ�����ʵ�ʶ�ȡ���ֽ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
*/   
int32_t ring_buffer_read_block(struct ring_buffer * rb,char* buffer,uint32_t num,uint32_t *num_read);

/**
  * @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬buffer���洢���ݵ�buffer��num����ȡ���ֽ�����ʵ�ʶ�ȡ���ֽ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
*/   
bool ring_buffer_peek_byte(struct ring_buffer * rb,char* data);

/** @brief  ringbuffer ����һ�ֽڵ�����
  * @param  rb��ringbuffer ��ָ�룬buffer���洢���ݵ�buffer��num����ȡ���ֽ�����ʵ�ʶ�ȡ���ֽ�����
  * @retval -1: ringbuffer ������˵��ringbuffer�����ǻ�������ˡ�0����ȷ����:����������1-����ringbuffer�Ŀռ䡣
*/   
int32_t ring_buffer_peek_block(struct ring_buffer * rb,char* buffer,uint32_t num,uint32_t *num_read);
#endif