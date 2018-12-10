#include <stdlib.h>
#include <string.h>
#include "gps.h"
#include "usart.h"
#include "Doublebuffer.h"

#define GPS_FRAME_START 0x02
#define GPS_FRAME_END  0x03
const double PI = 3.14159265;
enum
{
    FILE_CTRL_BLOCK_START = 0,
    FILE_CTRL_BLOCK_GPS_STATUS,
    FILE_CTRL_BLOCK_TYPE,
    FILE_CTRL_BLOCK_LEN,
    FILE_CTRL_BLOCK_RECEIVE_DATA,
    FILE_CTRL_BLOCK_CHECKSUM,
    FILE_CTRL_BLOCK_FRAME_END,
    DATA_BLOCK_START,
    DATA_BLOCK_GPS_STATUS,
    DATA_BLOCK_TYPE,
    DATA_BLOCK_LEN,
    DATA_BLOCK_RECEIVE_DATA,
    DATA_BLOCK_CHECKSUM,
    DATA_BLOCK_FRAME_END,
};
extern UART_HandleTypeDef huart6;
char usart6_cmd_mode = 0;
extern struct _cmd_rev *cmd_buffer;
extern struct double_buffer* usart6_rx_db;
struct _cmd_mgmt *gps_cmd_mgmt;
char gps_get_postion_flag_code(char flag);
int create_cmd_link(struct _cmd_link **cmd_link);
int delete_cmd(struct _cmd_mgmt *this);
void send_cmd(char* data,int size);
double motor2inter_double(double m);
long motor2inter_long(unsigned long m);
struct _LLA g_lla;
ROVER_BASE_GPS     g_gps_index;
int add_cmd(void *pthis,void *pcmd)
{
    
    struct _cmd_mgmt *this; 
    struct _gps_cmd  *cmd;
    this = (struct _cmd_mgmt *)pthis;
    cmd = (struct _gps_cmd  *)pcmd;
    if(this->current_link_size == this->cmd_max_size)
    {
        return -1;
    }
    struct _cmd_link *link;
    struct _cmd_link *new_node;
    link = &(this->cmd_link);
    
    while(link->next)
    {
        link = link->next;
    }
    //new_node  = malloc(sizeof(struct _cmd_link));
    create_cmd_link(&new_node);
    link->next = new_node;
    new_node->head = link;
    new_node->next = ((void*)0);
    new_node->gps_cmd.cmd_id = cmd->cmd_id;
    new_node->gps_cmd.receive_cmd = cmd->receive_cmd;
    new_node->gps_cmd.prepare_send_cmd = cmd->prepare_send_cmd;
    new_node->gps_cmd.send_cmd = cmd->send_cmd;
    new_node->gps_cmd.wait_cmd = cmd->wait_cmd;
    new_node->gps_cmd.dispose_cmd = cmd->dispose_cmd;
    new_node->gps_cmd.release = cmd->release;
    new_node->gps_cmd.data_receive_done = cmd->data_receive_done;
    new_node->gps_cmd.cmd_dispose_done = cmd->cmd_dispose_done;
    new_node->gps_cmd.timeout = cmd->timeout;
    return 0;
}

int delete_cmd(struct _cmd_mgmt *this)
{
    return 0;
}
int create_cmd_link(struct _cmd_link **cmd_link)
{
    *cmd_link  = malloc(sizeof(struct _cmd_link));
    if(cmd_link)
    {
        return 0;
    }
    return -1;
}
struct _cmd_mgmt *create_cmd_mngmt(int cmd_limit)
{
    struct _cmd_mgmt *cmd_mgmt;
    cmd_mgmt = malloc(sizeof(struct _cmd_mgmt));
    
    cmd_mgmt->cmd_link.head = ((struct _cmd_link*)0);
    //cmd_mgmt->cmd_link.gps_cmd = ((struct _gps_cmd *)0);
    cmd_mgmt->cmd_link.next = ((struct _cmd_link*)0);
       
    cmd_mgmt->add_cmd = (void*)add_cmd;
    cmd_mgmt->delete_cmd = (void*)delete_cmd;
    cmd_mgmt->cmd_max_size = cmd_limit;
    cmd_mgmt->current_link_size = 0;
    return cmd_mgmt;
}

