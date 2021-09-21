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
        print_er("Wrong msg type\r\n"); // FIXME:
        status |= CRG20_MSGTYPE_ERROR;
    }
    if (CRG20_CheckSum(rx) != (rx.c_sum)){
        status |= CRG20_CHECKSUM_ERROR;
    }
    return status;
}

static inline crg20_msg_status_t CRG20_SendReceivePacket(uint8_t msg_type, uint8_t self_test, crg20_pack_t * frame){
    crg20_pack_t tx_pack;
    tx_pack = CRG20_TxPackPrepare(msg_type, self_test);
    CS_SEL;
    CRG20_SPI_TRANSMIT(&tx_pack);
    CS_DESEL;
    DELAY(1);
    CS_SEL;
    CRG20_SPI_RECEIVE(frame);
    CS_DESEL;
    return CRG20_CheckPacket(tx_pack, *frame);
}

crg20_msg_status_t CRG20_GetDefault(crg20_default_t * frame){
    crg20_pack_t rx_pack; 
    crg20_msg_status_t status = CRG20_SendReceivePacket(CRG20_DEFAULT, CRG20_SELFTEST_OFF, &rx_pack);
    if (status == CRG20_MSG_OK){
        frame->rate = (int16_t)(rx_pack.data[1] | ((uint16_t)(rx_pack.data[0]) << 8));
        frame->rate_check_byte = rx_pack.data[2];
    }
    return status;
}

crg20_msg_status_t CRG20_GetAUX(uint8_t aux_num, crg20_aux_t * frame){
    crg20_pack_t rx_pack;
    crg20_msg_status_t status;
    if (aux_num == CRG20_AUX_1) status = CRG20_SendReceivePacket(CRG20_AUX1, CRG20_SELFTEST_OFF, &rx_pack);
    else status = CRG20_SendReceivePacket(CRG20_AUX2, CRG20_SELFTEST_OFF, &rx_pack);
    if (status == CRG20_MSG_OK){
        frame->rate = (int16_t)(rx_pack.data[1] | ((uint16_t)(rx_pack.data[0]) << 8));
        frame->data = (int16_t)(rx_pack.data[3] | ((uint16_t)(rx_pack.data[2]) << 8));
    }
    return status;
}

crg20_msg_status_t CRG20_GetTemperature(crg20_temp_t * frame){
    crg20_pack_t rx_pack;
    crg20_msg_status_t status = CRG20_SendReceivePacket(CRG20_TEMPERATURE, CRG20_SELFTEST_OFF, &rx_pack);
    if (status == CRG20_MSG_OK){
        frame->rate = (int16_t)(rx_pack.data[1] | ((uint16_t)(rx_pack.data[0]) << 8));
        frame->data = (int16_t)(rx_pack.data[3] | ((uint16_t)(rx_pack.data[2]) << 8));
    }
    return status;
}

crg20_msg_status_t CRG20_GetDevConfig(crg20_dev_configs_t * frame){
    crg20_pack_t rx_pack_1, rx_pack_2;
    crg20_msg_status_t status_1 = CRG20_SendReceivePacket(CRG20_DEV_CONF_1, CRG20_SELFTEST_OFF, &rx_pack_1);
    crg20_msg_status_t status_2 = CRG20_SendReceivePacket(CRG20_DEV_CONF_2, CRG20_SELFTEST_OFF, &rx_pack_2);
    if ((status_1 | status_2) == CRG20_MSG_OK){
        frame->model_bandwidth =    (rx_pack_1.data[0] & 0x0F);
        frame->model_rate_range =   ((rx_pack_1.data[0] >> 4) & 0x0F);
        frame->model_variant =      (rx_pack_1.data[1] & 0x0F);
        frame->software_version =   (rx_pack_1.data[2]);
        frame->manufacture_year =   (rx_pack_1.data[3] & 0b00011111);

        frame->manufacture_month =  ((((uint16_t)(rx_pack_2.data[0])) >> 4) & 0x0F);
        frame->manufacture_lot =    (((((uint16_t)(rx_pack_2.data[0])) & 0x0F) << 8) | ((uint16_t)(rx_pack_2.data[1])));
        frame->assembly_plant =     ((((uint16_t)(rx_pack_2.data[2])) >> 6) & 0b00000011);
        frame->serial_number =      (((((uint16_t)(rx_pack_2.data[2])) & 0b00111111) << 8) | ((uint16_t)(rx_pack_2.data[3])));
    }
    return (status_1 | status_2);
}
