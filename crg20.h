#ifndef CRG20_MEMS_H
#define CRG20_MEMS_H

#include "crg20_config.h"
#include <stdint.h>

/**
 * @brief to CRG20 message types
 */
#define CRG20_DEFAULT       (uint8_t)(0x00)
#define CRG20_AUX1          (NMT0)
#define CRG20_AUX2          (NMT1)
#define CRG20_TEMPERATURE   (NMT1 | NMT0)
#define CRG20_DEV_CONF_1    (NMT2)
#define CRG20_DEV_CONF_2    (NMT2 | NMT0)

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
 * @brief checksum calculation
 * sums all bytes of the package and returns the inverted LSB
 */
static inline uint8_t CRG20_CheckSum(crg20_pack_t pack);

/**
 * @brief transmit packet preparation
 * calcs the checksum and generates the status byte
 * @param msg_type status byte (NMT2:0)
 * @param self_test CBITA
 */
static inline crg20_pack_t CRG20_TxPackPrepare(uint8_t msg_type, uint8_t self_test);

/**
 * @brief transmits and receives the command to the CRG20
 * @param msg_type status byte (NMT2:0)
 * @param self_test CBITA
 */
crg20_pack_t CRG20_SendReceivePacket(uint8_t msg_type, uint8_t self_test);

#endif