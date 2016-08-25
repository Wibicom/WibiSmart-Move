///////////////////////////////////////////////////////////////////////////////
/// @file main.c
/// @author Olivier Tessier-Lariviere
/// @date 2016-05-10
/// @version 1.0
///
/// This class advertise ... 
///
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "ble_advdata.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_temp.h"
#include "ble_bas.h"
#include "ble_dis.h"
#include "ble_hts.h"
#include "ble_ads.h"
#include "ble_ats.h"
#include "ble_conn_params.h"
//#include "device_manager.h"
#include "pstorage.h"

#include "nrf_drv_clock.h"
#include "nrf_drv_gpiote.h"

#include "accelerometer.h"
#include "saadc.h"
#include "flash.h"

//#define BOARD_CUSTOM


#define CENTRAL_LINK_COUNT              0                                 // Number of central links used by the application. When changing this number remember to adjust the RAM settings.
#define PERIPHERAL_LINK_COUNT           1                                 // Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings

#define NON_CONNECTABLE_ADV_TIMEOUT     0                                 // Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. 
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(500, UNIT_0_625_MS) // The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). 
#define CONNECTABLE_ADV_INTERVAL        MSEC_TO_UNITS(500, UNIT_0_625_MS)              /**< The advertising interval for connectable advertisement (20 ms). This value can vary between 20ms to 10.24s. */
#define CONNECTABLE_ADV_TIMEOUT         0                                            /**< Time for which the device must be advertising in connectable mode (in seconds). */
#define TX_POWER_LEVEL									4                                 // the transmit power level can vary from -40dbm to 4dbm

#define APP_TIMER_PRESCALER             15                                 /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         3                                 /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                   0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

#define APP_COMPANY_IDENTIFIER          0x0000     //  Wibicom identifier coming soon 

#define DEVICE_NAME  "Move 05"
#define MANUFACTURER_NAME "Wibicom"

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER) /**< Time from initiating event (connect or start of indication) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                          /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND                  1                                          /**< Perform bonding. */
#define SEC_PARAM_MITM                  0                                          /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                  0                                          /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS              0                                          /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                       /**< No I/O capabilities. */
#define SEC_PARAM_OOB                   0                                          /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE          7                                          /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE          16                                         /**< Maximum encryption key size. */

void timeout_handler(void * p_context);

// User advertising settings
enum ADV_TYPE {iBeacon, eddystone_uid, eddystone_url, eddystone_tlm, alt_beacon};
bool is_connectable = true;
bool smart_power_mode_enabled = false;
uint16_t non_connectable_adv_interval = MSEC_TO_UNITS(1000, UNIT_0_625_MS);       // The advertising interval for non-connectable advertisement. (This value can vary between 100ms to 10.24s). 
int8_t tx_power_level = 0;		// The transmit power level can vary from -40dbm to 4dbm
uint8_t advertising_mode = -1; // Will be in connectable mode when it is powered on for the first time
bool is_in_beacon_mode = true;
bool test = true;

uint8_t* payload;
uint8_t payload_size;
uint8_t url_data_size;

// Storage block handles.
pstorage_handle_t				block_handle_1;
pstorage_handle_t				block_handle_2;
pstorage_handle_t				block_handle_3;
// Data to save in block 1.
uint8_t source_data_1[16] __attribute__((aligned(4))) = {0x23, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};
uint8_t destination_data_1[16]__attribute__((aligned(4)));
// Data to save in block 2.
uint8_t source_data_2[16] __attribute__((aligned(4))) = {0x23, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};;
uint8_t destination_data_2[16]__attribute__((aligned(4)));
// Data to save in block 3.
uint8_t source_data_3[16] __attribute__((aligned(4))) = {0x23, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F};;
uint8_t destination_data_3[16]__attribute__((aligned(4)));
	
static uint8_t pstorage_wait_flag = 0;
static pstorage_block_t pstorage_wait_handle = 0;


static ble_bas_t  m_bas;                              // Structure used to identify the battery service. 
static ble_ats_t  m_ats;                              // Structure used to identify the ambiant temperature service.
static ble_ads_t  m_ads; 															// Structure used to identify the advertising service.

typedef uint8_t 	nrf_power_mode_t;
int32_t volatile temp = 0x00;

uint16_t v_solar;
uint16_t v_bat;
uint8_t volatile cpt = 0;
APP_TIMER_DEF(timer_id);