struct _cmd_rev *prepare_send_cmd(int size)
{
    char *cmd_rev = malloc(size+ sizeof(struct _cmd_rev));
    struct _cmd_rev *rev;
    if(cmd_rev)
    {
        rev = (struct _cmd_rev*)cmd_rev;
        rev->offset = 0;
        rev->size = size;
        rev->buf = cmd_rev + sizeof(struct _cmd_rev);
        memset(rev->buf,0,rev->size);
        return rev;
    }
    return ((struct _cmd_rev *)0);
}
int read_LLA_prepare_send_cmd(void *pthis)
{
     struct _cmd_mgmt *this; 
     this = (struct _cmd_mgmt *)pthis;
     struct _cmd_rev *rev = prepare_send_cmd(200);//118
     this->cmd_link.next->gps_cmd.cmd_rev.size = rev->size;
     this->cmd_link.next->gps_cmd.cmd_rev.buf = rev->buf;
     this->cmd_link.next->gps_cmd.cmd_rev.offset = rev->offset;
     cmd_buffer = &(this->cmd_link.next->gps_cmd.cmd_rev);
     return 0;
}
    
int set_LLA_prepare_send_cmd(void *pthis)
{
     struct _cmd_mgmt *this; 
     this = (struct _cmd_mgmt *)pthis;
     struct _cmd_rev *rev = prepare_send_cmd(2);//118
     this->cmd_link.next->gps_cmd.cmd_rev.size = rev->size;
     this->cmd_link.next->gps_cmd.cmd_rev.buf = rev->buf;
     this->cmd_link.next->gps_cmd.cmd_rev.offset = rev->offset;
     cmd_buffer = &(this->cmd_link.next->gps_cmd.cmd_rev);
     return 0;    
}

typedef union _STN_SET
{
#pragma pack(1)
    struct _S_STN_SET
    {
        char record_type;
        char record_length;
        char flag;
        char nodex_index;
        char statition_name[8];
        double latitude;
        double longidude;
        double altitude;
        short station_id;
        char cmr_station_id;
        char frame_char1;
        char station_name_indicator;
        char cmr_station_name[20];
        char station_code_id;
        char station_code[16];
        char reserve1;
        char reserve2;
        char reserve3;
        char reserve4;
        char reserve5;
        char reserve6;
        char reserve7;
        char reserve8;
        char reserve9;
        char reserve10;
        char frame_char2;
        short rtcm_id;
    }stn_set_stct;
#pragma pack()
    unsigned char stn_set_array[93];
}STN_SET;

