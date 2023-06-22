/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void convert_cb_ADC_IB(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

/**
 * Example of using ADC_IB to generate waveform.
 */
void ADC_IB_example(void)
{
	adc_async_enable_channel(&ADC_IB, 0);
	adc_async_register_callback(&ADC_IB, 0, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_IB);
	adc_async_start_conversion(&ADC_IB);
}

static void convert_cb_ADC_IA(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

/**
 * Example of using ADC_IA to generate waveform.
 */
void ADC_IA_example(void)
{
	adc_async_enable_channel(&ADC_IA, 0);
	adc_async_register_callback(&ADC_IA, 0, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_IA);
	adc_async_start_conversion(&ADC_IA);
}

/**
 * Example of using DIGITAL_GLUE_LOGIC_0.
 */
void DIGITAL_GLUE_LOGIC_0_example(void)
{
	custom_logic_enable();
	/* Customer logic now works. */
}

/**
 * Example of using QUAD_SPI_0 to get N25Q256A status value,
 * and check bit 0 which indicate embedded operation is busy or not.
 */
void QUAD_SPI_0_example(void)
{
	uint8_t              status = 0xFF;
	struct _qspi_command cmd    = {
        .inst_frame.bits.inst_en  = 1,
        .inst_frame.bits.data_en  = 1,
        .inst_frame.bits.tfr_type = QSPI_READ_ACCESS,
        .instruction              = 0x05,
        .buf_len                  = 1,
        .rx_buf                   = &status,
    };

	qspi_sync_enable(&QUAD_SPI_0);
	while (status & (1 << 0)) {
		qspi_sync_serial_run_command(&QUAD_SPI_0, &cmd);
	}
	qspi_sync_deinit(&QUAD_SPI_0);
}

/**
 * Example of using SPI_ENC to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_SPI_ENC[12] = "Hello World!";

static void complete_cb_SPI_ENC(const struct spi_m_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void SPI_ENC_example(void)
{
	struct io_descriptor *io;
	spi_m_async_get_io_descriptor(&SPI_ENC, &io);

	spi_m_async_register_callback(&SPI_ENC, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_ENC);
	spi_m_async_enable(&SPI_ENC);
	io_write(io, example_SPI_ENC, 12);
}

static struct timer_task TIMER_2_task1, TIMER_2_task2;

/**
 * Example of using TIMER_2.
 */
static void TIMER_2_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_2_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_2_example(void)
{
	TIMER_2_task1.interval = 100;
	TIMER_2_task1.cb       = TIMER_2_task1_cb;
	TIMER_2_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_2_task2.interval = 200;
	TIMER_2_task2.cb       = TIMER_2_task2_cb;
	TIMER_2_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_2, &TIMER_2_task1);
	timer_add_task(&TIMER_2, &TIMER_2_task2);
	timer_start(&TIMER_2);
}
