#ifndef __STORAGE_H__
#define __STORAGE_H__
#include "stdint.h"
#define prtdiff_t int32_t
typedef enum StorageType {
    STORAGE_NONE = 0,
    STORAGE_USART_CONFIG = 1,
    StorageTypeMax
}eStorageType;

struct GroupInfo {
    uint8_t type; // AP_PARAM_*
    uint8_t idx;  // identifier within the group
    const char *name;
    int32_t  offset; // offset within the object
    union {
        const struct GroupInfo *group_info;
        const struct GroupInfo **group_info_ptr; // when AP_PARAM_FLAG_INFO_POINTER is set in flags
        const float def_value;
    };
    uint16_t flags;
};
#endif  //__STORAGE_H__