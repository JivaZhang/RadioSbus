#ifndef __DOUBLE_BUFFER_H__
#define __DOUBLE_BUFFER_H_


enum
{
    UNLOCKED = 0,
    LOCKED = 1,
};
#pragma pack(1)
struct buffer
{
    char* head;
    char  id;
    unsigned char  size;
    unsigned char index;
};
#pragma pack()
#pragma pack(1)
struct double_buffer
{
    struct buffer buffer[2];
    char lock;
    char rd_id;
    char wt_id;
    unsigned char size;
};
#pragma pack()
struct double_buffer* create_double_buffer(unsigned int size);
int delete_double_buffer(struct double_buffer* db);
inline int lock_double_buffer(struct double_buffer* db_buffer);
inline int unlock_double_buffer(struct double_buffer* db_buffer);
int double_buffer_exchange(struct double_buffer* db_buffer);
int double_buffer_memset(struct double_buffer* db_buffer);
int double_buffer_rxbuff_radio_check(struct double_buffer* db_buffer);
int double_buffer_rd_cnt(struct double_buffer* db_buffer);
#endif 
