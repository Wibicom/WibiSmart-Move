///////////////////////////////////////////////////////////////////////////////
/// @file light_sensor.c
/// @author Olivier Tessier-Lariviere
/// @date 2016-05-26
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////


#include "saadc.h"
#include <math.h>

/**@brief Function that start the sampling of the SAADC.
*
* @details saadc_callback will be called when the sample is done.
*/
void start_sampling(void)
{
	 // Start sampling
	 ret_code_t err_code = nrf_drv_saadc_sample();
	 APP_ERROR_CHECK(err_code);
}

/**@brief Function that is called when the SAADC sampling is done
*
* @details Get the data from the buffer and place them in data_ch_0 (v_solar) & data_ch_1 (v_bat)
*/
void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
			  ret_code_t err_code;
		
        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);
				
				// Get data of the enabled channel.
			  if(m_adc_channel_enabled == 0)
					data_ch_0 += p_event->data.done.p_buffer[0];
				else if(m_adc_channel_enabled == 1)
					data_ch_1 += p_event->data.done.p_buffer[0];
		
				// Uninitialize SAADC to save power.
				nrf_drv_saadc_uninit();
				NRF_SAADC->INTENCLR = (SAADC_INTENCLR_END_Clear << SAADC_INTENCLR_END_Pos);
				NVIC_ClearPendingIRQ(SAADC_IRQn);
    }
}

/**@brief Function that initialize the SAADC.
*
* @details Initialize two channel for v_solar and v_bat respectively.
*/
void saadc_init(void)
{
    ret_code_t err_code;
	
    // Set configuration for saadc channel 0 (v_solar)
		channel_0_config.resistor_p = NRF_SAADC_RESISTOR_DISABLED;
    channel_0_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;
    channel_0_config.gain       = NRF_SAADC_GAIN1_6;
    channel_0_config.reference  = NRF_SAADC_REFERENCE_INTERNAL;
    channel_0_config.acq_time   = NRF_SAADC_ACQTIME_20US;
    channel_0_config.mode       = NRF_SAADC_MODE_SINGLE_ENDED;
    channel_0_config.pin_p      = (nrf_saadc_input_t)(NRF_SAADC_INPUT_AIN5);
    channel_0_config.pin_n      = NRF_SAADC_INPUT_DISABLED;
	
		// Set configuration for saadc channel 1 (v_bat)
		channel_1_config.resistor_p = NRF_SAADC_RESISTOR_DISABLED;
    channel_1_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;
    channel_1_config.gain       = NRF_SAADC_GAIN1_6;
    channel_1_config.reference  = NRF_SAADC_REFERENCE_INTERNAL;
    channel_1_config.acq_time   = NRF_SAADC_ACQTIME_20US;
    channel_1_config.mode       = NRF_SAADC_MODE_SINGLE_ENDED;
    channel_1_config.pin_p      = (nrf_saadc_input_t)(NRF_SAADC_INPUT_AIN4);
    channel_1_config.pin_n      = NRF_SAADC_INPUT_DISABLED;
	
		// Initialize SAADC driver
    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);
		
		// Initialize the enabled channel
		if(m_adc_channel_enabled == 0)
		{
				err_code = nrf_drv_saadc_channel_init(1, &channel_1_config);
				APP_ERROR_CHECK(err_code);
				m_adc_channel_enabled = 1;
		}
		else if(m_adc_channel_enabled == 1)
		{
				err_code = nrf_drv_saadc_channel_init(0, &channel_0_config);
				APP_ERROR_CHECK(err_code);
				m_adc_channel_enabled = 0;
		}
		
    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0],SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code); 
}

/**@brief Function that reset SAADC channels data to 0.
*
*/
void saadc_reset_data(void)
{
	data_ch_0 = 0; // v_solar
	data_ch_1 = 0; // v_bat
}

/**@brief Accessor of data in the channel 0 (v_solar)
*
*/
uint16_t saadc_get_data_ch_0(void)
{
	 return data_ch_0;
}

/**@brief Accessor of data in the channel 1 (v_bat)
*
*/
uint16_t saadc_get_data_ch_1(void)
{
	 return data_ch_1;
}
