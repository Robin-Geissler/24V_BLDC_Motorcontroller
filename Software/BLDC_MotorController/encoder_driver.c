/*
 * encoder_driver.c
 *
 * Created: 03/02/2023 17:41:59
 *  
 */ 

#include "encoder_driver.h"

/*Read joint encoder position and return as float from 0 to 360 degrees*/
void conversion_joint_encoder(char* encoder_spi_reply, float* encoder_joint_pos, 
					unsigned int* encoder_joint_warning_bit, unsigned int* encoder_joint_error_bit)
{	
	*encoder_joint_warning_bit = (encoder_spi_reply[4] & (1<<0));
	*encoder_joint_error_bit = (encoder_spi_reply[4] & (1<<1) >> 1);
	
	volatile uint32_t encoder_value = 0;
	volatile float absolute_joint_position = 0;
	
	encoder_value = ((encoder_spi_reply[2])<<11) | (encoder_spi_reply[3]<<3) | (encoder_spi_reply[4]>> 5);
	
	//absolute readings.
	absolute_joint_position = ((encoder_value / (float)0x7FFFF) * 360) + ENCODER_OFFSET;
	 *encoder_joint_pos = absolute_joint_position;
	
	//new positions with damaged encoder
	if (absolute_joint_position <= NULL_POS_ABS_VALUE+ENCODER_OFFSET)
	{
		*encoder_joint_pos = -absolute_joint_position + NULL_POS_ABS_VALUE;
	}
	else
	{
		*encoder_joint_pos = -absolute_joint_position + NULL_POS_ABS_VALUE + 360;
	}
	 
	 
	//Readings offset to 0 degrees at start (horizontal test arm) and  110° at max left (turned ccw)
	 //*encoder_joint_pos = JOINT_RANGE*(absolute_joint_position - NULL_POS_ABS_VALUE)/(MAX_POS_ABS_VALUE-NULL_POS_ABS_VALUE);
	
	// TODO: load rx_buffer array with received data from the encodre
	//uint64_t dw_CRCinputData = 0;
	//volatile uint8_t calculated_crc=0;
	//dw_CRCinputData = ((uint64_t)encoder_spi_reply[0] << 32) + ((uint64_t)encoder_spi_reply[1] << 24) +
	//((uint64_t)encoder_spi_reply[2] << 16) + ((uint64_t)encoder_spi_reply[3] << 8) +
	//((uint64_t)encoder_spi_reply[4] << 0 );
	//calculated_crc = ~(CRC_SPI_97_64bit(dw_CRCinputData))& 0xFF; //inverted CRC
}

//uint8_t CRC_SPI_97_64bit(uint64_t dw_InputData)
//{
	//uint8_t b_Index = 0;
	//uint8_t b_CRC = 0;
	//b_Index = (uint8_t)((dw_InputData >> 56u) & (uint64_t)0x000000FFu);
	//b_CRC = (uint8_t)((dw_InputData >> 48u) & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = (uint8_t)((dw_InputData >> 40u) & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = (uint8_t)((dw_InputData >> 32u) & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = (uint8_t)((dw_InputData >> 24u) & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = (uint8_t)((dw_InputData >> 16u) & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = (uint8_t)((dw_InputData >> 8u) & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = (uint8_t)(dw_InputData & (uint64_t)0x000000FFu);
	//b_Index = b_CRC ^ ab_CRC8_LUT[b_Index];
	//b_CRC = ab_CRC8_LUT[b_Index];
	//return b_CRC;
//}