int set_LLA_send_cmd(void *pthis)
{
    STN_SET station_set;
    char head[110];
    unsigned char checksum = 0;
    
    memset((void*)&station_set,0,sizeof(STN_SET));
    station_set.stn_set_stct.record_type = 0x03;
    station_set.stn_set_stct.record_length = 0x59;
    station_set.stn_set_stct.flag = 0;
    station_set.stn_set_stct.nodex_index = 0;
    
    strcpy(station_set.stn_set_stct.statition_name,"base");
    station_set.stn_set_stct.latitude = g_lla.latitude*PI;
    station_set.stn_set_stct.longidude  = g_lla.longitude*PI;
    station_set.stn_set_stct.latitude = motor2inter_double(station_set.stn_set_stct.latitude);
    station_set.stn_set_stct.longidude = motor2inter_double(station_set.stn_set_stct.longidude);
    station_set.stn_set_stct.altitude = motor2inter_double(g_lla.altitude);
    station_set.stn_set_stct.station_id = 0x0200;
    station_set.stn_set_stct.cmr_station_id = 1;
    station_set.stn_set_stct.frame_char1 = 0x40;
    station_set.stn_set_stct.station_name_indicator = 0x41;
    strcpy(station_set.stn_set_stct.cmr_station_name,"base");
    station_set.stn_set_stct.station_code_id = 0x42;
    strcpy(station_set.stn_set_stct.station_code,"base");
    station_set.stn_set_stct.reserve1 = 0x44;
    station_set.stn_set_stct.reserve2 = 0x41;
    station_set.stn_set_stct.reserve3 = 0x4E;
    station_set.stn_set_stct.reserve4 = 0x4D;
    station_set.stn_set_stct.reserve5 = 0x43;
    station_set.stn_set_stct.reserve6 = 0x34;
    station_set.stn_set_stct.reserve7 = 0x20;
    station_set.stn_set_stct.reserve8 = 0x20;
    station_set.stn_set_stct.reserve9 = 0x20;
    station_set.stn_set_stct.reserve10 = 0x20;
    station_set.stn_set_stct.frame_char2 = 0x40;
    station_set.stn_set_stct.rtcm_id = 0x0300; 
    head[0] = 0x02;
    head[1] = 0x00;
    head[2] = 0x64;
    head[3] = 0x62;
    head[4] = 0x00;
    head[5] = 0x00;
    head[6] = 0x00;
    head[7] = 0x03;
    head[8] = 0x00;
    head[9] = 0x01;
    head[10] = 0x00;
    
    memcpy(&head[11],station_set.stn_set_array,91);
    
    /*head[11] = 0x03;
    head[12] = 0x59;
    head[13] = 0x00;
    head[14] = 0x00;
    head[15] = 0x42;
    head[16] = 0x41;
    head[17] = 0x53;
    head[18] = 0x45;
    head[19] = 0x20;
    
    head[20] = 0x20;
    head[21] = 0x20;
    head[22] = 0x20;
    head[23] = 0x3F;
    head[24] = 0xE6;
    head[25] = 0x48;
    head[26] = 0x80;
    head[27] = 0x57;
    head[28] = 0x7F;
    head[29] = 0xA2;
    
    head[30] = 0x1F;
    head[31] = 0xBF;
    head[32] = 0xFD;
    head[33] = 0x5A;
    head[34] = 0x5D;
    head[35] = 0xDD;
    head[36] = 0x52;
    head[37] = 0xDF;
    head[38] = 0x68;
    head[39] = 0x40;
   
    head[40] = 0x9A;
    head[41] = 0x04;
    head[42] = 0x9C;
    head[43] = 0xAC;
    head[44] = 0x08;
    head[45] = 0x31;
    head[46] = 0x27;
    head[47] = 0x00;
    head[48] = 0x02;
    head[49] = 0x01;
    
    head[50] = 0x40;
    head[51] = 0x41;
    head[52] = 0x42;
    head[53] = 0x41;
    head[54] = 0x53;
    head[55] = 0x45;
    head[56] = 0x20;
    head[57] = 0x20;
    head[58] = 0x20;
    head[59] = 0x20;
    
    head[60] = 0x20;
    head[61] = 0x20;
    head[62] = 0x20;
    head[63] = 0x20;
    head[64] = 0x20;
    head[65] = 0x20;
    head[66] = 0x20;
    head[67] = 0x20;
    head[68] = 0x20;
    head[69] = 0x20;
    
    head[70] = 0x20;
    head[71] = 0x20;
    head[72] = 0x42;
    head[73] = 0x74;
    head[74] = 0x65;
    head[75] = 0x73;
    head[76] = 0x74;
    head[77] = 0x5F;
    head[78] = 0x62;
    head[79] = 0x61;
    
    head[80] = 0x73;
    head[81] = 0x65;
    head[82] = 0x20;
    head[83] = 0x20;
    head[84] = 0x20;
    head[85] = 0x20;
    head[86] = 0x20;
    head[87] = 0x20;
    head[88] = 0x20;
    head[89] = 0x44;
    head[90] = 0x41;
    
    head[91] = 0x4E;
    head[92] = 0x4D;
    head[93] = 0x43;
    head[94] = 0x34;
    head[95] = 0x20;
    head[96] = 0x20;
    head[97] = 0x20;
    head[98] = 0x20;
    head[99] = 0x40;
    head[100] = 0x00;
    head[101] = 0x03;*/
    
    for(unsigned char index = 1; index < 102; index++)
    {
        checksum += head[index];
    }
    head[102] = checksum;
    head[103] = 0x03;
    gps_cmd_mgmt->cmd_link.next->gps_cmd.start_tick = HAL_GetTick();
    usart6_cmd_mode = 0;
    send_cmd(head,105);
    return 0;
}

int read_utc_prepare_send_cmd(void *pthis)
{
     struct _cmd_mgmt *this; 
     this = (struct _cmd_mgmt *)pthis;
     struct _cmd_rev *rev = prepare_send_cmd(200);
     this->cmd_link.gps_cmd.cmd_rev.size = rev->size;
     this->cmd_link.gps_cmd.cmd_rev.buf = rev->buf;
     this->cmd_link.gps_cmd.cmd_rev.offset = rev->offset;
     cmd_buffer = &(this->cmd_link.gps_cmd.cmd_rev);
     return 0;
}

