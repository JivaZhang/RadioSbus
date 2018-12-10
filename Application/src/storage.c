//#include "storage.h"
//#include "stdint.h"
//#include "usart.h"
//#define STORAGE_AREA_SIZE 4
//#define STORAGE_START_ADDRESS 0x80D0000U
//
//static const uint8_t        k_EEPROM_magic0      = 0x4E;
//static const uint8_t        k_EEPROM_magic1      = 0x49; ///< "NI"
//static const uint8_t        k_EEPROM_revision    = 1; ///< current format revision
//
//
//struct GroupInfo {
//    uint8_t type; // AP_PARAM_*
//    uint8_t idx;  // identifier within the group
//    
//    ptrdiff_t offset; // offset within the object
//    union {
//        const struct GroupInfo *group_info;
//        const struct GroupInfo **group_info_ptr; // when AP_PARAM_FLAG_INFO_POINTER is set in flags
//        const float def_value;
//    };
//    uint16_t flags;
//};
//
//struct StorageArea {
//    eStorageType type;
//    
//    uint16_t    offset;
//    uint16_t    length;
//};
//
//typedef struct eepromMagic
//{
//    
//};
//
//struct StorageArea systemParam[STORAGE_AREA_SIZE] = 
//{
//  {StorageUsartConfig,0,20};
//  {StorageUsartConfig,20,20};
//  {StorageUsartConfig,40,20};
//  {StorageUsartConfig,60,20};
//};
//extern 
//bool LoadSystemParam()
//{
//    stUsartDevCfg* usart1
//    return true;
//}
//AP_GROUPINFO("MIN",  1, SRV_Channel, servo_min, 1100),
//
//
//#define AP_GROUPINFO(name, idx, clazz, element, def) AP_GROUPINFO_FLAGS(name, idx, clazz, element, def, 0)
//
//#define AP_GROUPINFO_FLAGS(name, idx, clazz, element, def, flags) { AP_CLASSTYPE(clazz, element), idx, name, AP_VAROFFSET(clazz, element), {def_value : def}, flags }
//#define AP_CLASSTYPE(clazz, element) ((uint8_t)(((const clazz *) 1)->element.vtype))
//
//{type,idx,"min",name,offset,def,flags}
