/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

#include <hpl_adc_base.h>
#include <hpl_adc_base.h>

/* The channel amount for ADC */
#define ADC_IB_CH_AMOUNT 1

/* The buffer size for ADC */
#define ADC_IB_BUFFER_SIZE 16

/* The maximal channel number of enabled channels */
#define ADC_IB_CH_MAX 0

/* The channel amount for ADC */
#define ADC_IA_CH_AMOUNT 1

/* The buffer size for ADC */
#define ADC_IA_BUFFER_SIZE 16

/* The maximal channel number of enabled channels */
#define ADC_IA_CH_MAX 0

struct adc_async_descriptor         ADC_IB;
struct adc_async_channel_descriptor ADC_IB_ch[ADC_IB_CH_AMOUNT];
struct adc_async_descriptor         ADC_IA;
struct adc_async_channel_descriptor ADC_IA_ch[ADC_IA_CH_AMOUNT];
struct timer_descriptor             TIMER_2;

static uint8_t ADC_IB_buffer[ADC_IB_BUFFER_SIZE];
static uint8_t ADC_IB_map[ADC_IB_CH_MAX + 1];
static uint8_t ADC_IA_buffer[ADC_IA_BUFFER_SIZE];
static uint8_t ADC_IA_map[ADC_IA_CH_MAX + 1];

struct qspi_sync_descriptor QUAD_SPI_0;

struct spi_m_async_descriptor SPI_ENC;

/**
 * \brief ADC initialization function
 *
 * Enables ADC peripheral, clocks and initializes ADC driver
 */