int send_read_utc_cmd(void)
{
    char head[50];
    //transfer head
    head[0] = 0x02; //start transmission
    head[1] = 0x00; //receiver status code 
    head[2] = 0x56; //command packet type 
    head[3] = 0x03; //data  byte count 
    head[4] = 0x01; //type raw data. 00 real-time sruver data record,01 position record.
    head[5] = 0x02; //flag
    
    //file control block 
    head[5] = 0x00; //page index
    head[6] = 0x00; //maximum page index
    head[7] = 0x00; //application file specification version ,always 3 in ths version
    head[8] = 0x00; //device type unique identifier for every receiver/device type that support s the application file interface 
    head[9] = 0x00; //start application file flag ,determines whether the application file is activeted immediately after records are sent to receiver
    head[10] = 0x00; //factroy setting flag 
    
    //output message record
    head[11] = 0x07; //record type.output message record
    head[12] = 0x00; //record length.number of bytes in the record.excluding
    head[13] = 0x00; //output message type
    head[14] = 0x00; //port index;
    head[15] = 0x00; //frequency;
    head[16] = 0x00; //offset;
    head[17] = 0x00; //gosfsubmessage type;
    head[18] = 0x00; //cmr message type flags;
    head[19] = 0x00; //rtcm flags
    head[20] = 0x00; //real-time 17 message flags
    
    head[7] = (head[1] + head[2] + head[3] + head[4] +head[5] + head[6]);
    head[8] = 0x03;
    usart6_cmd_mode = 1;
    send_cmd(head,9);
    return 0;
}

