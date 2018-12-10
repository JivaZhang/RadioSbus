#ifndef __GPS__H__
#define __GPS__H___


#define GPS_CMD_LINK_MAX_SIZE  20

typedef int (*FUN_INT_INT)(int cmd);
typedef int (*FUN_INT_VOID)(void);
typedef int (*FUN_INT_VOIDP)(void*);
typedef int (*FUN_INT_VOIDPC)(void*,char c);
struct _LLA
{
    double latitude;
    double longitude;
    double altitude;
};

typedef struct _postion_info
{
    double latitude;
    double longitude;
    double altitude;
    double pdop;
    char position_flag;
}POSTION_INFO;

struct _cmd_rev
{
    int  offset;
    int size;
    char  *buf;
};

enum
{
    PREPARE_SEND = 0,
    SEND_CMD = 1,
    WAIT_DATA = 2,
    DISPOSE =3,
    CMD_MAX
};
typedef enum
{
    GPS_CMD_RECEIVE = 0,
    GPS_PREPARE_SEND,
    GPS_CMD_SEND_CMD,
    GPS_WAIT_DATA,
    GPS_DISPOSE_DATA,
    GPS_RELEASE,
}ROVER_BASE_GPS;
typedef int (*FUNCTION)(void*,void*);
#pragma pack(1)
struct _gps_cmd
{
    int cmd_id;
    unsigned int start_tick;
    unsigned int timeout;
    unsigned char data_receive_done;
    unsigned char cmd_dispose_done;
    FUN_INT_VOIDP prepare_send_cmd;
    FUN_INT_VOIDP send_cmd;
    FUN_INT_VOIDPC receive_cmd;
    FUN_INT_VOIDP wait_cmd;
    FUN_INT_VOIDP dispose_cmd;
    FUN_INT_VOIDP release;
    struct _cmd_rev cmd_rev;
};
struct _cmd_link
{
    struct _cmd_link* head;
    struct _gps_cmd gps_cmd;
    struct _cmd_link* next;
};

struct _cmd_mgmt
{
    struct _cmd_link cmd_link;
    void*  add_cmd;
    void* delete_cmd;
    int current_link_size;
    int cmd_max_size;
};
#pragma pack()
int add_cmd(void *this,void *cmd);
int dispose_read_LLA(void* this);
int wait_read_LLA(void* this);
int send_read_LLA_cmd(void* this);
int receive_data_LLA(void* pthis,char c);
struct _cmd_mgmt *create_cmd_mngmt(int cmd_limit);
int read_LLA_prepare_send_cmd(void *pthis);
int release(void* pthis);
int set_LLA_send_cmd(void *pthis);
int set_LLA_prepare_send_cmd(void *pthis);
int gps_get_position_info(char *buff ,unsigned char size,struct _postion_info* pp,unsigned char format);
void gps_cmd_receive(void);
void gps_prepare_send_cmd(void);
void gps_send_cmd(void);
void gps_wait_reply(void);
void gps_dispose_data(void);
void gps_release(void);
int gps_get_LLA(char *buff ,unsigned char size,struct _LLA* plla,unsigned char format);
#endif
