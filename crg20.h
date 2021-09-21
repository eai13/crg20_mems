#ifndef CRG20_MEMS_H
#define CRG20_MEMS_H

#include "crg20_config.h"
#include <stdint.h>
#include "debug.h"

/**
 * @brief error types for received messages
 */
typedef enum crg20_msg_status{
    CRG20_MSG_OK =          0x00,
    CRG20_CHECKSUM_ERROR =  0x01,
    CRG20_MSGTYPE_ERROR =   0x02
}crg20_msg_status_t;

/**
 * @brief aux number definitions
 */
#define CRG20_AUX_1         (uint8_t)(0x01)
#define CRG20_AUX_2         (uint8_t)(0x02)

/**
 * @brief self-test macros
 */
#define CRG20_SELFTEST_ON   (0x01)
#define CRG20_SELFTEST_OFF  (0x00)

/**
 * @brief to CRG20 message types
 */
#define CRG20_DEFAULT       (uint8_t)(0x00)
#define CRG20_AUX1          (CRG20_TO_STATUS_NMT0)
#define CRG20_AUX2          (CRG20_TO_STATUS_NMT1)
#define CRG20_TEMPERATURE   (CRG20_TO_STATUS_NMT1 | CRG20_TO_STATUS_NMT0)
#define CRG20_DEV_CONF_1    (CRG20_TO_STATUS_NMT2)
#define CRG20_DEV_CONF_2    (CRG20_TO_STATUS_NMT2 | CRG20_TO_STATUS_NMT0)

/**
 * @brief tx/rx packet structure
 */
#pragma pack(push, 1)
typedef struct crg20_pack{
    uint8_t status_byte;
    uint8_t data[4];
    uint8_t c_sum;
}crg20_pack_t;
#pragma pack(pop)

/**
 * @brief data structure for aux messages
 */
#pragma pack(push, 1)
typedef struct crg20_aux{
    uint16_t rate;
    uint16_t data;
}crg20_aux_t;
#pragma pack(pop)

/**
 * @brief data structure for default messages
 */
#pragma pack(push, 1)
typedef struct crg20_default{
    uint16_t rate;
    uint8_t rate_check_byte;
}crg20_default_t;
#pragma pack(pop)

/**
 * @brief data structure for temperature messages
 */
#pragma pack(push, 1)
typedef struct crg20_temp{
    uint16_t rate;
    uint16_t data;
}crg20_temp_t;
#pragma pack(pop)

/**
 * @brief data structure for device configuration messages
 */
#pragma pack(push, 1)
typedef struct crg20_dev_configs{
    uint8_t model_bandwidth     : 4;
    uint8_t model_rate_range    : 4;
    uint8_t model_variant       : 8;
    uint8_t software_version    : 8;
    uint8_t manufacture_year    : 8;
    uint16_t manufacture_month  : 4;
    uint16_t manufacture_lot    : 12;
    uint16_t assembly_plant     : 2;
    uint16_t serial_number      : 14;
}crg20_dev_configs_t;
#pragma pack(pop)

/**
 * @brief checksum calculation
 * sums all bytes of the package and returns the inverted LSB
 */
static inline uint8_t CRG20_CheckSum(crg20_pack_t pack);

/**
 * @brief transmit packet preparation
 * calcs the checksum and generates the status byte
 * @param msg_type status byte (NMT2:0)
 * @param self_test inbuilt self-test, use macros CRG20_SELFTEST
 */
static inline crg20_pack_t CRG20_TxPackPrepare(uint8_t msg_type, uint8_t self_test);

/**
 * @brief check received packet
 * @param tx transmitted packet
 * @param rx received packet
 * @return status
 */
static inline crg20_msg_status_t CRG20_CheckPacket(crg20_pack_t tx, crg20_pack_t rx);

/**
 * @brief transmits and receives the command to the CRG20
 * @param msg_type status byte (NMT2:0)
 * @param self_test inbuilt self-test, use macros CRG20_SELFTEST
 * @param frame address to write the obtained frame to
 * @return status
 */
static inline crg20_msg_status_t CRG20_SendReceivePacket(uint8_t msg_type, uint8_t self_test, crg20_pack_t * frame);

/**
 * @brief get aux data
 * @param aux_num number of aux, use macros AUX_1 or AUX_2
 * @param frame address to write data to
 * @return crg20_msg_status_t status is returned, for messages check
 */
crg20_msg_status_t CRG20_GetAUX(uint8_t aux_num, crg20_aux_t * frame);

/**
 * @brief default crg20 message
 * @param frame address to write data to
 * @return crg20_msg_status_t status is returned, for messages check
 */
crg20_msg_status_t CRG20_GetDefault(crg20_default_t * frame);

/**
 * @brief get temperature data
 * @param frame address to write data to
 * @return crg20_msg_status_t status is returned, for messages check
 */
crg20_msg_status_t CRG20_GetTemperature(crg20_temp_t * frame);

/**
 * @brief get device configuration data
 * @param frame address to write data to
 * @return crg20_msg_status_t status is returned, for messages check
 */
crg20_msg_status_t CRG20_GetDevConfig(crg20_dev_configs_t * frame);

#endif