int receive_data_LLA(void* pthis,char c)
{
    struct _cmd_rev *this; 
    this = cmd_buffer;
    int size = this->size;
    static char receive_state = 0;
    static unsigned char file_block_data_len = 0;
    static unsigned char data_block_data_len = 0;
    static char checksum = 0;
    static char received_data_index;
    char* pbuf;
    //static char error_step = FILE_CTRL_BLOCK_START;
     //error_step = receive_state;
    
    switch(receive_state)
    {
    case FILE_CTRL_BLOCK_START: 
      if(c == GPS_FRAME_START)
      {
          receive_state =  FILE_CTRL_BLOCK_GPS_STATUS;   
          checksum = 0;
      }
      else
      {
          pbuf = usart6_rx_db->buffer[usart6_rx_db->wt_id].head + usart6_rx_db->buffer[usart6_rx_db->wt_id].index;
          *pbuf = c; 
          usart6_rx_db->buffer[usart6_rx_db->wt_id].index++;
          if(usart6_rx_db->buffer[usart6_rx_db->wt_id].index == usart6_rx_db->size)
          {
              usart6_rx_db->buffer[usart6_rx_db->wt_id].index = 0;
          }           
      }
      break;
    case FILE_CTRL_BLOCK_GPS_STATUS: 
          checksum+=c;
          receive_state =  FILE_CTRL_BLOCK_TYPE;   
      break;
    case FILE_CTRL_BLOCK_TYPE: 
          if(c == 0x57)
          {
              receive_state =  FILE_CTRL_BLOCK_LEN;   
              checksum+=c;
          }
          else
          {
              receive_state =  FILE_CTRL_BLOCK_START;
              cmd_buffer->offset = 0;
          }
      break;
    case FILE_CTRL_BLOCK_LEN:
        file_block_data_len = c;
        if(c == 0x0A)
        {
            checksum+=c;
            receive_state =  FILE_CTRL_BLOCK_RECEIVE_DATA;            
        }
        else
        {
            receive_state =  FILE_CTRL_BLOCK_START;
            received_data_index = 0;
        }
      break;
    case FILE_CTRL_BLOCK_RECEIVE_DATA:
         received_data_index++;
         checksum +=c;
         if(received_data_index == file_block_data_len)
         {
             receive_state =  FILE_CTRL_BLOCK_CHECKSUM;
         }
      break;
    case FILE_CTRL_BLOCK_CHECKSUM:
          if(checksum == c)
          {
              receive_state =  FILE_CTRL_BLOCK_FRAME_END;
          }
          else
          {
              receive_state =  FILE_CTRL_BLOCK_START;
          }
      break;
    case FILE_CTRL_BLOCK_FRAME_END:
          if(0x03 == c)
          {
              receive_state =  DATA_BLOCK_START;
              checksum +=c;
          }
          else
          {
              receive_state =  FILE_CTRL_BLOCK_START;
          }      
      break;
    case DATA_BLOCK_START: 
          if(GPS_FRAME_START == c)
          {
              receive_state =  DATA_BLOCK_GPS_STATUS;
              checksum = 0;
          }
          else
          {
              receive_state =  FILE_CTRL_BLOCK_START;
          }   
      break;
    case DATA_BLOCK_GPS_STATUS:
          checksum+=c;
          receive_state =  DATA_BLOCK_TYPE;
      break;
    case DATA_BLOCK_TYPE:
          if(0x57 != c)
          {
              receive_state =  FILE_CTRL_BLOCK_START;
          }
          else
          {
              receive_state =  DATA_BLOCK_LEN;
              checksum += c;
          }
      break;
    case DATA_BLOCK_LEN:
        data_block_data_len = c;
        if(c > size - 22)  //data len max length
        {
            receive_state =  FILE_CTRL_BLOCK_START;
            checksum = 0;          
        }
        else
        {
            checksum +=c;
            receive_state =  DATA_BLOCK_RECEIVE_DATA;
            received_data_index = 0;
        }          
      break;
    case DATA_BLOCK_RECEIVE_DATA:
      *(cmd_buffer->buf + cmd_buffer->offset) = c;
      cmd_buffer->offset++;
      checksum+=c;
      if(data_block_data_len == cmd_buffer->offset)
      {
          receive_state =  DATA_BLOCK_CHECKSUM;
      }
      else if (cmd_buffer->offset > data_block_data_len)
      {
          cmd_buffer->offset = 0;
          receive_state =  FILE_CTRL_BLOCK_START;
      }
      break;
    case DATA_BLOCK_CHECKSUM:
      receive_state =  DATA_BLOCK_FRAME_END;
      /*if(c == checksum)
      {
         _FS_LOCK 
      }
      else
      {
          cmd_buffer->offset = 0;
          receive_state =  FILE_CTRL_BLOCK_START;
      }*/
      break;
    case DATA_BLOCK_FRAME_END:
      if(0x03 == c)
      {
          usart6_cmd_mode = 0;//turn off cmd receive mode
          gps_cmd_mgmt->cmd_link.next->gps_cmd.data_receive_done = 1;
      }
      receive_state =  FILE_CTRL_BLOCK_START;  
      break;
    default:
      receive_state =  FILE_CTRL_BLOCK_START;
      break;
    }   
    return 1;
}

int wait_read_LLA(void* pthis)
{    
    return (gps_cmd_mgmt->cmd_link.next->gps_cmd.data_receive_done == 1);
}

extern int get_position();

int release(void* pthis)
{
    struct _cmd_mgmt *this; 
    this = (struct _cmd_mgmt *)pthis;
    struct _cmd_link *link = this->cmd_link.next;
    
    free(link->gps_cmd.cmd_rev.buf);
    if(link->next)
    {
        this->cmd_link.next = link->next;
        link->next->head = &this->cmd_link;
    }
    else
    {
        this->cmd_link.next = 0;
    }
    free(link);
    return 0;
}

struct _main_commmand 
{
    char frame_head;
    char receiver_status;
    char pack_type;
    char length;
    char checksum;
    char frame_end;
};

int send_read_LLA_cmd(void* pthis)
{
    char head[9];
    head[0] = 0x02; //start transmission
    head[1] = 0x00; //receiver status code 
    head[2] = 0x56; //command packet type 
    head[3] = 0x03; //data  byte count 
    head[4] = 0x01; //sub type
    head[5] = 0x00; // svn number,if read UTC, this byte ignor
    head[6] = 0x00;  //flag
    head[7] = (head[1] + head[2] + head[3] + head[4] +head[5] + head[6]);
    head[8] = 0x03;
    gps_cmd_mgmt->cmd_link.next->gps_cmd.start_tick = HAL_GetTick();
    usart6_cmd_mode = 1;
    send_cmd(head,9);
    return 0;
}

