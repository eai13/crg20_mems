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

static inline crg20_msg_status_t CRG20_CheckPacket(crg20_pack_t tx, crg20_pack_t rx){
    crg20_msg_status_t status = CRG20_MSG_OK;
    if ((tx.status_byte & (CRG20_TO_STATUS_NMT0 | CRG20_TO_STATUS_NMT1 | CRG20_TO_STATUS_NMT2)) !=
        (rx.status_byte & (CRG20_FROM_STATUS_MT0 | CRG20_FROM_STATUS_MT1 | CRG20_FROM_STATUS_MT2))){
        status |= CRG20_MSGTYPE_ERROR;
    }
    if (CRG20_CheckSum(rx) != (rx.c_sum)){
        status |= CRG20_CHECKSUM_ERROR;
    }
    return status;
}

static inline crg20_pack_t CRG20_SendReceivePacket(uint8_t msg_type, uint8_t self_test){
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

crg20_msg_status_t