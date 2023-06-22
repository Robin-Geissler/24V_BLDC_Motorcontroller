/*
 * qspi_ethercat.h
 *
 * Created: 11/11/2022 11:48:10
 * 
 */ 

#include "atmel_start.h"

#ifndef QSPI_ETHERCAT_H_
#define QSPI_ETHERCAT_H_

/************************************************************************/
/* Global Variables                                                     */
/************************************************************************/
extern uint8_t transmit_msg_abort_fifo[12];
extern uint8_t transmit_msg_config_fifo[16];
extern uint8_t transmit_msg_clear_rdram[64];

/* read and write buffers for EtherCAT*/
extern volatile uint32_t send_data[16];
extern volatile uint32_t read_data[16];

void init_standard_messages();
void send_to_ethercat(struct _qspi_command* cmd);
char* read_from_ethercat(struct _qspi_command* cmd);
void config_fifo();
void abort_fifo();
void clear_rdram();
void write_16_DWORDS_data();
void read_16_DWORDS_data();
void convert_byteorder(uint32_t* val);

#endif /* QSPI_ETHERCAT_H_ */