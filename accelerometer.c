///////////////////////////////////////////////////////////////////////////////
/// @file accelerometer.c
/// @author Olivier Tessier-Lariviere
/// @date 2016-07-14
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////

#include "accelerometer.h"

#include "nrf_delay.h"

struct bma2x2_t bma2x2;

#define	I2C_BUFFER_LEN 8
#define BMA2x2_BUS_READ_WRITE_ARRAY_INDEX	1
#define BMA_LATCH_DURATION		BMA2x2_LATCH_DURN_2S	//Latch duration of interrupts




/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{   
     ret_code_t err_code;
    
     switch(p_event->type)
     {
         case NRF_DRV_TWI_EVT_DONE:
             if(p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX)
             {
								m_set_mode_done  = true;
						 }
             else
             {
                 m_xfer_done = true;
             }
             break;
	 			case NRF_DRV_TWI_EVT_ADDRESS_NACK:
					//m_xfer_done = true;
	 				break;
	 			case NRF_DRV_TWI_EVT_DATA_NACK :
	 				break;
         default:
             break;        
     }   
}


                    
void twi_init (void)
{
    ret_code_t err_code;
    
    const nrf_drv_twi_config_t twi_config = {
       .scl                = ARDUINO_SCL_PIN,
       .sda                = ARDUINO_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOW
    };
    
		err_code = nrf_drv_twi_init(&m_twi_drv, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_twi_enable(&m_twi_drv);
		
}


void twi_reset()
{
	//nrf_drv_twi_disable(&m_twi_drv);
	
	uint32_t* reg_addr = (uint32_t*)(NRF_TWIM0_BASE + 0xFFC);
	*reg_addr = 0;	
	*(volatile uint32_t *) reg_addr;
	*reg_addr = 1;
	
}


/*******************************************************************************
 * @fn          bma_i2c_write
 *
 * @brief       Redirect the bma2x2 functions to write data to the bma280
 *
 * @return      0 if wrote
 */
int8_t bma_i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t length)
{
	
	u8 array[I2C_BUFFER_LEN];
	array[0] = reg_addr;
	
	for(u8 i = 0; i < length; i++)
	{
		array[i + 1] = *(reg_data + i);
	}
	
	uint32_t err_code = nrf_drv_twi_tx(&m_twi_drv, dev_addr, array, length + 1, true);
	APP_ERROR_CHECK(err_code);
	
	while(m_set_mode_done == false);
	m_set_mode_done = false;
	
	return 0;
}


/*******************************************************************************
 * @fn          bma_i2c_read
 *
 * @brief       Redirect the bma2x2 functions to read data from the bma280
 *
 * @return      0 if read
 */
int8_t bma_i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t length)
{
	

	//	for (stringpos = BMA2x2_INIT_VALUE; stringpos < length; stringpos++) {
	//		array[stringpos + BMA2x2_BUS_READ_WRITE_ARRAY_INDEX] =
	//		*(reg_data + stringpos);
	//	}
	
	uint8_t addr[1];
	
	addr[0] = reg_addr;	
	
	uint32_t err_code = nrf_drv_twi_tx(&m_twi_drv, dev_addr, addr, 1, true);  
  APP_ERROR_CHECK(err_code);
	
	while(m_set_mode_done == false);
	m_set_mode_done = false;
	
	err_code = nrf_drv_twi_rx(&m_twi_drv, dev_addr, reg_data, length);
  APP_ERROR_CHECK(err_code);
	
	while(m_xfer_done == false);
	m_xfer_done = false;
	
	return 0;
	
}

/*******************************************************************************
 * @fn          bma_delay_msec
 *
 * @brief       Apply a delay, needed for bma2x2 functions
 *
 * @return      none
 */
void bma_delay_msec(uint32_t msec)
{
	nrf_delay_ms(msec);
}

/*--------------------------------------------------------------------------*
*	The following function is used to map the I2C bus read, write, delay and
*	device address with global structure bma2x2_t
*-------------------------------------------------------------------------*/
s8 I2C_routine(void)
{
/*--------------------------------------------------------------------------*
 *  By using bma2x2 the following structure parameter can be accessed
 *	Bus write function pointer: BMA2x2_WR_FUNC_PTR
 *	Bus read function pointer: BMA2x2_RD_FUNC_PTR
 *	Delay function pointer: delay_msec
 *	I2C address: dev_addr
 *--------------------------------------------------------------------------*/
	
	bma2x2.bus_write = bma_i2c_write;
	bma2x2.bus_read = bma_i2c_read;
	bma2x2.delay_msec = bma_delay_msec;
	bma2x2.dev_addr = BMA2x2_I2C_ADDR1;
	

	return BMA2x2_INIT_VALUE;
}


