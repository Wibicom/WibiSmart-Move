///////////////////////////////////////////////////////////////////////////////
/// @file light_sensor.h
/// @author Olivier Tessier-Lariviere
/// @date 2016-05-24
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////

#ifndef LIGH_SENSOR_H
#define  LIGH_SENSOR_H



#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

#include "bma2x2.h"


// OPT3001 I2C address (ADD pin is connected to ground)
#define OPT3001_I2C_ADDR 0x44

static uint8_t tmp_data[3];

/* TWI instance. */
static const nrf_drv_twi_t m_twi_drv = NRF_DRV_TWI_INSTANCE(0);

/* Indicates if reading operation from the light sensor has ended. */
static volatile bool m_xfer_done = false;

/* Indicates if setting mode operation has ended. */
static volatile bool m_set_mode_done = false;

uint8_t* get_ambient_light(void);

float decode_data(void);

void OPT3001_Configuration(void);
                    
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);

void twi_init (void);

void bma280_init(void);

void bma280_read(s16 *accel_x_s16, s16 *accel_y_s16, s16 *accel_z_s16);

void bma280_set_off(void);

void bma280_set_on(void);

void twi_reset(void);

void bma280_read_accel_xyz(uint8_t *accel_array);


#endif

