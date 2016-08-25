///////////////////////////////////////////////////////////////////////////////
/// @file ats.c
/// @author Olivier Tessier-Lariviere
/// @date 2016-06-22
/// @version 1.0
///
///
///////////////////////////////////////////////////////////////////////////////

#include "ble_ats.h"
#include <string.h>
#include "nordic_common.h"
#include "sdk_common.h"
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "app_util.h"

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_ats       Ambiant Temperature Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_ats_t * p_ats, ble_evt_t * p_ble_evt)
{
    p_ats->conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_ats       Ambiant Temperature Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_ats_t * p_ats, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_ats->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the HVC event.
 *
 * @details Handles HVC events from the BLE stack.
 *
 * @param[in]   p_ats       Ambiant Temperature Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
//static void on_hvc(ble_ats_t * p_ats, ble_evt_t * p_ble_evt)
//{
//    ble_gatts_evt_hvc_t * p_hvc = &p_ble_evt->evt.gatts_evt.params.hvc;

//    if (p_hvc->handle == p_ats->meas_handles.value_handle)
//    {
//        ble_ats_evt_t evt;

//        evt.evt_type = BLE_ATS_EVT_INDICATION_CONFIRMED;
//        p_ats->evt_handler(p_ats, &evt);
//    }
//}

void ble_ats_on_ble_evt(ble_ats_t * p_ats, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_ats, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_ats, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            //on_write(p_ats, p_ble_evt);
            break;

        case BLE_GATTS_EVT_HVC:
            //on_hvc(p_ats, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}
/**@brief Function for adding vSolar Measurement characteristic.
 *
 * @param[in]   p_ats        Sensors Service structure.
 * @param[in]   p_ats_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t ats_vsolar_char_add(ble_ats_t * p_ats, const ble_ats_init_t * p_ats_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    cccd_md.write_perm = p_ats_init->ats_meas_attr_md.cccd_write_perm;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read = 1;
		char_md.char_props.notify = 1;
    char_md.p_char_user_desc    = NULL;
    char_md.p_char_pf           = NULL;
    char_md.p_user_desc_md      = NULL;
    char_md.p_cccd_md           = &cccd_md;
    char_md.p_sccd_md           = NULL;

    ble_uuid.type = p_ats->uuid_type;
    ble_uuid.uuid = ATS_UUID_VSOLAR_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.read_perm  = p_ats_init->ats_meas_attr_md.read_perm;
    attr_md.write_perm = p_ats_init->ats_meas_attr_md.write_perm;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint16_t);
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_ats->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ats->vsolar_handle);
}

/**@brief Function for adding accelerometer measurement characteristic.
 *
 * @param[in]   p_ats        Sensors Service structure.
 * @param[in]   p_ats_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t ats_accelerometer_data_char_add(ble_ats_t * p_ats, const ble_ats_init_t * p_ats_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    cccd_md.write_perm = p_ats_init->ats_meas_attr_md.cccd_write_perm;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read = 1;
		char_md.char_props.notify = 1;
    char_md.p_char_user_desc    = NULL;
    char_md.p_char_pf           = NULL;
    char_md.p_user_desc_md      = NULL;
    char_md.p_cccd_md           = &cccd_md;
    char_md.p_sccd_md           = NULL;

    ble_uuid.type = p_ats->uuid_type;
    ble_uuid.uuid = ATS_UUID_ACCELEROMETER_DATA_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.read_perm  = p_ats_init->ats_meas_attr_md.read_perm;
    attr_md.write_perm = p_ats_init->ats_meas_attr_md.write_perm;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 6;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_ats->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ats->accelerometer_data_handle);
}


/**@brief Function for adding Ambiant temperature Measurement characteristics.
 *
 * @param[in]   p_ats        Health Thermometer Service structure.
 * @param[in]   p_ats_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t ats_temperature_char_add(ble_ats_t * p_ats, const ble_ats_init_t * p_ats_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    cccd_md.write_perm = p_ats_init->ats_meas_attr_md.cccd_write_perm;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read = 1;
		char_md.char_props.notify = 1;
    char_md.p_char_user_desc    = NULL;
    char_md.p_char_pf           = NULL;
    char_md.p_user_desc_md      = NULL;
    char_md.p_cccd_md           = &cccd_md;
    char_md.p_sccd_md           = NULL;

    ble_uuid.type = p_ats->uuid_type;
    ble_uuid.uuid = ATS_UUID_TEMPERATURE_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.read_perm  = p_ats_init->ats_meas_attr_md.read_perm;
    attr_md.write_perm = p_ats_init->ats_meas_attr_md.write_perm;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint32_t);
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_ats->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ats->temperature_handle);
}

uint32_t ble_ats_init(ble_ats_t * p_ats, const ble_ats_init_t * p_ats_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_ats->evt_handler = p_ats_init->evt_handler;
    p_ats->conn_handle = BLE_CONN_HANDLE_INVALID;
    //p_ats->temp_type   = p_ats_init->temp_type;

    // Add service
    //BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_HEALTH_THERMOMETER_SERVICE);
	
	  // Add service.
    ble_uuid128_t base_uuid = {ATS_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_ats->uuid_type);
		
		ble_uuid.type = p_ats->uuid_type;
    ble_uuid.uuid = ATS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_ats->service_handle);
		VERIFY_SUCCESS(err_code);

    // Add temperature characteristic
    err_code = ats_temperature_char_add(p_ats, p_ats_init);
		VERIFY_SUCCESS(err_code);
		
		// Add vsolar characteristic
		err_code = ats_vsolar_char_add(p_ats, p_ats_init);
		VERIFY_SUCCESS(err_code);
		
		// Add accelerometer_data characteristic
		err_code = ats_accelerometer_data_char_add(p_ats, p_ats_init);
		VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

uint32_t ble_ats_temperature_update(ble_ats_t * p_ats, uint32_t temperature)
{
	
	  if (p_ats == NULL)
        return NRF_ERROR_NULL;
    
    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

		// Initialize value struct.
		memset(&gatts_value, 0, sizeof(gatts_value));

		gatts_value.len     = sizeof(temperature);
		gatts_value.offset  = 0;
		gatts_value.p_value = (uint8_t *)&temperature;

		// Update database.
		err_code = sd_ble_gatts_value_set(p_ats->conn_handle,
																			p_ats->temperature_handle.value_handle,
																			&gatts_value);

		// Send value if connected and notifying.
		if ((p_ats->conn_handle != BLE_CONN_HANDLE_INVALID)) //&& p_ats->is_notification_supported)
		{
				ble_gatts_hvx_params_t hvx_params;

				memset(&hvx_params, 0, sizeof(hvx_params));

				hvx_params.handle = p_ats->temperature_handle.value_handle;
				hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
				hvx_params.offset = gatts_value.offset;
				hvx_params.p_len  = &gatts_value.len;
				hvx_params.p_data = gatts_value.p_value;

				err_code = sd_ble_gatts_hvx(p_ats->conn_handle, &hvx_params);
		}
		else
		{
				err_code = NRF_ERROR_INVALID_STATE;
		}
    

    return err_code;
	
}

uint32_t ble_ats_vsolar_update(ble_ats_t * p_ats, uint16_t vsolar)
{
	
	  if (p_ats == NULL)
        return NRF_ERROR_NULL;
    
    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

		// Initialize value struct.
		memset(&gatts_value, 0, sizeof(gatts_value));

		gatts_value.len     = sizeof(vsolar);
		gatts_value.offset  = 0;
		gatts_value.p_value = (uint8_t *)&vsolar;

		// Update database.
		err_code = sd_ble_gatts_value_set(p_ats->conn_handle,
																			p_ats->vsolar_handle.value_handle,
																			&gatts_value);

		// Send value if connected and notifying.
		if ((p_ats->conn_handle != BLE_CONN_HANDLE_INVALID)) //&& p_ats->is_notification_supported)
		{
				ble_gatts_hvx_params_t hvx_params;

				memset(&hvx_params, 0, sizeof(hvx_params));

				hvx_params.handle = p_ats->vsolar_handle.value_handle;
				hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
				hvx_params.offset = gatts_value.offset;
				hvx_params.p_len  = &gatts_value.len;
				hvx_params.p_data = gatts_value.p_value;

				err_code = sd_ble_gatts_hvx(p_ats->conn_handle, &hvx_params);
		}
		else
		{
				err_code = NRF_ERROR_INVALID_STATE;
		}
    

    return err_code;
	
}

uint32_t ble_ats_accelerometer_data_update(ble_ats_t * p_ats, uint8_t* accel_data)
{
	
	  if (p_ats == NULL)
        return NRF_ERROR_NULL;
    
    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

		// Initialize value struct.
		memset(&gatts_value, 0, sizeof(gatts_value));

		gatts_value.len     = 6;
		gatts_value.offset  = 0;
		gatts_value.p_value = accel_data;

		// Update database.
		err_code = sd_ble_gatts_value_set(p_ats->conn_handle,
																			p_ats->accelerometer_data_handle.value_handle,
																			&gatts_value);

		// Send value if connected and notifying.
		if ((p_ats->conn_handle != BLE_CONN_HANDLE_INVALID)) //&& p_ats->is_notification_supported)
		{
				ble_gatts_hvx_params_t hvx_params;

				memset(&hvx_params, 0, sizeof(hvx_params));

				hvx_params.handle = p_ats->accelerometer_data_handle.value_handle;
				hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
				hvx_params.offset = gatts_value.offset;
				hvx_params.p_len  = &gatts_value.len;
				hvx_params.p_data = gatts_value.p_value;

				err_code = sd_ble_gatts_hvx(p_ats->conn_handle, &hvx_params);
		}
		else
		{
				err_code = NRF_ERROR_INVALID_STATE;
		}
    

    return err_code;
	
}