static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;   /**< Handle of the current connection. */
//static dm_application_instance_t        m_app_handle;                              /**< Application identifier allocated by device manager */

static uint8_t beacon_payload[23];

static ble_gap_adv_params_t m_adv_params;            // Parameters to be passed to the stack when starting advertising.
static uint8_t iBeacon_info[] =                    // Information advertised by the Beacon.
{
	0x02, 0x15, 																																													 // iBeacon config data.
	0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,        // UUID
	0x01, 0x00, 																																													 // Major
	0x02, 0x01,																																														 // Minor
	0xEE 																																																	 // RSSI
};

static uint8_t eddystone_uid_data[] =   /**< Information advertised by the Eddystone UID frame type. */
{
    0x00,                                                          // Eddystone UID frame type.
    0xEE,                                                          // RSSI value at 0 m.
    0xAA, 0xAA, 0xBB, 0xBB, 0xCC, 0xCC, 0xDD, 0xDD, 0xEE, 0xEE,    // 10-byte namespace value. Similar to Beacon Major.
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,                            // 6-byte ID value. Similar to Beacon Minor.
    0x00, 0x00                                                     // Reserved for future use.
};

static uint8_t eddystone_tlm_data[] =   /**< Information advertised by the Eddystone TLM frame type. */
{
   0x20,                      // Eddystone TLM frame type.
   0x00,                      // Eddystone TLM version.
   0x00, 0xF0,                // Battery voltage in mV/bit.
   0xF0, 0x00,                // Temperature [C].
   0x00, 0x00, 0x00, 0x00,    // Number of advertisements since power-up or reboot.
   0x00, 0x00, 0x00, 0x00     // Time since power-up or reboot. 0.1 s increments.
};

static uint8_t eddystone_url_data[] =   /**< Information advertised by the Eddystone URL frame type. */
{
    0x10,                     // Eddystone URL frame type.
    0xEE,                     // RSSI value at 0 m.
    0x00,                     // Scheme or prefix for URL ("http", "http://www", etc.)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //'w', 'i', 'b', 'i', 'c', 'o', 'm', 0x00     // URL with a maximum length of 17 bytes. Last byte is suffix (".com", ".org", etc.)
};

static    ble_uuid_t m_adv_uuids[] = {{ADS_UUID_SERVICE, BLE_UUID_TYPE_BLE},
																			{BLE_UUID_HEALTH_THERMOMETER_SERVICE, BLE_UUID_TYPE_BLE},
																			{BLE_UUID_BATTERY_SERVICE,            BLE_UUID_TYPE_BLE},
                                      {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}}; /**< Universally unique service identifiers. */
static void advertising_start();
void timers_start(void);
static void save_settings(void);		
static void load_settings(void);		
static void update_advertising();
void set_characteristic( ble_evt_t * p_ble_evt, uint16_t len, uint16_t offset, uint8_t* p_value, uint16_t value_handle);

uint8_t dummyValue = 0;			
ble_gatts_value_t value_adv_mode;
ble_gatts_value_t value_adv_interval;
ble_gatts_value_t value_adv_power;
ble_gatts_value_t value_smart_power;																				
																	