void send_cmd(char* data,int size)
{
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart6,(uint8_t*)data,size,200);
}


int send_read_position_cmd()
{
    unsigned char head[9];
    /*head[0] = 0x02; //start transmission
    head[1] = 0x00; //receiver status code 
    head[2] = 0x54; //command packet type 
    head[3] = 0x03; //data  byte count 
    head[4] = 0x03; //sub type
    head[5] = 0x00; // svn number,if read UTC, this byte ignor
    head[6] = 0x00;  //flag
    head[7] = (head[1] + head[2] + head[3] + head[4] +head[5] + head[6]); //checksum  = 
    head[8] = 0x03;
    */
    
    
    head[0] = 0x02; //start transmission
    head[1] = 0x00; //receiver status code 
    head[2] = 0x56; //command packet type 
    head[3] = 0x03; //data  byte count 
    head[4] = 0x01; //sub type
    head[5] = 0x00; // svn number,if read UTC, this byte ignor
    head[6] = 0x00;  //flag
    head[7] = (head[1] + head[2] + head[3] + head[4] +head[5] + head[6]); //checksum  = 
    head[8] = 0x03;
    HAL_UART_Transmit(&huart6,head, 9, 200);
    return 0;
}


int send_usart6_output_disable()
{
    unsigned char head[9];    
    head[0] = 0x02; //start transmission
    head[1] = 0x00; //receiver status code 
    head[2] = 0x56; //command packet type 
    head[3] = 0x03; //data  byte count 
    head[4] = 0x01; //sub type
    head[5] = 0x00; // svn number,if read UTC, this byte ignor
    head[6] = 0x00;  //flag
    head[7] = (head[1] + head[2] + head[3] + head[4] +head[5] + head[6]); //checksum  = 
    head[8] = 0x03;
   
    HAL_UART_Transmit(&huart6,head, 9, 200);
    return 0;
}

double motor2inter_double(double m)
{
    char  tmp;
    char  *pm;
    pm = (char*)(&m);
    for(char i = 0 ;i < 4; i++)
    {
        tmp = *(pm+i);
        *(pm+i) = *(pm+7-i);
        *(pm+7-i) = tmp;
    }
    return m;
}

long motor2inter_long(unsigned long m)
{
    char  tmp;
    char  *pm;
    pm = (char*)(&m);
    for(char i = 0 ;i < 2; i++)
    {
        tmp = *(pm+i);
        *(pm+i) = *(pm+3-i);
        *(pm+3-i) = tmp;
    }
    return m;    
}
int gps_get_LLA(char *buff ,unsigned char size,struct _LLA* plla,unsigned char format)
{
    //memset(temp,0,200);
    //int num = double_buffer_buffer_rd_cnt(usart6_rx_db);  //after  double_buffer_exchange(usart6_rx_db) be called ,data is store in read buffer.   
    //int num = cmd_buffer->offset;
    //memcpy(temp,cmd_buffer->buf,num);

    double  latitude;
    double  longitude;
    double  altitude;
    double  *platitude;
    double  *plongitude;
    double  *paltitude;
    platitude = (double*)(buff+4);
    plongitude = (double*)(buff+12);
    paltitude = (double*)(buff+20);
    
    if((format &0x01) == 1)  // 1--inter,else MOTOROLAR
    {
        latitude = motor2inter_double(*platitude);
        longitude = motor2inter_double(*plongitude);
        altitude = motor2inter_double(*paltitude);
    }
    else if((format &0x01) == 0) //MOTOROLAR format
    {
        latitude = *platitude;
        longitude = *plongitude;
        altitude = *paltitude;        
    }
    
    plla->latitude = latitude;
    plla->longitude = longitude;
    if((format & 0x02))  //
    {
         plla->latitude *= 180;
         plla->longitude *= 180;
    }
    plla->altitude = altitude;
    return 0;
}
char temp[200] = {0};
int gps_get_position_info(char *buff ,unsigned char size,struct _postion_info* pp,unsigned char format)
{
    memset(temp,0,200);
    //int num = double_buffer_buffer_rd_cnt(usart6_rx_db);  //after  double_buffer_exchange(usart6_rx_db) be called ,data is store in read buffer.   
    int num = cmd_buffer->offset;
    memcpy(temp,cmd_buffer->buf,num);

    double  latitude;
    double  longitude;
    double  altitude;
    double  pdop;
    char    postion_flag;
    double  *platitude;
    double  *plongitude;
    double  *paltitude;
    double  *ppdop;
    platitude = (double*)(buff+4);
    plongitude = (double*)(buff+12);
    paltitude = (double*)(buff+20);
    ppdop = (double*)(buff+44);
    postion_flag = *(buff+80);
    if(format &0x01)  // 1--inter,else MOTOROLAR
    {
        latitude = motor2inter_double(*platitude);
        longitude = motor2inter_double(*plongitude);
        altitude = motor2inter_double(*paltitude);
        pdop = motor2inter_double(*ppdop);
    }
    else if((format &0x01) == 0) //MOTOROLAR format
    {
        latitude = *platitude;
        longitude = *plongitude;
        altitude = *paltitude;        
    }
    
    pp->latitude = latitude;
    pp->longitude = longitude;
    if((format & 0x02))  //
    {
         pp->latitude *= 180;
         pp->longitude *= 180;
    }
    pp->altitude = altitude;
    pp->position_flag = gps_get_postion_flag_code(postion_flag);
    pp->pdop = pdop;
    return 0;
}

