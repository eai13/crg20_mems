#ifndef CRG20_CONFIG_H
#define CRG20_CONFIG_H

#include "spi.h"
#include "gpio.h"

/**
 * @brief basic CRG20 SPI configurations
 */
#define CRG20_SPI_TIMEOUT           100
#define CRG20_SPI                   hspi1
#define CRG20_PACK_SIZE             6
#define CRG20_SPI_TRANSMIT(data)    for (uint8_t iter = 0; iter < CRG20_PACK_SIZE; iter++) HAL_SPI_Transmit(&CRG20_SPI, (uint8_t *)data + iter, 1, CRG20_SPI_TIMEOUT)
#define CRG20_SPI_RECEIVE(data)     for (uint8_t iter = 0; iter < CRG20_PACK_SIZE; iter++) HAL_SPI_Receive(&CRG20_SPI, (uint8_t *)data + iter, 1, CRG20_SPI_TIMEOUT)
#define CS_SEL                      HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, 0)
#define CS_DESEL                    HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, 1)
#define DELAY(ms)                   HAL_Delay(ms)

/**
 * @brief in case of reserved byte use this
 */
#define RESERVED_BYTE               (uint8_t)(0x00)

/**
 * @brief status byte to CRG20
 */
#define CRG20_TO_STATUS_NMT0             (uint8_t)(1 << 0)
#define CRG20_TO_STATUS_NMT1             (uint8_t)(1 << 1)
#define CRG20_TO_STATUS_NMT2             (uint8_t)(1 << 2)
#define CRG20_TO_STATUS_CBITA            (uint8_t)(1 << 3)

/**
 * @brief status byte from CRG20
 */
#define CRG20_FROM_STATUS_MT0            (uint8_t)(1 << 0)
#define CRG20_FROM_STATUS_MT1            (uint8_t)(1 << 1)
#define CRG20_FROM_STATUS_MT2            (uint8_t)(1 << 2)
#define CRG20_FROM_STATUS_CBITA          (uint8_t)(1 << 3)
#define CRG20_FROM_STATUS_BF             (uint8_t)(1 << 4)
#define CRG20_FROM_STATUS_HCF            (uint8_t)(1 << 5)
#define CRG20_FROM_STATUS_MC0            (uint8_t)(1 << 6)
#define CRG20_FROM_STATUS_MC1            (uint8_t)(1 << 7)

#endif