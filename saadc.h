///////////////////////////////////////////////////////////////////////////////
/// @file saadc.h
/// @author Olivier Tessier-Lariviere
/// @date 2016-05-26
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "boards.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include <string.h>
#include "nrf_drv_saadc.h"


#define SAMPLES_NB 10 // Needs to be an even number
#define SAMPLES_IN_BUFFER 1
static volatile uint8_t state = 1;

static const nrf_drv_timer_t   m_timer = NRF_DRV_TIMER_INSTANCE(0);
static nrf_saadc_value_t       m_buffer_pool[2][SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t       m_ppi_channel;
static uint32_t                m_adc_evt_counter;
static bool is_sampling_done = false;
static uint16_t data_array[SAMPLES_IN_BUFFER];
static uint16_t data_ch_0 = 0;
static uint16_t data_ch_1 = 0;

static nrf_saadc_channel_config_t channel_0_config;
static nrf_saadc_channel_config_t channel_1_config;
static uint8_t m_adc_channel_enabled = 0; 

uint16_t saadc_get_data_ch_0(void);
uint16_t saadc_get_data_ch_1(void);

void saadc_reset_data(void);

uint16_t saadc_get_data(int channel);


void start_sampling(void);

void saadc_callback(nrf_drv_saadc_evt_t const * p_event);

void saadc_init(void);

