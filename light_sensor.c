///////////////////////////////////////////////////////////////////////////////
/// @file light_sensor.c
/// @author Olivier Tessier-Lariviere
/// @date 2016-05-24
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////

#include "light_sensor.h"

uint8_t* get_ambient_light()
{
	 ret_code_t err_code;
	 tmp_data[0] = 0x00;
	 err_code = nrf_drv_twi_tx(&m_twi_drv, OPT3001_I2C_ADDR, tmp_data, 1, true);
	 APP_ERROR_CHECK(err_code);
	
	 while(m_xfer_done == false){/*wait until the transfer is done.*/};
	
	 return tmp_data;
}

float decode_data(void)
{
//    unsigned int DataSum;
//    float Ambient_Data;

//    DataSum = ((tmp_data[0] << 8) | tmp_data[1]);                                 // Justify Ambient values
//    tmp_data[0] = tmp_data[0] >> 4;                                               // Justify exponent data
//    DataSum = DataSum << 4;                                                       // Justify fractional results
//    DataSum = DataSum >> 4;
//    
//    Ambient_Data = 0.01 * (2 << tmp_data[0]) * DataSum;                           // Lux equation
	
	  uint16_t raw = ((tmp_data[0] << 8) | tmp_data[1]);
    uint16_t result, exponent;
    float lux;

    result = raw & 0x0FFF;
    exponent = (raw>>12) & 0x000F;

    lux=0.01*(1<<exponent)*result;

    
    return lux;
}

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
								if(m_set_mode_done != true)
								{
									 m_set_mode_done  = true;
									 return;
								}
								m_xfer_done = false;
								err_code = nrf_drv_twi_rx(&m_twi_drv, OPT3001_I2C_ADDR, (uint8_t*)&tmp_data, 3);
								APP_ERROR_CHECK(err_code);
						 }
             else
             {
                 m_xfer_done = true;
                
                 // **** We need to transmit data here ****
                 float ambient_light = decode_data();
								 //iBeacon_info[0] = tmp_data[0];
								 //iBeacon_info[1] = tmp_data[1];
								 //iBeacon_info[2] = tmp_data[2];
									
             }
             break;
	 			case NRF_DRV_TWI_EVT_ADDRESS_NACK:
	 				break;
	 			case NRF_DRV_TWI_EVT_DATA_NACK :
	 				break;
         default:
             break;        
     }   
}

void OPT3001_Configuration(){
  tmp_data[0] = 0x01;                                                           // Configuration register
  tmp_data[1] = 0xC6;                                                           // Set Mode of conversion operation field as
  tmp_data[2] = 0x10;                                                           // 11 - Continuous conversions
                                                              
  ret_code_t err_code = nrf_drv_twi_tx(&m_twi_drv, OPT3001_I2C_ADDR, tmp_data, 3, false);
  APP_ERROR_CHECK(err_code);    	// Send 3 bytes (tmp_data)
	
	while(m_set_mode_done == false);
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




