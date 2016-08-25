/** @file
 *
 * @defgroup ble_sdk_srv_ads LED Button Service Server
 * @{
 * @ingroup ble_sdk_srv
 *
 * @brief LED Button Service Server module.
 *
 * @details This module implements a custom LED Button Service with an LED and Button Characteristics.
 *          During initialization, the module adds the LED Button Service and Characteristics
 *          to the BLE stack database.
 *
 *          The application must supply an event handler for receiving LED Button Service
 *          events. Using this handler, the service notifies the application when the
 *          LED value changes.
 *
 *          The service also provides a function for letting the application notify
 *          the state of the Button Characteristic to connected peers.
 *
 * @note The application must propagate BLE stack events to the LED Button Service
 *       module by calling ble_ads_on_ble_evt() from the @ref softdevice_handler callback.
*/

#ifndef BLE_ADS_H__
#define BLE_ADS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define ADS_UUID_BASE        {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                              0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define ADS_UUID_SERVICE     0x1520//3

#define ADS_UUID_MODE_CHAR    0x3020
#define ADS_UUID_INTERVAL_CHAR 0x3021
#define ADS_UUID_TX_POWER_CHAR 0x3022
#define ADS_UUID_SMART_POWER_CHAR 0x3023
#define ADS_UUID_PAYLOAD_CHAR 0x3024

// Forward declaration of the ble_ads_t type. 
typedef struct ble_ads_s ble_ads_t;

typedef void (*ble_ads_write_handler_t) (ble_ads_t * p_ads, uint8_t* data);

/** @brief Advertising mode Service init structure. This structure contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_ads_write_handler_t adv_mode_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
	  ble_ads_write_handler_t adv_interval_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
	  ble_ads_write_handler_t adv_tx_power_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
	  ble_ads_write_handler_t adv_smart_power_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
	  ble_ads_write_handler_t adv_payload_write_handler;
} ble_ads_init_t;

/**@brief Advertising mode Service structure. This structure contains various status information for the service. */
struct ble_ads_s
{
    uint16_t                    service_handle;      /**< Handle of LED Button Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    adv_mode_char_handles;    /**< Handles related to the LED Characteristic. */
    ble_gatts_char_handles_t    adv_interval_char_handles; /**< Handles related to the Button Characteristic. */
	  ble_gatts_char_handles_t    adv_tx_power_char_handles; /**< Handles related to the Button Characteristic. */
	  ble_gatts_char_handles_t    adv_smart_power_char_handles; /**< Handles related to the Button Characteristic. */
	  ble_gatts_char_handles_t    adv_payload_char_handles;
    uint8_t                     uuid_type;           /**< UUID type for the LED Button Service. */
    uint16_t                    conn_handle;         /**< Handle of the current connection (as provided by the BLE stack). BLE_CONN_HANDLE_INVALID if not in a connection. */
    ble_ads_write_handler_t     adv_mode_write_handler;   /**< Event handler to be called when the LED Characteristic is written. */
	  ble_ads_write_handler_t     adv_interval_write_handler;
	  ble_ads_write_handler_t     adv_tx_power_write_handler;
	  ble_ads_write_handler_t     adv_smart_power_write_handler;
	  ble_ads_write_handler_t			adv_payload_write_handler;
};

/**@brief Function for initializing the LED Button Service.
 *
 * @param[out] p_ads      LED Button Service structure. This structure must be supplied by
 *                        the application. It is initialized by this function and will later
 *                        be used to identify this particular service instance.
 * @param[in] p_ads_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t ble_ads_init(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init);

/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the LED Button Service.
 *
 * @param[in] p_ads      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
void ble_ads_on_ble_evt(ble_ads_t * p_ads, ble_evt_t * p_ble_evt);

/**@brief Function for sending a button state notification.
 *
 * @param[in] p_ads      LED Button Service structure.
 * @param[in] button_state  New button state.
 *
 * @retval NRF_SUCCESS If the notification was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_ads_on_button_change(ble_ads_t * p_ads, uint8_t button_state);

#endif // BLE_ADS_H__

/** @} */
