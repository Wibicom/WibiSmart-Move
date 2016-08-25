
///////////////////////////////////////////////////////////////////////////////
/// @file ats.h
/// @author Olivier Tessier-Lariviere
/// @date 2016-06-22
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_date_time.h"


#define ATS_UUID_BASE        {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                              0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}

#define ATS_UUID_SERVICE     0x1620
#define	ATS_UUID_TEMPERATURE_CHAR 0x3120
#define	ATS_UUID_VSOLAR_CHAR 0x3121
#define	ATS_UUID_ACCELEROMETER_DATA_CHAR 0x3122

/**@brief Ambiant temperature Service event type. */
typedef enum
{
    BLE_ATS_EVT_INDICATION_ENABLED,                                         /**< Ambiant temperature value indication enabled event. */
    BLE_ATS_EVT_INDICATION_DISABLED,                                        /**< Ambiant temperature value indication disabled event. */
    BLE_ATS_EVT_INDICATION_CONFIRMED                                        /**< Confirmation of a temperature measurement indication has been received. */
} ble_ats_evt_type_t;

/**@brief Ambiant temperature Service event. */
typedef struct
{
    ble_ats_evt_type_t evt_type;                                            /**< Type of event. */
} ble_ats_evt_t;

// Forward declaration of the ble_ats_t type. 
typedef struct ble_ats_s ble_ats_t;

/**@brief Ambiant temperature Service event handler type. */
typedef void (*ble_ats_evt_handler_t) (ble_ats_t * p_ats, ble_ats_evt_t * p_evt);

/**@brief Ambiant temperature Service init structure. This contains all options and data
 *        needed for initialization of the service. */
typedef struct
{
    ble_ats_evt_handler_t        evt_handler;                               /**< Event handler to be called for handling events in the Ambiant temperature Service. */
    ble_srv_cccd_security_mode_t ats_meas_attr_md;                          /**< Initial security level for Ambiant temperature measurement attribute */
    ble_srv_security_mode_t      ats_temp_type_attr_md;                     /**< Initial security level for Ambiant temperature tempearture type attribute */
    uint8_t                      temp_type_as_characteristic;               /**< Set non-zero if temp type given as characteristic */
    uint8_t                      temp_type;                                 /**< Temperature type if temperature characteristic is used */
} ble_ats_init_t;

/**@brief Ambiant temperature Service structure. This contains various status information for
 *        the service. */
struct ble_ats_s
{
    ble_ats_evt_handler_t        evt_handler;                               /**< Event handler to be called for handling events in the Ambiant temperature Service. */
    uint16_t                     service_handle;                            /**< Handle of Ambiant temperature Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t     temperature_handle;                              /**< Handles related to the Ambiant temperature Measurement characteristic. */
	  ble_gatts_char_handles_t     vsolar_handle;
	  ble_gatts_char_handles_t     accelerometer_data_handle;
    uint16_t                     conn_handle;                               /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
	  uint8_t                     uuid_type; 
};


uint32_t ble_ats_init(ble_ats_t * p_ats, const ble_ats_init_t * p_ats_init);

void ble_ats_on_ble_evt(ble_ats_t * p_ats, ble_evt_t * p_ble_evt);

//uint32_t ble_ats_measurement_send(ble_ats_t * p_ats, uint32_t* p_ats_meas);
uint32_t ble_ats_temperature_update(ble_ats_t * p_ats, uint32_t temperature);
uint32_t ble_ats_vsolar_update(ble_ats_t * p_ats, uint16_t vsolar);
uint32_t ble_ats_accelerometer_data_update(ble_ats_t * p_ats, uint8_t* accel_data);

uint32_t ble_ats_is_indication_enabled(ble_ats_t * p_ats, bool * p_indication_enabled);
