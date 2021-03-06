#include "ble_ads.h"
#include "ble_srv_common.h"
#include "sdk_common.h"


/**@brief Function for handling the Connect event.
 *
 * @param[in] p_ads      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_connect(ble_ads_t * p_ads, ble_evt_t * p_ble_evt)
{
    p_ads->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in] p_ads      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_disconnect(ble_ads_t * p_ads, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_ads->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the Write event.
 *
 * @param[in] p_ads      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_write(ble_ads_t * p_ads, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
		
		if(p_evt_write->handle == p_ads->adv_mode_char_handles.value_handle)
		{
			p_ads->adv_mode_write_handler(p_ads, p_evt_write->data);
		}
		else if(p_evt_write->handle == p_ads->adv_interval_char_handles.value_handle)
		{
			p_ads->adv_interval_write_handler(p_ads, p_evt_write->data);
		}
		else if(p_evt_write->handle == p_ads->adv_tx_power_char_handles.value_handle)
		{
			p_ads->adv_tx_power_write_handler(p_ads, p_evt_write->data);
		}
		else if(p_evt_write->handle == p_ads->adv_smart_power_char_handles.value_handle)
		{
			p_ads->adv_smart_power_write_handler(p_ads, p_evt_write->data);
		}
		else if(p_evt_write->handle == p_ads->adv_payload_char_handles.value_handle)
		{
			p_ads->adv_payload_write_handler(p_ads, p_evt_write->data);
		}
}


void ble_ads_on_ble_evt(ble_ads_t * p_ads, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_ads, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_ads, p_ble_evt);
            break;
            
        case BLE_GATTS_EVT_WRITE:
            on_write(p_ads, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding the advertising mode characteristic
 *
 * @param[in] p_ads      Advertising Service structure.
 * @param[in] p_ads_init Advertising Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t adv_mode_char_add(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_ads->uuid_type;
    ble_uuid.uuid = ADS_UUID_MODE_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_ads->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ads->adv_mode_char_handles);
}

/**@brief Function for adding the advertising interval characteristic
 *
 * @param[in] p_ads      Advertising Service structure.
 * @param[in] p_ads_init Advertising Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t adv_interval_char_add(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_ads->uuid_type;
    ble_uuid.uuid = ADS_UUID_INTERVAL_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_ads->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ads->adv_interval_char_handles);
}

/**@brief Function for adding the advertising transmit power characteristic
 *
 * @param[in] p_ads      Advertising Service structure.
 * @param[in] p_ads_init Advertising Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t adv_tx_power_char_add(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_ads->uuid_type;
    ble_uuid.uuid = ADS_UUID_TX_POWER_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_ads->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ads->adv_tx_power_char_handles);
}

/**@brief Function for adding the advertising smart power characteristic
 *
 * @param[in] p_ads      Advertising Service structure.
 * @param[in] p_ads_init Advertising Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t adv_smart_power_char_add(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_ads->uuid_type;
    ble_uuid.uuid = ADS_UUID_SMART_POWER_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_ads->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ads->adv_smart_power_char_handles);
}

/**@brief Function for adding the advertising payload characteristic
 *
 * @param[in] p_ads      Advertising Service structure.
 * @param[in] p_ads_init Advertising Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t adv_payload_char_add(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_ads->uuid_type;
    ble_uuid.uuid = ADS_UUID_PAYLOAD_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = 20; //* sizeof(uint8_t);
    attr_char_value.p_value      = NULL;

    return sd_ble_gatts_characteristic_add(p_ads->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_ads->adv_payload_char_handles);
}

uint32_t ble_ads_init(ble_ads_t * p_ads, const ble_ads_init_t * p_ads_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure.
    p_ads->conn_handle       = BLE_CONN_HANDLE_INVALID;
    p_ads->adv_mode_write_handler = p_ads_init->adv_mode_write_handler;
	  p_ads->adv_interval_write_handler = p_ads_init->adv_interval_write_handler;
	  p_ads->adv_tx_power_write_handler = p_ads_init->adv_tx_power_write_handler;
	  p_ads->adv_smart_power_write_handler = p_ads_init->adv_smart_power_write_handler;
	  p_ads->adv_payload_write_handler = p_ads_init->adv_payload_write_handler;


    // Add service.
    ble_uuid128_t base_uuid = {ADS_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_ads->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_ads->uuid_type;
    ble_uuid.uuid = ADS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_ads->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add characteristics.
    err_code = adv_mode_char_add(p_ads, p_ads_init); // Advertising mode
    VERIFY_SUCCESS(err_code);
    err_code = adv_interval_char_add(p_ads, p_ads_init); // Advertising interval
    VERIFY_SUCCESS(err_code);
		err_code = adv_tx_power_char_add(p_ads, p_ads_init); // Advertising tx power
    VERIFY_SUCCESS(err_code);
		err_code = adv_smart_power_char_add(p_ads, p_ads_init); // Advertising smart power mode
    VERIFY_SUCCESS(err_code);
		err_code = adv_payload_char_add(p_ads, p_ads_init); // Advertising payload
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}