/**@brief Function for handling the Application's BLE stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
																			
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
				
				    // Update characteristics value.
//					
//						// Set advertising mode characteristic.
//						set_characteristic(p_ble_evt, 1, 0, &adv_mode, m_ads.adv_mode_char_handles.value_handle);
//				
//						// Set advertising intervals characteristic.
//				    set_characteristic(p_ble_evt, 2, 0, (uint8_t *)&non_connectable_adv_interval, m_ads.adv_interval_char_handles.value_handle);
//						
//						// Set transmit power mode characteristic.
//						//get_tx_power_setting();
//				    set_characteristic(p_ble_evt, 1, 0, &dummyValue, m_ads.adv_tx_power_char_handles.value_handle);
//				
//						// Set smart power mode characteristic.
//						set_characteristic(p_ble_evt, 1, 0, &dummyValue, m_ads.adv_smart_power_char_handles.value_handle);
				
//				    value_adv_mode.len=1;
//						value_adv_mode.offset=0;
//						value_adv_mode.p_value = &adv_mode;
//						sd_ble_gatts_value_set(p_ble_evt->evt.gap_evt.conn_handle, m_ads.adv_mode_char_handles.value_handle, &value_adv_mode);
//					
//						value_adv_interval.len=2;
//						value_adv_interval.offset=0;
//						value_adv_interval.p_value = (uint8_t *)&non_connectable_adv_interval;
//						sd_ble_gatts_value_set(p_ble_evt->evt.gap_evt.conn_handle, m_ads.adv_interval_char_handles.value_handle, &value_adv_interval);
//			
//						value_adv_power.len=1;
//						value_adv_power.offset=0;
//				    //get_tx_power_setting();
//						value_adv_power.p_value = &dummyValue;//&tx_power_setting;
//						sd_ble_gatts_value_set(p_ble_evt->evt.gap_evt.conn_handle, m_ads.adv_tx_power_char_handles.value_handle, &value_adv_power);
//						
//						value_smart_power.len=1;
//						value_smart_power.offset=0;
//						value_smart_power.p_value = &dummyValue;
//						sd_ble_gatts_value_set(p_ble_evt->evt.gap_evt.conn_handle, m_ads.adv_smart_power_char_handles.value_handle, &value_smart_power);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
						advertising_start();
//				    save_settings();
//				    uint32_t err = sd_ble_gap_adv_stop();
//	          APP_ERROR_CHECK(err);
//            update_advertising();
//						is_in_beacon_mode = true;				  
        default:
            break;
    }
}

void set_characteristic( ble_evt_t * p_ble_evt, uint16_t len, uint16_t offset, uint8_t* p_value, uint16_t value_handle)
{
	ble_gatts_value_t value_adv_mode;
	
	value_adv_mode.len = len;
	value_adv_mode.offset = offset;
	value_adv_mode.p_value = p_value;
	sd_ble_gatts_value_set(p_ble_evt->evt.gap_evt.conn_handle, value_handle, &value_adv_mode);
}

/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
	  ble_ads_on_ble_evt(&m_ads, p_ble_evt);
    ble_bas_on_ble_evt(&m_bas, p_ble_evt);
	  ble_ats_on_ble_evt(&m_ats, p_ble_evt);
		
    //ble_conn_params_on_ble_evt(p_ble_evt);
	
    //dm_ble_evt_handler(p_ble_evt);
    //bsp_btn_ble_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
	  
    //ble_advertising_on_ble_evt(p_ble_evt);
}


/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
  //pstorage_sys_event_handler(sys_evt);
	//if(sys_evt == NRF_EVT_POWER_FAILURE_WARNING)
	//{
	//	sd_power_system_off();
	//}
}
																			
/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
		
    uint32_t err_code;
    
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
    
    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);
    
    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);
    
    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);
    
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
	
	  // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
		

    // Register with the SoftDevice handler module for system events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
		
}


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    uint32_t err_code;
    ble_gap_conn_params_t   gap_conn_params; 	// Struct to store GAP connection parameters like max min connection interval etc
    ble_gap_conn_sec_mode_t sec_mode;			// Struct to store security parameters 

    // Set open security.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    // Store the device name and security mode in the SoftDevice.
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code); // Check for errors

    // Always initialize all fields in structs to zero or you might get unexpected behaviour
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    // Populate the GAP connection parameter struct
     gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
     gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
     gap_conn_params.slave_latency     = SLAVE_LATENCY;
     gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    // Set GAP Peripheral Preferred Connection Parameters
    // The device use these prefered values when negotiating connection terms with another device
    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
                                          
    // Set appearence										  
		sd_ble_gap_appearance_set(576);   // 576: Generic keyring 
	  APP_ERROR_CHECK(err_code);// Check for errors                                 
                                          
}

static uint32_t get_temperature_data(void)
{
    int32_t temp;
    uint32_t err_code;
    
    err_code = sd_temp_get(&temp);
    APP_ERROR_CHECK(err_code);
    
		return temp;
}

/**@brief Function for initializing the Advertisement packet in the Eddystone format.
 *
 * @details This function initializes the data that is to be placed in an advertisement packet 
 *
 */
static void advertising_eddystone_init(uint8_t* dataArray, uint16_t size)
{
    uint32_t                   err_code;
    ble_advdata_t              advdata;
    //ble_advdata_manuf_data_t   manuf_data;
		uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
		ble_uuid_t    adv_uuids[] = {{0X0F3E, BLE_UUID_TYPE_BLE}};

		
		// Service data (Eddystone)
		uint8_array_t data_array;
		
		data_array.p_data = dataArray;                 // Pointer to the data to advertise.
    data_array.size = size;
	
	  ble_advdata_service_data_t service_data;            // Structure to hold Service Data.
    service_data.service_uuid = 0xFEAA;                 // Eddystone UUID to allow discoverability on iOS devices.
    service_data.data = data_array;   

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));
		
    advdata.flags                 = flags;
		advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuids;
    advdata.p_service_data_array    = &service_data;                // Pointer to Service Data structure.
    advdata.service_data_count      = 1;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertisement packet in the iBeacon format.
 *
 * @details This function initializes the data that is to be placed in an advertisement packet 
 *
 */