char gps_get_postion_flag_code(char flag)
{
    char code = 6;  //other state return 6 as postion flag
    struct _flag
    {

      union
      {
        char bit0_2:3;
        char bit3:1;
        char bit4:1;
        char bit5:1;
        char bit6:1;
        char bit7:1;
        unsigned char bt;
      };
    }pos_flag;
    pos_flag.bt = flag;
    if(pos_flag.bit0_2 <= 3) //bit0~2  == 0~3 ,position type = 0
    {
        code = 0;
    }
    else if (4 == pos_flag.bit0_2 || 5 == pos_flag.bit0_2)  
    {
        if(pos_flag.bit3)
        {
            code = 4;   
        }
        else if(pos_flag.bit3 || 0 == pos_flag.bit4 ) 
        {
            code = 1;
        }
        else if (0 == pos_flag.bit3 && 1 == pos_flag.bit4 && 0 == pos_flag.bit6)
        {
            code = 2;
        }
        else if (0 == pos_flag.bit3 && 1 == pos_flag.bit4 && 1 == pos_flag.bit6)
        {
            code = 5;
        }
    }

    return code;
}

void send_cmd_to_gps(struct _gps_cmd* pcmd)
{
    ((FUNCTION)(gps_cmd_mgmt->add_cmd))((void*)gps_cmd_mgmt,(void*)pcmd);
}

void gps_cmd_receive(void)
{
    if(gps_cmd_mgmt->cmd_link.next)
    {
        g_gps_index = GPS_PREPARE_SEND;
    }
}

void gps_prepare_send_cmd(void)
{
    (*gps_cmd_mgmt->cmd_link.next->gps_cmd.prepare_send_cmd)((void*) gps_cmd_mgmt); 
    g_gps_index = GPS_CMD_SEND_CMD;
}

void gps_send_cmd(void)
{
    (*gps_cmd_mgmt->cmd_link.next->gps_cmd.send_cmd)((void*) gps_cmd_mgmt); 
    g_gps_index = GPS_WAIT_DATA;
}

void gps_wait_reply(void)
{
    unsigned int eclipse = 0;
    eclipse = HAL_GetTick();
    eclipse -= gps_cmd_mgmt->cmd_link.next->gps_cmd.start_tick;
    if(eclipse > gps_cmd_mgmt->cmd_link.next->gps_cmd.timeout)
    {
        g_gps_index = GPS_RELEASE;
        usart6_cmd_mode = 0;
    }
    else
    {
        if(gps_cmd_mgmt->cmd_link.next->gps_cmd.wait_cmd((void*) gps_cmd_mgmt))
        {
            g_gps_index = GPS_DISPOSE_DATA;
        }
    }   
}

void gps_dispose_data(void)
{
    gps_cmd_mgmt->cmd_link.next->gps_cmd.dispose_cmd((void*) gps_cmd_mgmt);
    g_gps_index = GPS_RELEASE;   
}

void gps_release(void)
{
    release((void*) gps_cmd_mgmt);  
    g_gps_index = GPS_CMD_RECEIVE;
}