void ADC_IB_init(void)
{
	hri_mclk_set_APBDMASK_ADC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC0_GCLK_ID, CONF_GCLK_ADC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	adc_async_init(&ADC_IB, ADC0, ADC_IB_map, ADC_IB_CH_MAX, ADC_IB_CH_AMOUNT, &ADC_IB_ch[0], (void *)NULL);
	adc_async_register_channel_buffer(&ADC_IB, 0, ADC_IB_buffer, ADC_IB_BUFFER_SIZE);

	// Disable digital pin circuitry
	gpio_set_pin_direction(ADC_PLUS, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(ADC_PLUS, PINMUX_PA02B_ADC0_AIN0);

	// Disable digital pin circuitry
	gpio_set_pin_direction(ADC_MINUS, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(ADC_MINUS, PINMUX_PA03B_ADC0_AIN1);

	// Disable digital pin circuitry
	gpio_set_pin_direction(ADC_B, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(ADC_B, PINMUX_PB09B_ADC0_AIN3);
}

/**
 * \brief ADC initialization function
 *
 * Enables ADC peripheral, clocks and initializes ADC driver
 */
void ADC_IA_init(void)
{
	hri_mclk_set_APBDMASK_ADC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC1_GCLK_ID, CONF_GCLK_ADC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	adc_async_init(&ADC_IA, ADC1, ADC_IA_map, ADC_IA_CH_MAX, ADC_IA_CH_AMOUNT, &ADC_IA_ch[0], (void *)NULL);
	adc_async_register_channel_buffer(&ADC_IA, 0, ADC_IA_buffer, ADC_IA_BUFFER_SIZE);

	// Disable digital pin circuitry
	gpio_set_pin_direction(ADC_A, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(ADC_A, PINMUX_PB08B_ADC1_AIN0);
}

void DIGITAL_GLUE_LOGIC_0_PORT_init(void)
{

	gpio_set_pin_function(HALL_C, PINMUX_PA04N_CCL_IN0);

	gpio_set_pin_function(HALL_B, PINMUX_PA05N_CCL_IN1);

	gpio_set_pin_function(HALL_A, PINMUX_PA06N_CCL_IN2);

	gpio_set_pin_function(HALL_OUT, PINMUX_PA07N_CCL_OUT0);
}

void DIGITAL_GLUE_LOGIC_0_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_CCL_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, CCL_GCLK_ID, CONF_GCLK_CCL_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void DIGITAL_GLUE_LOGIC_0_init(void)
{
	DIGITAL_GLUE_LOGIC_0_CLOCK_init();
	custom_logic_init();
	DIGITAL_GLUE_LOGIC_0_PORT_init();
}

void EVENT_SYSTEM_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EVSYS_GCLK_ID_0, CONF_GCLK_EVSYS_CHANNEL_0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, EVSYS_GCLK_ID_1, CONF_GCLK_EVSYS_CHANNEL_1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, EVSYS_GCLK_ID_2, CONF_GCLK_EVSYS_CHANNEL_2_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_EVSYS_bit(MCLK);

	event_system_init();
}

void QUAD_SPI_0_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(ECAT_SPI_CS, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ECAT_SPI_CS,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ECAT_SPI_CS, PINMUX_PB11H_QSPI_CS);

	gpio_set_pin_direction(ECAT_SPI_DATA0,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(ECAT_SPI_DATA0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(ECAT_SPI_DATA0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ECAT_SPI_DATA0,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA08H_QSPI_DATA0"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA08H_QSPI_DATA0);

	gpio_set_pin_direction(ECAT_SPI_DATA1,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(ECAT_SPI_DATA1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(ECAT_SPI_DATA1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ECAT_SPI_DATA1,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA09H_QSPI_DATA1"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA09H_QSPI_DATA1);

	gpio_set_pin_direction(ECAT_SPI_DATA2,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(ECAT_SPI_DATA2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(ECAT_SPI_DATA2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ECAT_SPI_DATA2,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA10H_QSPI_DATA2"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA10H_QSPI_DATA2);

	gpio_set_pin_direction(ECAT_SPI_DATA3,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(ECAT_SPI_DATA3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(ECAT_SPI_DATA3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ECAT_SPI_DATA3,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA11H_QSPI_DATA3"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA11H_QSPI_DATA3);

	// Set pin direction to input
	gpio_set_pin_direction(ECAT_SPI_SCK, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ECAT_SPI_SCK,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ECAT_SPI_SCK, PINMUX_PB10H_QSPI_SCK);
}

void QUAD_SPI_0_CLOCK_init(void)
{
	hri_mclk_set_AHBMASK_QSPI_bit(MCLK);
	hri_mclk_set_AHBMASK_QSPI_2X_bit(MCLK);
	hri_mclk_set_APBCMASK_QSPI_bit(MCLK);
}

void QUAD_SPI_0_init(void)
{
	QUAD_SPI_0_CLOCK_init();
	qspi_sync_init(&QUAD_SPI_0, QSPI);
	QUAD_SPI_0_PORT_init();
}

void SPI_ENC_PORT_init(void)
{

	gpio_set_pin_level(ENCODER_SPI_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ENCODER_SPI_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ENCODER_SPI_SCK, PINMUX_PB03D_SERCOM5_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(ENCODER_SPI_MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ENCODER_SPI_MISO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ENCODER_SPI_MISO, PINMUX_PA24D_SERCOM5_PAD2);

	gpio_set_pin_level(ENCODER_SPI_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ENCODER_SPI_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ENCODER_SPI_MOSI, PINMUX_PA25D_SERCOM5_PAD3);
}

void SPI_ENC_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void SPI_ENC_init(void)
{
	SPI_ENC_CLOCK_init();
	spi_m_async_init(&SPI_ENC, SERCOM5);
	SPI_ENC_PORT_init();
}

void TIMER_1_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_TC0_bit(MCLK);

	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void TIMER_0_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_TC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC1_GCLK_ID, CONF_GCLK_TC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_2_init(void)
{
	hri_mclk_set_APBBMASK_TC2_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC2_GCLK_ID, CONF_GCLK_TC2_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMER_2, TC2, _tc_get_timer());
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PWM_C, PINMUX_PA16F_TCC1_WO0);

	gpio_set_pin_function(PWM_B, PINMUX_PA17F_TCC1_WO1);

	gpio_set_pin_function(PWM_A, PINMUX_PA18F_TCC1_WO2);

	gpio_set_pin_function(EN_C, PINMUX_PA19F_TCC1_WO3);

	gpio_set_pin_function(EN_B, PINMUX_PA20F_TCC1_WO4);

	gpio_set_pin_function(EN_A, PINMUX_PA21F_TCC1_WO5);
}

void PWM_0_CLOCK_init(void)
{
	hri_mclk_set_APBBMASK_TCC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TCC1_GCLK_ID, CONF_GCLK_TCC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA12

	gpio_set_pin_level(ECAT_GPIO1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ECAT_GPIO1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ECAT_GPIO1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA13

	gpio_set_pin_level(ECAT_GPIO2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ECAT_GPIO2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ECAT_GPIO2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA14

	gpio_set_pin_level(ECAT_GPIO3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ECAT_GPIO3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ECAT_GPIO3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA15

	gpio_set_pin_level(ECAT_GPIO4,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ECAT_GPIO4, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ECAT_GPIO4, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA22

	gpio_set_pin_level(NSLEEP,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(NSLEEP, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(NSLEEP, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA23

	// Set pin direction to input
	gpio_set_pin_direction(NFAULT, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(NFAULT,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(NFAULT, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA27

	gpio_set_pin_level(STATUS_LED1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(STATUS_LED1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(STATUS_LED1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB02

	gpio_set_pin_level(ENCODER_SPI_NCS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(ENCODER_SPI_NCS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ENCODER_SPI_NCS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB23

	gpio_set_pin_level(STATUS_LED2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(STATUS_LED2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(STATUS_LED2, GPIO_PIN_FUNCTION_OFF);

	ADC_IB_init();
	ADC_IA_init();

	DIGITAL_GLUE_LOGIC_0_init();

	EVENT_SYSTEM_0_init();

	QUAD_SPI_0_init();

	SPI_ENC_init();

	TIMER_1_CLOCK_init();

	TIMER_1_init();

	TIMER_0_CLOCK_init();

	TIMER_0_init();

	TIMER_2_init();
	PWM_0_CLOCK_init();

	PWM_0_PORT_init();

	PWM_0_init();
}