static void advertising_iBeacon_init()
{
    uint32_t                   err_code;
    ble_advdata_t              advdata;
		uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  // non-connectable, undirected advertising, single-mode device

    // Manifacturer specific data
	  ble_advdata_manuf_data_t manuf_specific_data;
    manuf_specific_data.company_identifier = 0x004C;  // Apple company identifier
    manuf_specific_data.data.p_data = (uint8_t *) iBeacon_info;
    manuf_specific_data.data.size   = sizeof(iBeacon_info);
		
    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));
    
	  advdata.name_type             = BLE_ADVDATA_NO_NAME;
    advdata.flags                 = flags;
    advdata.p_manuf_specific_data = &manuf_specific_data;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
static void services_advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
	
    ble_uuid_t adv_uuids[] = {{ADS_UUID_SERVICE, m_ads.uuid_type},
															{BLE_UUID_HEALTH_THERMOMETER_SERVICE, BLE_UUID_TYPE_BLE},
															{BLE_UUID_BATTERY_SERVICE,            BLE_UUID_TYPE_BLE},
                              {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}};
    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = m_adv_uuids;

		err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the connectable advertisement parameters.
 *
 * @details This function initializes the advertisement parameters to values that will put 
 *          the application in connectable mode.
 *
 */
static void connectable_adv_init(void)
{
    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    
    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND ;
    m_adv_params.p_peer_addr = NULL;                               // Undirected advertisement
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = CONNECTABLE_ADV_INTERVAL;
    m_adv_params.timeout     = CONNECTABLE_ADV_TIMEOUT;
	
	  sd_ble_gap_tx_power_set(tx_power_level);
}

/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = true;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the non-connectable advertisement parameters.
 *
 * @details This function initializes the advertisement parameters to values that will put 
 *          the application in non-connectable mode.
 *
 */
static void non_connectable_adv_init(void)
{
    
    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));
    
    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND; //BLE_GAP_ADV_TYPE_ADV_IND ; 
    m_adv_params.p_peer_addr = NULL;                     // Undirected advertisement
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = non_connectable_adv_interval;
    m_adv_params.timeout     = NON_CONNECTABLE_ADV_TIMEOUT;
	
	  sd_ble_gap_tx_power_set(tx_power_level);
}

/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code;
    
    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

//static void temperature_measurement_send(ble_hts_t * p_hts, ble_hts_evt_t * p_evt)
//{
//		//uint32_t temp = temperature_data_get();
//		uint32_t err_code = ble_hts_measurement_send(&m_ats, (ble_hts_meas_t *)&temp);
//}
static void set_iBeacon_payload()
{
	for(uint8_t i = 0; i < 20; i++)
	{
		iBeacon_info[i+2] = beacon_payload[i];
	}
}

static void set_eddystone_uid_payload()
{
	for(uint8_t i = 0; i < 16; i++)
	{
		eddystone_uid_data[i+2] = beacon_payload[i];
	}
}

bool is_extension_byte(uint8_t byte)
{
	return (byte == 0x00 
			|| byte == 0x01 
			|| byte == 0x02 
			|| byte == 0x03 
			|| byte == 0x04
			|| byte == 0x05
			|| byte == 0x06
			|| byte == 0x07
			|| byte == 0x08
			|| byte == 0x09
			|| byte == 0x0a
			|| byte == 0x0b
			|| byte == 0x0c
			|| byte == 0x0d);
}
static void set_eddystone_url_payload()
{
	uint8_t i;
	for(i = 0; i < 17; i++)
	{
		eddystone_url_data[i+3] = beacon_payload[i];
		
		// Find the end of the url.
		if(is_extension_byte(beacon_payload[i]))
			break;
	}
	
	url_data_size = i + 4;
}

