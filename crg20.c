#include "crg20.h"

static inline uint8_t CRG20_CheckSum(crg20_pack_t pack){
    uint8_t * pntr = (uint8_t *)(&pack);
    uint16_t sum = 0x0000;
    for (uint8_t iter = 0; iter < (sizeof(crg20_pack_t) - 1); iter++) sum += pntr[iter];
    return ((uint8_t)((~sum) & 0xFF));
}

static inline crg20_pack_t CRG20_TxPackPrepare(uint8_t msg_type, uint8_t self_test){
    crg20_pack_t pack;
    for (uint8_t iter = 0; iter < 4; iter++) pack.data[iter] = RESERVED_BYTE;
    pack.status_byte = msg_type | ((self_test) ? (CRG20_TO_STATUS_CBITA) : (0x00));
    pack.c_sum = CRG20_CheckSum(pack);
    return pack;
}

crg20_pack_t CRG20_SendReceivePacket(uint8_t msg_type, uint8_t self_test){
    crg20_pack_t tx_pack, rx_pack;
    tx_pack = CRG20_TxPackPrepare(msg_type, self_test);
    CS_SEL;
    DELAY(1);
    CRG20_SPI_TRANSMIT(&tx_pack);
    CS_DESEL;
    DELAY(1);
    CS_SEL;
    CRG20_SPI_RECEIVE(&rx_pack);
    CS_DESEL;
    return rx_pack;
}