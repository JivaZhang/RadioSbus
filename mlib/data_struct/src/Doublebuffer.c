#include <stdlib.h>
#include "Doublebuffer.h"
#include <string.h>
struct double_buffer* create_double_buffer(unsigned int size)
{
   struct double_buffer* db = malloc(sizeof(struct double_buffer));
   char* bf1 = malloc(size); 
   memset((void*)bf1,0,size);
   char* bf2 = malloc(size);  
   memset((void*)bf2,0,size);
   db->buffer[0].head = bf1;
   db->buffer[0].id = 0;
   db->buffer[0].size = size;
   db->buffer[0].index = 0;

   db->buffer[1].head = bf2;
   db->buffer[1].id = 1;
   db->buffer[1].size = size;
   db->buffer[1].index = 0;
   db->lock = UNLOCKED; 
   db->rd_id = 1; 
   db->wt_id = 0;
   db->size = size;
   return db;
}

int delete_double_buffer(struct double_buffer* db)
{
   free(db->buffer[0].head); 
   free(db->buffer[1].head);
   free(db);
   return 0;
}

inline int lock_double_buffer(struct double_buffer* db_buffer)
{
    db_buffer->lock = LOCKED;
    return 1;
}

inline int unlock_double_buffer(struct double_buffer* db_buffer)
{
    db_buffer->lock = UNLOCKED;
    return 1;
}

int double_buffer_exchange(struct double_buffer* db_buffer)
{
    char temp;
    lock_double_buffer(db_buffer);
    temp = db_buffer->rd_id;
    db_buffer->rd_id = db_buffer->wt_id; 
    db_buffer->wt_id = temp;
    unlock_double_buffer(db_buffer);
    return 1;
}

int double_buffer_memset(struct double_buffer* db_buffer)
{
    memset((void*)(&db_buffer->buffer[db_buffer->rd_id]),0,db_buffer->size);
    return 1;
}

int double_buffer_rxbuff_radio_check(struct double_buffer* db_buffer)
{
    return  (db_buffer->buffer[db_buffer->wt_id].index >= 120 ? 1 : 0);
}
int double_buffer_rd_cnt(struct double_buffer* db_buffer)
{
    return db_buffer->buffer[db_buffer->wt_id].index;
}