static void update_advertising()
{
		//Initialize the right advertising mode
	switch(advertising_mode)
	{
		case 0x00: // iBeacon
				set_iBeacon_payload();
			  non_connectable_adv_init();
				advertising_iBeacon_init();
				break;
		
		case 0x01: // Eddystone UID
			  set_eddystone_uid_payload();
			  non_connectable_adv_init();
	      advertising_eddystone_init(eddystone_uid_data, sizeof(eddystone_uid_data));
				break;
		
		case 0x02: // Eddystone URL
			  set_eddystone_url_payload();
			  non_connectable_adv_init();
				advertising_eddystone_init(eddystone_url_data, url_data_size);
				break;
			
		case 0x03: // Eddystone TLM
			  non_connectable_adv_init();
				advertising_eddystone_init(eddystone_tlm_data, sizeof(eddystone_tlm_data));
				break;
		
		case 0x04: // AltBeacon
			  non_connectable_adv_init();
				// Not implemented yet
				break;
			
		default:
			 connectable_adv_init();
			 services_advertising_init();
		   //is_in_beacon_mode = false;
				break;
	}
	// Start the advertising
	advertising_start();
}

/**@brief Function for handling write events to the advertising mode characteristic.
 *
 *
 */
static void adv_mode_write_handler(ble_ads_t * p_ads, uint8_t* new_adv_mode)
{
	uint32_t err_code;
	advertising_mode = new_adv_mode[0];
	
}

/**@brief Function for handling write events to the advertising interval characteristic.
 *
 *
 */
static void adv_interval_write_handler(ble_ads_t * p_ads, uint8_t* adv_interval)
{
	uint8_t interval = adv_interval[0] * 100;
	non_connectable_adv_interval = MSEC_TO_UNITS(interval, UNIT_0_625_MS);
	
}

/**@brief Function for handling write events to the advertising transmit power characteristic.
 *
 *
 */
static void adv_tx_power_write_handler(ble_ads_t * p_ads, uint8_t* adv_tx_power)
{
	switch(adv_tx_power[0])
	{
		case 0x00: // 4dbm
			tx_power_level = 4;
			break;
		case 0x01: // 0dbm
			tx_power_level = 0;
			break;
		case 0x02: // -4dbm
			tx_power_level = -4;
			break;
		case 0x03: // -8dbm
			tx_power_level = -8;
			break;
		case 0x04: // -12dbm
			tx_power_level = -12;
			break;
		case 0x05: // -16dbm
			tx_power_level = -16;
			break;
		case 0x06: // -20dbm
			tx_power_level = -20;
			break;
		case 0x07: // -40dbm
			tx_power_level = -40;
			break;
		default:
			break;
	}
}

/**@brief Function for handling write events to the advertising smart power characteristic.
 *
 *
 */
static void adv_smart_power_write_handler(ble_ads_t * p_ads, uint8_t* adv_smart_power)
{
	if(adv_smart_power[0] == 1 && !smart_power_mode_enabled)
	{
		timers_start();
		smart_power_mode_enabled = true;
	}
	else if(smart_power_mode_enabled)
	{
		smart_power_mode_enabled = false;
		app_timer_stop(timer_id);
	}
	
}


static void adv_payload_write_handler(ble_ads_t * p_ads, uint8_t* payload)
{
	// Reset every entry of the array
	memset(beacon_payload, 0x00, sizeof(beacon_payload));
	
	for(int i = 0; i < 20; i++)
	{
		beacon_payload[i] = payload[i];
	}
	

// Disconnect the central
	//uint32_t err_code = sd_ble_gap_disconnect(m_conn_handle, 0x13);
	//APP_ERROR_CHECK(err_code);
	//is_in_beacon_mode = true;
	//save_settings();
	//update_advertising();
	
}
 /**@brief Event handler for the ambient temperature service
 *
 *
 */
void on_temperature_send()
{
	
}


 /**@brief Function for initializing services that will be used by the application.
 *
 *
 */
