/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_async.h>
#include <hal_adc_async.h>

#include <hal_custom_logic.h>

#include <hal_evsys.h>

#include <hal_qspi_sync.h>

#include <hal_spi_m_async.h>
#include <tc_lite.h>
#include <tc_lite.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>
#include <tcc_lite.h>

extern struct adc_async_descriptor ADC_IB;
extern struct adc_async_descriptor ADC_IA;

extern struct qspi_sync_descriptor QUAD_SPI_0;

extern struct spi_m_async_descriptor SPI_ENC;
extern struct timer_descriptor       TIMER_2;

void ADC_IB_init(void);

void ADC_IA_init(void);

void DIGITAL_GLUE_LOGIC_0_PORT_init(void);
void DIGITAL_GLUE_LOGIC_0_CLOCK_init(void);
void DIGITAL_GLUE_LOGIC_0_init(void);

void QUAD_SPI_0_PORT_init(void);
void QUAD_SPI_0_CLOCK_init(void);
void QUAD_SPI_0_init(void);

void SPI_ENC_PORT_init(void);
void SPI_ENC_CLOCK_init(void);
void SPI_ENC_init(void);

void TIMER_1_CLOCK_init(void);

int8_t TIMER_1_init(void);

void TIMER_0_CLOCK_init(void);

int8_t TIMER_0_init(void);

void PWM_0_CLOCK_init(void);

void PWM_0_PORT_init(void);

int8_t PWM_0_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
