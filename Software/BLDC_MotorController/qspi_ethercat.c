/*
 * qspi_ethercat.c
 *
 * Created: 11/11/2022 11:49:42
 *  
 */ 

#include "qspi_ethercat.h"

/************************************************************************/
/* Global Variables                                                     */
/************************************************************************/
uint8_t transmit_msg_abort_fifo[12] = {0};
uint8_t transmit_msg_config_fifo[16] = {0};
uint8_t transmit_msg_clear_rdram[64] = {0};
uint8_t transmit_msg_clear_rdram_config[8] = {0};

/* read and write buffers for EtherCAT*/
volatile uint32_t send_data[16] = {0};
volatile uint32_t read_data[16] = {0};
	
struct _qspi_command cmd_config_fifo    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.tfr_type = QSPI_WRITE_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x02,
	.address				  = 0x024308,
	.tx_buf					  = transmit_msg_config_fifo,
	.buf_len				  = 16,
};

struct _qspi_command cmd_abort_fifo    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.tfr_type = QSPI_WRITE_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x02,
	.address				  = 0x02430C,
	.tx_buf					  = transmit_msg_abort_fifo,
	.buf_len				  = 12,
};

struct _qspi_command cmd_clear_rdram_data    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.tfr_type = QSPI_WRITE_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x02,
	.address				  = 0x020020,
	.tx_buf					  = transmit_msg_clear_rdram,
	.buf_len				  = 64,
};

struct _qspi_command cmd_clear_rdram_config    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.tfr_type = QSPI_WRITE_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x02,
	.address				  = 0x024310,
	.tx_buf					  = transmit_msg_clear_rdram_config,
	.buf_len				  = 8,
};

struct _qspi_command cmd_send_data    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.tfr_type = QSPI_WRITE_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x02,
	.address					= 0x020020,
	.tx_buf					  = send_data,
	.buf_len				  = 16*4,
};

struct _qspi_command cmd_read_data    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.continues_read = 0,
	.inst_frame.bits.tfr_type = QSPI_READ_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x03,
	.address					= 0x030000,
	.tx_buf					  = send_data,
	.rx_buf						= read_data,
	.buf_len				  = 16*4,
};
struct _qspi_command cmd_write_to_read_data    = {
	.inst_frame.bits.width	  = 0,
	.inst_frame.bits.inst_en  = 0,
	.inst_frame.bits.addr_en  = 1,
	.inst_frame.bits.addr_len = 0,	//0 - 24bit address; 1- 32bit address
	.inst_frame.bits.opt_en	  = 0,
	.inst_frame.bits.data_en  = 1,
	.inst_frame.bits.opt_len  = 0,
	.inst_frame.bits.tfr_type = QSPI_READ_ACCESS,
	.inst_frame.bits.ddr_enable = 0,
	.instruction              = 0x03,
	.address					= 0x030000,
	.tx_buf					  = NULL,
	.rx_buf					  = read_data,
	.buf_len				  = 16*4,
};


/************************************************************************/
/* Functions                                                            */
/************************************************************************/
void init_standard_messages()
{
	// Abort FIFO message	
	transmit_msg_abort_fifo[0] = 0x00;
	transmit_msg_abort_fifo[1] = 0x00;
	transmit_msg_abort_fifo[2] = 0x00;
	transmit_msg_abort_fifo[3] = 0x40;
	
	transmit_msg_abort_fifo[4] = 0x00;
	transmit_msg_abort_fifo[5] = 0x00;
	transmit_msg_abort_fifo[6] = 0x00;
	transmit_msg_abort_fifo[7] = 0x00;
	
	transmit_msg_abort_fifo[8] = 0x00;
	transmit_msg_abort_fifo[9] = 0x00;
	transmit_msg_abort_fifo[10] = 0x00;
	transmit_msg_abort_fifo[11] = 0x40;
	
	
	// Config FIFO message	
	transmit_msg_config_fifo[0] = 0x00;
	transmit_msg_config_fifo[1] = 0x10;
	transmit_msg_config_fifo[2] = 0x40;
	transmit_msg_config_fifo[3] = 0x00;
	
	transmit_msg_config_fifo[4] = 0x00;
	transmit_msg_config_fifo[5] = 0x00;
	transmit_msg_config_fifo[6] = 0x00;
	transmit_msg_config_fifo[7] = 0x80;
	
	transmit_msg_config_fifo[8] = 0x00;
	transmit_msg_config_fifo[9] = 0x18;
	transmit_msg_config_fifo[10] = 0x40;
	transmit_msg_config_fifo[11] = 0x00;
	
	transmit_msg_config_fifo[12] = 0x00;
	transmit_msg_config_fifo[13] = 0x00;
	transmit_msg_config_fifo[14] = 0x00;
	transmit_msg_config_fifo[15] = 0x80;

	//clear rdram config
	
	transmit_msg_clear_rdram_config[0] = 0x00;
	transmit_msg_clear_rdram_config[1] = 0x10;
	transmit_msg_clear_rdram_config[2] = 0x40;
	transmit_msg_clear_rdram_config[3] = 0x00;
	
	transmit_msg_clear_rdram_config[4] = 0x00;
	transmit_msg_clear_rdram_config[5] = 0x00;
	transmit_msg_clear_rdram_config[6] = 0x00;
	transmit_msg_clear_rdram_config[7] = 0x80;
	
	// task data
	send_data[0] = 0x00000000;
	send_data[1] = 0x00000000;
	send_data[2] = 0x00000000;
	send_data[3] = 0x00000000;
	send_data[4] = 0x00000000;
	send_data[5] = 0x00000000;
	send_data[6] = 0x00000000;
	send_data[7] = 0x00000000;
	send_data[8] = 0x00000000;
	send_data[9] = 0x00000000;
	send_data[10] = 0x00000000;
	send_data[11] = 0x00000000;
	send_data[12] = 0x00000000;
	send_data[13] = 0x00000000;
	send_data[14] = 0x00000000;
	send_data[15] = 0x00000000;
	
	
}


void send_to_ethercat(struct _qspi_command* cmd)
{
	qspi_sync_serial_run_command(&QUAD_SPI_0, cmd);
	//qspi_sync_deinit(&QUAD_SPI_0);
}

char* read_from_ethercat(struct _qspi_command* cmd)
{

	//qspi_sync_enable(&QUAD_SPI_0);
	qspi_sync_serial_run_command(&QUAD_SPI_0, cmd);
	//qspi_sync_deinit(&QUAD_SPI_0);
}


void config_fifo()
{	
	send_to_ethercat(&cmd_config_fifo);
}


void abort_fifo()
{
	send_to_ethercat(&cmd_abort_fifo);
}


void clear_rdram()
{
	send_to_ethercat(&cmd_clear_rdram_data);
	send_to_ethercat(&cmd_clear_rdram_config);
}

void write_16_DWORDS_data()
{
	send_to_ethercat(&cmd_send_data);
	//send_data[0]++;
}

void read_16_DWORDS_data()
{	
	send_to_ethercat(&cmd_write_to_read_data);
	//send_to_ethercat(&cmd_read_data);
	
}