static void services_init(void)
{
    uint32_t         err_code;
	  ble_ats_init_t   ats_init;
    ble_bas_init_t   bas_init;
	  ble_dis_init_t   dis_init;
    ble_dis_sys_id_t sys_id;
	
		ble_ads_init_t ads_init;
	  ads_init.adv_mode_write_handler = adv_mode_write_handler;
	  ads_init.adv_interval_write_handler = adv_interval_write_handler;
	  ads_init.adv_tx_power_write_handler = adv_tx_power_write_handler;
	  ads_init.adv_smart_power_write_handler = adv_smart_power_write_handler;
		ads_init.adv_payload_write_handler = adv_payload_write_handler;
		err_code = ble_ads_init(&m_ads, &ads_init);
    APP_ERROR_CHECK(err_code);
	
	
	  // Initialize Health Thermometer Service
    memset(&ats_init, 0, sizeof(ats_init));

    ats_init.evt_handler                 = on_temperature_send;
    ats_init.temp_type_as_characteristic = 0;            // Temperature type as a field of measurement
    ats_init.temp_type                   = BLE_HTS_TEMP_TYPE_BODY;
	
		// Here the sec level for the Thermometer Service can be changed/increased.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&ats_init.ats_meas_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&ats_init.ats_meas_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&ats_init.ats_meas_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&ats_init.ats_temp_type_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&ats_init.ats_temp_type_attr_md.write_perm);

    err_code = ble_ats_init(&m_ats, &ats_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Battery Service.
    memset(&bas_init, 0, sizeof(bas_init));

    // Here the sec level for the Battery Service can be changed/increased.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_report_read_perm);

    bas_init.evt_handler          = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref         = NULL;
    bas_init.initial_batt_level   = 0;

    err_code = ble_bas_init(&m_bas, &bas_init);
    APP_ERROR_CHECK(err_code);
	
	  // Initialize Device Information Service.
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, MANUFACTURER_NAME);
    ble_srv_ascii_to_utf8(&dis_init.model_num_str,     "0001");  // MODEL_NUM

    sys_id.manufacturer_id            = 0x1122334455;   // MANUFACTURER_ID;
    sys_id.organizationally_unique_id = 0x667788;       // ORG_UNIQUE_ID;
    dis_init.p_sys_id                 = &sys_id;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err_code = ble_dis_init(&dis_init);
    APP_ERROR_CHECK(err_code);
}



 /**@brief Function that set advertising intervals.
  *
  * @details Set advertising intervals from v_solar & v_bat
  */
void set_advertising_interval(uint16_t vSolar, uint16_t vBat)
{
	uint16_t interval_ms;
	if(vSolar < 50)       // 0 to 500 Lux
		interval_ms = 10000;
	else if(vSolar < 80)  // 500 to 2000 Lux
		interval_ms = 5000;
	else if(vSolar < 95)  // 2000 to 4000 Lux
		interval_ms = 3000;
	else if(vSolar < 120)                  
		interval_ms = 1000;
	else 
		interval_ms = 500;

	if(vBat < 770) // Battery have less than 2.75 volt
		interval_ms = 10000;
	
	non_connectable_adv_interval = MSEC_TO_UNITS(interval_ms, UNIT_0_625_MS); // Interval units are 0.625ms
}