void bma280_init()
{
	
	/* Local variable used to assign the bandwidth value*/
	u8 bw_value_u8 = BMA2x2_INIT_VALUE;
	
	
	
	/*********************** START INITIALIZATION ************************/
	
	// Initialize nordic TWI driver
	twi_init();
	nrf_delay_ms(2);
	// Initialize global structure bma2x2_t.
	I2C_routine();
	
	
	bma2x2_init(&bma2x2);
	
	// For initialization it is required to set the mode of
  // the sensor as "NORMAL"
	bma2x2_set_power_mode(BMA2x2_MODE_DEEP_SUSPEND);//BMA2x2_MODE_NORMAL);
	
	bw_value_u8 = 0x08;/* set bandwidth of 7.81Hz*/
	bma2x2_set_bw(bw_value_u8);

	//bma2x2_set_power_mode(BMA2x2_MODE_NORMAL);//BMA2x2_MODE_DEEP_SUSPEND);

	//static uint8_t varTest1 = 0x12;
	//bma2x2_get_power_mode(&varTest1);
	
	
//	bma2x2_set_latch_intr(BMA_LATCH_DURATION);
//	bma2x2_set_new_data(BMA2x2_INTR1_NEWDATA, true);
//	bma2x2_set_intr_enable(BMA2x2_DATA_ENABLE, true);
//	bma2x2_rst_intr(1);
	
	
	static uint8_t varTest2 = 0x12;
	
	//set int pin config to avoid too high consumption
	nrf_delay_ms(2);
	bma2x2_set_intr_output_type(BMA2x2_INTR1_OUTPUT, PUSS_PULL);	//PUSS_PULL (default)  OPEN_DRAIN
	bma2x2_set_intr_output_type(BMA2x2_INTR2_OUTPUT, PUSS_PULL);	//PUSS_PULL (default)  OPEN_DRAIN
	bma2x2_set_intr_level(BMA2x2_INTR1_LEVEL, ACTIVE_LOW);			//ACTIVE_HIGH (default)  ACTIVE_LOW
	bma2x2_set_intr_level(BMA2x2_INTR2_LEVEL, ACTIVE_LOW);			//ACTIVE_HIGH (default)  ACTIVE_LOW
	nrf_delay_ms(2);
	
	
//	bma2x2_get_intr_output_type(BMA2x2_INTR1_OUTPUT, &varTest2);	//PUSS_PULL (default)  OPEN_DRAIN
//	bma2x2_get_intr_output_type(BMA2x2_INTR2_OUTPUT, &varTest2);	//PUSS_PULL (default)  OPEN_DRAIN
//	bma2x2_get_intr_level(BMA2x2_INTR1_LEVEL, &varTest2);			//ACTIVE_HIGH (default)  ACTIVE_LOW
//	bma2x2_get_intr_level(BMA2x2_INTR2_LEVEL, &varTest2);			//ACTIVE_HIGH (default)  ACTIVE_LOW
	
	
//	
//	s16	accel_x_s16;
//	s16	accel_y_s16;
//	s16	accel_z_s16;
//	
//	bma2x2_read_accel_x(&accel_x_s16);
//	bma2x2_read_accel_y(&accel_y_s16);
//	bma2x2_read_accel_z(&accel_z_s16);
}

void bma280_read(s16 *accel_x_s16, s16 *accel_y_s16, s16 *accel_z_s16)
{
	bma2x2_read_accel_x(accel_x_s16);
	bma2x2_read_accel_y(accel_y_s16);
	bma2x2_read_accel_z(accel_z_s16);
}

void bma280_read_accel_xyz(uint8_t *accel_array)
{
	bool success = 0;
	struct bma2x2_accel_data bma280_accel_data;
	s16	accel_x_s16;
	s16	accel_y_s16;
	s16	accel_z_s16;
	bma2x2_read_accel_x(&accel_x_s16);
	bma2x2_read_accel_y(&accel_y_s16);
	bma2x2_read_accel_z(&accel_z_s16);
	//success = bma2x2_read_accel_xyz(&bma280_accel_data);
	
	if(success == 0)
	{
		accel_array[0] = (bma280_accel_data.x ) 	 & 0xFF ;
		accel_array[1] = (bma280_accel_data.x >> 8) & 0xFF ;
		
		accel_array[2] = (bma280_accel_data.y ) 	 & 0xFF ;
		accel_array[3] = (bma280_accel_data.y >> 8) & 0xFF ;
		
		accel_array[4] = (bma280_accel_data.z ) 	 & 0xFF ;
		accel_array[5] = (bma280_accel_data.z >> 8) & 0xFF ;
	}
}

void bma280_set_off()
{
	bma2x2_set_power_mode(BMA2x2_MODE_DEEP_SUSPEND);
}

void bma280_set_on()
{
	bma2x2_set_power_mode(BMA2x2_MODE_NORMAL);
}