/**@brief Function called everytime the timer is done to read sensors
*
* @details Start data sampling for v_bat & v_solar and update advertising 
*/
void sensor_timer_handler(void * p_context)
{
    // Get data from saadc
		saadc_init();	
		start_sampling();
		
		// Once every SAMPLES_NB, update the advertising settings & data
	  if(cpt == SAMPLES_NB)// && is_in_beacon_mode)// && smart_power_mode_enabled) 
		{
			// Get v_solar and v_bat
			v_solar = saadc_get_data_ch_0()/(SAMPLES_NB/2); // Divide by half of the total nb of samples because there is two channels.
			v_bat = saadc_get_data_ch_1()/(SAMPLES_NB/2);
			
			// Set v_solar and v_bat to zero
			saadc_reset_data();
			
//			if(is_in_beacon_mode)
//			{
//				uint32_t err = sd_ble_gap_adv_stop();
//				APP_ERROR_CHECK(err);
//				
//				// Update advertising intervals 
//				//set_advertising_interval(v_solar, v_bat);
//				
//				// Update data in the advertising packet
//				iBeacon_info[2] = v_bat & 0xff;
//				iBeacon_info[3] = v_bat >> 8;
//				iBeacon_info[4] = v_solar & 0xff;
//				iBeacon_info[5] = v_solar >> 8;

//				update_advertising();
//			}
			
			cpt = 0;
		}
		
		cpt ++;

		if(m_conn_handle != BLE_CONN_HANDLE_INVALID) // If we are connected.
		{
			uint8_t batteryPercent = ((v_bat - 16.626)/274.25)/3.2 * 100;
			
			// update battery level service
			uint32_t err_code = ble_bas_battery_level_update(&m_bas, batteryPercent);
			if ((err_code != NRF_SUCCESS) 
					&& (err_code != NRF_ERROR_INVALID_STATE) 
					&& (err_code != BLE_ERROR_NO_TX_PACKETS) 
					&& (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
			{
					APP_ERROR_HANDLER(err_code);
			}
			
			// Update the temperature gatt characteristic
			uint32_t temperature = get_temperature_data();
		  ble_ats_temperature_update(&m_ats, temperature);
			
			// Update the vsolar gatt characteristic
			ble_ats_vsolar_update(&m_ats, v_solar);
			
			
		}	
}


/**@brief Function for the Timer initialization.
*
* @details Initializes the timer module. This creates the application timers.
*/
void timers_init(void)
{
	uint32_t err_code;
	
  // Initialize the application timer module.
  APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
	
	// Create timer
  err_code = app_timer_create(&timer_id, APP_TIMER_MODE_REPEATED, sensor_timer_handler);
}

/**@brief Function for the Timer initialization.
*
* @details This start the application timers.
*/
void timers_start(void)
{
	uint32_t err_code;
	
	// Start timer
	err_code = app_timer_start(timer_id, APP_TIMER_TICKS(1024, APP_TIMER_PRESCALER), NULL); // 500 ms
  APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function called when the button is pressed
 *
 * @details This configure the advertising as connectable
 */
void button_interrupt()
{
	
	//sd_power_system_off();
	if(test)
	{
		uint32_t err = sd_ble_gap_adv_stop();
	 APP_ERROR_CHECK(err);
		test = false;
	}
	else
	{
		advertising_start();
		test = true;
	}
	 
	
//	 if(is_in_beacon_mode)
//	 {
//		 save_settings();
//		 
//		 if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
//		 {
//			 uint32_t err_code = sd_ble_gap_disconnect(m_conn_handle, 0x13);
//			 APP_ERROR_CHECK(err_code);
//		 }
//		 else
//		 {
//			 uint32_t err = sd_ble_gap_adv_stop();
//			 APP_ERROR_CHECK(err);
//		 }
//		 
//		 update_advertising();
//		 is_in_beacon_mode = false;
//	 }
//	 else
//	 {
//		 
//		 if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
//		 {
//			 uint32_t err_code = sd_ble_gap_disconnect(m_conn_handle, 0x13);
//			 APP_ERROR_CHECK(err_code);
//		 }
//		 else
//		 {
//			 uint32_t err = sd_ble_gap_adv_stop();
//			 APP_ERROR_CHECK(err);
//		 }
//		 
//		 connectable_adv_init();
//		 services_advertising_init();
//		 advertising_start();
//		 is_in_beacon_mode = true;
//	 }
	 
}

void accelerometer_interrupt()
{
	// Update the accelerometer_data gatt characteristic
		u8 accel_data[6];
		bma280_read_accel_xyz(accel_data);
		ble_ats_accelerometer_data_update(&m_ats, accel_data);
}

/**
 * @brief Function called when vBat ok go from Hi to Lo
 *
 */
void on_power_low()
{
	// Set the chip to system off (it can be waken up by gpios)
	if(nrf_gpio_pin_read(28) == 0)
	{
		sd_power_system_off();
	}
		
}

/**
 * @brief Function called when vBat ok go from Lo to Hi
 *
 */
void on_power_restored()
{
	// Nothing here
}

/**
 * @brief Function for configuring interrupt on Button 1
 *
 */
static void gpio_init(void)
{
    ret_code_t err_code;
		
		// Init gpiote drivers
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
		
		// Config from hi to lo
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false); 
    in_config.pull = NRF_GPIO_PIN_PULLUP;
		//in_config.hi_accuracy = false;
	
		// Set event on Button
    err_code = nrf_drv_gpiote_in_init(BSP_BUTTON_0, &in_config, button_interrupt);
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_event_enable(BSP_BUTTON_0, true);
	
//		// Set event on vBat_ok
//		err_code = nrf_drv_gpiote_in_init(28, &in_config, on_power_low);
//    APP_ERROR_CHECK(err_code);
//		nrf_drv_gpiote_in_event_enable(28, true);
	
	  // Config from hi to lo
    //nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false); 
    //in_config.pull = NRF_GPIO_PIN_PULLUP;
		//in_config.hi_accuracy = false;
		
		 // Set event on vBat_ok
		 //err_code = nrf_drv_gpiote_in_init(28, &config, on_power_low);
     //APP_ERROR_CHECK(err_code);
		 //nrf_drv_gpiote_in_event_enable(28, true);
		 
		 
		 // Need this to wake up when vBat_ok goes from low to hi
		 //nrf_gpio_cfg_sense_input(28, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_HIGH);
		
		 //NRF_GPIO->PIN_CNF[28] |= (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);
	
//		err_code = nrf_drv_gpiote_in_init(12, &in_config, accelerometer_interrupt);
//    APP_ERROR_CHECK(err_code);
//		nrf_drv_gpiote_in_event_enable(12, true);
//	err_code = nrf_drv_gpiote_in_init(11, &in_config, accelerometer_interrupt);
//    APP_ERROR_CHECK(err_code);
//		nrf_drv_gpiote_in_event_enable(11, true);
}


/**
 * @brief Function that write settings to flash
 *
 */
static void save_settings()
{

	uint32_t err_code;
	// Clear blocks.
	err_code = pstorage_clear(&block_handle_1, 48);
	err_code = pstorage_clear(&block_handle_2, 48);
	err_code = pstorage_clear(&block_handle_3, 48);
	
	// Fill the data to save.
	source_data_1[0] = advertising_mode;
	source_data_1[1] = non_connectable_adv_interval;
	source_data_1[2] = tx_power_level;
	source_data_1[3] = smart_power_mode_enabled;
	
	// Load beacon_payload in block data 2 & 3
	for(uint8_t i = 0; i < 16; i++)
	{
		source_data_2[i] = beacon_payload[i];
	}
	for(uint8_t i = 16; i < 23; i++)
	{
		source_data_3[i-16] = beacon_payload[i];
	}
		
	// Store data.
	err_code = pstorage_store(&block_handle_1, source_data_1, 16, 0);
	err_code = pstorage_store(&block_handle_2, source_data_2, 16, 0);
	err_code = pstorage_store(&block_handle_3, source_data_3, 16, 0);
}

/**
 * @brief Function that read settings from flash
 *
 */
static void load_settings()
{
	// Get the saved data.
	uint32_t err_code;
  err_code = pstorage_load(destination_data_1, &block_handle_1, 16, 0);
	err_code = pstorage_load(destination_data_2, &block_handle_2, 16, 0);
	err_code = pstorage_load(destination_data_3, &block_handle_3, 16, 0);
	
	// Set the saved data to the right variables.
	advertising_mode = destination_data_1[0];
	//non_connectable_adv_interval = destination_data[1];
	tx_power_level = destination_data_1[2];
	smart_power_mode_enabled = destination_data_1[3];
	
	// Load data from block 2 & 3 in beacon_payload.
	for(uint8_t i = 0; i < 16; i++)
	{
		beacon_payload[i] = destination_data_2[i];
	}
	for(uint8_t i = 16; i < 23; i++)
	{
		beacon_payload[i] = destination_data_3[i-16];
	}
}

static void storage_cb_handler(pstorage_handle_t  * handle, uint8_t op_code, uint32_t result, uint8_t * p_data, uint32_t data_len)
{
	// Nothing to do here.
	//if(handle->block_id == pstorage_wait_handle) { pstorage_wait_flag = 0; }  //If we are waiting for this callback, clear the wait flag.
}
		
/**
 * @brief Function that initialize and configure the pstorage module
 *
 */
static void storage_init()
{
	uint32_t err_code;
	pstorage_handle_t       handle;
	
	pstorage_module_param_t param;
	
	param.block_size  = 16;        //Select block size of 16 bytes
	param.block_count = 4;        //Select 4 blocks, total of 64 bytes
	param.cb          = storage_cb_handler;    //Set the pstorage callback handler
	
	err_code = pstorage_init();
	
	// Apply parameters.
	err_code = pstorage_register(&param, &handle);
	
	// Get block handles.
	err_code = pstorage_block_identifier_get(&handle, 0, &block_handle_1);
	err_code = pstorage_block_identifier_get(&handle, 1, &block_handle_2);
	err_code = pstorage_block_identifier_get(&handle, 2, &block_handle_3);
}

/**@brief Application entry
*/
int main(void)
{
	uint32_t err_code;
	gpio_init();
	bma280_init();
	twi_reset();
	
	// Initialize.
	ble_stack_init();
  gap_params_init();
  sd_power_dcdc_mode_set(1);
	
	storage_init();
	timers_init();
	
	load_settings();
	
  services_init();
	
	//if(smart_power_mode_enabled)
	timers_start();
	
	update_advertising();
	

	
	
	for(;;)
	{
		// Enter sleep mode.
		power_manage();
	}
	
}


