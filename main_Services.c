/////////////////////////////////////////////////////////////////////////////////
///// @file main.c
///// @author Olivier Tessier-Lariviere
///// @date 2016-05-10
///// @version 1.0
/////
///// This class advertise ... 
/////
/////////////////////////////////////////////////////////////////////////////////

//#include <stdint.h>
//#include "ble_advdata.h"
//#include "nordic_common.h"
//#include "softdevice_handler.h"
//#include "bsp.h"
//#include "app_timer.h"
//#include "nrf_temp.h"
//#include "ble_bas.h"

//#include "nrf.h"
//#include "app_error.h"
//#include "ble.h"
//#include "ble_hci.h" 
//#include "ble_srv_common.h"


//#define CENTRAL_LINK_COUNT          0                                 // Number of central links used by the application. When changing this number remember to adjust the RAM settings.
//#define PERIPHERAL_LINK_COUNT       0                                 // Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings

//#define NON_CONNECTABLE_ADV_TIMEOUT     0                                 // Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. 
//#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(100, UNIT_0_625_MS) // The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). 
//#define CONNECTABLE_ADV_INTERVAL      MSEC_TO_UNITS(20, UNIT_0_625_MS)              /**< The advertising interval for connectable advertisement (20 ms). This value can vary between 20ms to 10.24s. */
//#define CONNECTABLE_ADV_TIMEOUT       30                                            /**< Time for which the device must be advertising in connectable mode (in seconds). */

//#define TX_POWER_LEVEL									4                                 // the transmit power level can vary from -40dbm to 4dbm

//#define APP_TIMER_PRESCALER             0                                 /**< Value of the RTC1 PRESCALER register. */
//#define APP_TIMER_OP_QUEUE_SIZE         4                                 /**< Size of timer operation queues. */

//#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.1 seconds). */
//#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.2 second). */
//#define SLAVE_LATENCY                    0                                          /**< Slave latency. */
//#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

//#define APP_COMPANY_IDENTIFIER       0x0000     // * Wibicom identifier coming soon *

//#define DEVICE_NAME  "WIBEACON"


////static ble_bas_t  m_bas;                              // Structure used to identify the battery service. 
//typedef uint8_t 	nrf_power_mode_t;
//int32_t volatile temp;
//int8_t volatile cpt = 0;

//static ble_gap_adv_params_t     m_adv_params;                                       /**< Parameters to be passed to the stack when starting advertising. */
//static uint8_t                  m_char_value[20];                     /**< Value of the characteristic that will be sent as a notification to the central. */
////static uint8_t                  m_addl_adv_manuf_data[ADV_ADDL_MANUF_DATA_LEN];     /**< Value of the additional manufacturer specific data that will be placed in air (initialized to all zeros). */
//static ble_gatts_char_handles_t m_char_handles;                                     /**< Handles of local characteristic (as provided by the BLE stack).*/
//static uint16_t                 m_conn_handle = BLE_CONN_HANDLE_INVALID;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
//static uint16_t                 m_service_handle;                                   /**< Handle of local service (as provided by the BLE stack).*/
//static bool                     m_is_notifying_enabled = false;                     /**< Variable to indicate whether the notification is enabled by the peer.*/


//static ble_gap_adv_params_t m_adv_params;            // Parameters to be passed to the stack when starting advertising.

//static ble_uuid_t m_adv_uuids[] = {{BLE_UUID_GLUCOSE_SERVICE,            BLE_UUID_TYPE_BLE},
//                                   {BLE_UUID_BATTERY_SERVICE,            BLE_UUID_TYPE_BLE},
//                                   {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}}; /**< Universally unique service identifiers. */


///**@brief 128-bit UUID base List. */
//static const ble_uuid128_t m_base_uuid128 =
//{
//   {
//       0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
//       0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
//   }
//};

//static uint8_t adv_data[] =                    // Information advertised by the Beacon.
//{
//		0x02, 0x15, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
//};

//static uint8_t iBeacon_info[] =                    // Information advertised by the Beacon.
//{
//		0x02, 0x15, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x01, 0x00, 0x02, 0x01
//};

//static uint8_t eddystone_uid_data[] =   /**< Information advertised by the Eddystone UID frame type. */
//{
//    0x00,                                                          // Eddystone UID frame type.
//    0xEE,                                                          // RSSI value at 0 m.
//    0xAA, 0xAA, 0xBB, 0xBB, 0xCC, 0xCC, 0xDD, 0xDD, 0xEE, 0xEE,    // 10-byte namespace value. Similar to Beacon Major.
//    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,                            // 6-byte ID value. Similar to Beacon Minor.
//    0x00, 0x00                                                     // Reserved for future use.
//};

//static uint8_t eddystone_tlm_data[] =   /**< Information advertised by the Eddystone TLM frame type. */
//{
//   0x20,                      // Eddystone TLM frame type.
//   0x00,                      // Eddystone TLM version.
//   0x00, 0xF0,                // Battery voltage in mV/bit.
//   0xF0, 0x00,                // Temperature [C].
//   0x00, 0x00, 0x00, 0x00,    // Number of advertisements since power-up or reboot.
//   0x00, 0x00, 0x00, 0x00     // Time since power-up or reboot. 0.1 s increments.
//};

//static uint8_t eddystone_url_data[] =   /**< Information advertised by the Eddystone URL frame type. */
//{
//    0x10,                     // Eddystone URL frame type.
//    0xEE,                     // RSSI value at 0 m.
//    0x00,                     // Scheme or prefix for URL ("http", "http://www", etc.)
//    'w', 'i', 'b', 'i', 'c', 'o', 'm', 0x00     // URL with a maximum length of 17 bytes. Last byte is suffix (".com", ".org", etc.)
//};





///**@brief Function for the GAP initialization.
// *
// * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
// *          device including the device name, appearance, and the preferred connection parameters.
// */
//static void gap_params_init(void)
//{
//    uint32_t err_code;
//    ble_gap_conn_params_t   gap_conn_params; 	// Struct to store GAP connection parameters like max min connection interval etc
//    ble_gap_conn_sec_mode_t sec_mode;			// Struct to store security parameters 

//    // Set open security.
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

//    // Store the device name and security mode in the SoftDevice.
//    err_code = sd_ble_gap_device_name_set(&sec_mode,
//                                          (const uint8_t *)DEVICE_NAME,
//                                          strlen(DEVICE_NAME));
//    APP_ERROR_CHECK(err_code); // Check for errors

//    // Always initialize all fields in structs to zero or you might get unexpected behaviour
//    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

//    // Populate the GAP connection parameter struct
//     gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
//     gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
//     gap_conn_params.slave_latency     = SLAVE_LATENCY;
//     gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

//    // Set GAP Peripheral Preferred Connection Parameters
//    // The device use these prefered values when negotiating connection terms with another device
//    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
//    APP_ERROR_CHECK(err_code);
//                                          
//    // Set appearence										  
//		//sd_ble_gap_appearance_set(576);   // 576: Generic keyring 
//	  //APP_ERROR_CHECK(err_code);        // Check for errors                                                               
//}

///**@brief Function for initializing the connectable advertisement parameters.
// *
// * @details This function initializes the advertisement parameters to values that will put 
// *          the application in connectable mode.
// *
// */
//static void connectable_adv_init(void)
//{
//    // Initialize advertising parameters (used when starting advertising).
//    memset(&m_adv_params, 0, sizeof(m_adv_params));
//    
//    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND ;
//    m_adv_params.p_peer_addr = NULL;                               // Undirected advertisement
//    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
//    m_adv_params.interval    = CONNECTABLE_ADV_INTERVAL;
//    m_adv_params.timeout     = CONNECTABLE_ADV_TIMEOUT;
//}

///**@brief Function for initializing the non-connectable advertisement parameters.
// *
// * @details This function initializes the advertisement parameters to values that will put 
// *          the application in non-connectable mode.
// *
// */
//static void non_connectable_adv_init(void)
//{
//    
//    // Initialize advertising parameters (used when starting advertising).
//    memset(&m_adv_params, 0, sizeof(m_adv_params));
//    
//    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND; //BLE_GAP_ADV_TYPE_ADV_IND ; 
//    m_adv_params.p_peer_addr = NULL;                     // Undirected advertisement
//    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
//    m_adv_params.interval    = NON_CONNECTABLE_ADV_INTERVAL;
//    m_adv_params.timeout     = NON_CONNECTABLE_ADV_TIMEOUT;
//}













///**@brief Function for initializing the Advertisement packet in the iBeacon format.
// *
// * @details This function initializes the data that is to be placed in an advertisement packet 
// *
// */
//static void advertising_iBeacon_init()
//{
//    uint32_t                   err_code;
//    ble_advdata_t              advdata;
//		uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;  // non-connectable, undirected advertising, single-mode device

//    // Manifacturer specific data
//	  ble_advdata_manuf_data_t manuf_specific_data;
//    manuf_specific_data.company_identifier = 0x004C;  // Apple company identifier
//    manuf_specific_data.data.p_data = (uint8_t *) iBeacon_info;
//    manuf_specific_data.data.size   = sizeof(iBeacon_info);
//		
//    // Build and set advertising data.
//    memset(&advdata, 0, sizeof(advdata));
//    
//	  advdata.name_type             = BLE_ADVDATA_NO_NAME;
//    advdata.flags                 = flags;
//    advdata.p_manuf_specific_data = &manuf_specific_data;

//    err_code = ble_advdata_set(&advdata, NULL);
//    APP_ERROR_CHECK(err_code);
//	
//	  // Initialize advertising parameters (used when starting advertising).
//    //non_connectable_adv_init();
//}

///**@brief Function for initializing the Advertisement packet.
// *
// * @details This function initializes the data that is to be placed in an advertisement packet in 
// *          both connectable and non-connectable modes.
// *
// */
//static void advertising_data_init(void)
//{
//    uint32_t                   err_code;
//    ble_advdata_t              advdata;
//    ble_advdata_manuf_data_t   manuf_data;
//    uint8_t                    flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;//BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;


//    // Build and set advertising data
//    memset(&advdata, 0, sizeof(advdata));

//    manuf_data.company_identifier = 0x00; // Company identifier coming soon. 
//    manuf_data.data.size          = sizeof(adv_data);
//    manuf_data.data.p_data        = (uint8_t *)adv_data;
//		
//	  advdata.name_type             = BLE_ADVDATA_NO_NAME;
//    advdata.flags                 = flags;
//    advdata.p_manuf_specific_data = &manuf_data;

//    err_code = ble_advdata_set(&advdata, NULL);
//    APP_ERROR_CHECK(err_code);
//}

///**@brief Function for initializing the Advertising functionality.
// *
// * @details Encodes the required advertising data and passes it to the stack.
// *          Also builds a structure to be passed to the stack when starting advertising.
// */
//static void advertising_init(void)
//{
//    uint32_t      err_code;
//    ble_advdata_t advdata;

//    // Build and set advertising data.
//    memset(&advdata, 0, sizeof(advdata));

//    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
//    advdata.include_appearance      = true;
//    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;;
//    advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
//    advdata.uuids_complete.p_uuids  = m_adv_uuids;

////    ble_adv_modes_config_t options = {0};
////    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
////    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
////    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

//    err_code = ble_advdata_set(&advdata, NULL);
//    APP_ERROR_CHECK(err_code);
//}

///**@brief Function for starting advertising.
// */
//static void advertising_start(void)
//{
//    uint32_t err_code;
//    
//    err_code = sd_ble_gap_adv_start(&m_adv_params);
//    APP_ERROR_CHECK(err_code);
//}

///**@brief Function for doing power management.
// */
//static void power_manage(void)
//{
//    uint32_t err_code = sd_app_evt_wait();
//    APP_ERROR_CHECK(err_code);
//}

///**@brief Function that read the temperature sensor.
// */
////static void read_temperature(void)
////{
////    NRF_TEMP->TASKS_START = 1;
////    while (NRF_TEMP->EVENTS_DATARDY == 0){}; // wait while the sensor measurment is not finished
////    NRF_TEMP->EVENTS_DATARDY = 0;
////    temp = (nrf_temp_read() / 4); // measured units are 0.25 celsius
////    NRF_TEMP->TASKS_STOP = 1;
////}

///**@brief Function for performing battery measurement and updating the Battery Level characteristic
// *        in Battery Service.
// */
//// static void battery_level_update(void)
//// {
////     uint32_t err_code;
////     uint8_t  battery_level;

////     battery_level = (uint8_t)sensorsim_measure(&m_battery_sim_state, &m_battery_sim_cfg);

////     err_code = ble_bas_battery_level_update(&m_bas, battery_level);
////     if ((err_code != NRF_SUCCESS) &&
////         (err_code != NRF_ERROR_INVALID_STATE) &&
////         (err_code != BLE_ERROR_NO_TX_PACKETS) &&
////         (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
////     )
////     {
////         APP_ERROR_HANDLER(err_code);
////     }
//// }

//// /**@brief Function for handling the Battery measurement timer timeout.
////  *
////  * @details This function will be called each time the battery level measurement timer expires.
////  *
////  * @param[in]   p_context   Pointer used for passing some arbitrary information (context) from the
////  *                          app_start_timer() call to the timeout handler.
////  */
//// static void battery_level_meas_timeout_handler(void * p_context)
//// {
////     UNUSED_PARAMETER(p_context);
////     battery_level_update();
//// }





//// /**@brief Function that check for button press
////  *
////  * @details Change advertising type.
////  */
//void button_check(void)
//{
//  bool is_button1_pressed; // Eddystone UID
//	bool is_button2_pressed; // Eddystone URL
//	bool is_button3_pressed; // Eddystone TLM
//  //bool is_button4_pressed; // iBeacon
//	
//	uint32_t err_code;
//  // Initialize buttons
//	err_code = bsp_init(BSP_INIT_BUTTONS, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
//  APP_ERROR_CHECK(err_code);
//	
//  // Button 1
//	err_code = bsp_button_is_pressed(0, &is_button1_pressed);
//  APP_ERROR_CHECK(err_code);
//	
//  // Button 2
//	err_code = bsp_button_is_pressed(1, &is_button2_pressed);
//  APP_ERROR_CHECK(err_code);
//	
//  // Button 3
//	err_code = bsp_button_is_pressed(2, &is_button3_pressed);
//  APP_ERROR_CHECK(err_code);
//	
//  // Button 4
//  // err_code = bsp_button_is_pressed(3, &is_button4_pressed);
//  // APP_ERROR_CHECK(err_code);
//  
//  if(is_button1_pressed)
//	{
//		advertising_Eddystone_init((uint8_t *)eddystone_uid_data, sizeof(eddystone_uid_data));
//	}
//	else if(is_button2_pressed)
//	{
//		advertising_Eddystone_init((uint8_t *)eddystone_url_data, sizeof(eddystone_url_data));
//	}
//	else if(is_button3_pressed)
//	{
//		advertising_Eddystone_init((uint8_t *)eddystone_tlm_data, sizeof(eddystone_tlm_data));
//	}
////  else if(is_button4_pressed)
////	{
////		advertising_iBeacon_init();
////	}
//	else
//	{
//		 err_code = sd_power_system_off();  
//     APP_ERROR_CHECK(err_code);
//	}
//}


//int main(void)
//{
//	//uint32_t err_code;
//	
//	// Initialize.
//	//ble_stack_init();
//  //gap_params_init();
//  //timers_init();
//  
//  //bool is_connectable_mode;
//  
//  // Button 4
//  //err_code = bsp_button_is_pressed(3, &is_connectable_mode);
//  //APP_ERROR_CHECK(err_code);
//  
//  
//  //if (is_connectable_mode)
//  //{
//      // Connectable button is pressed. Start connectable advertisement.
//      //connectable_adv_init();
//		
//    //service_add();

//		//advertising_iBeacon_init();
//  //}
//  
//  
//  //button_check();
//		//connectable_adv_init();
//	  //service_add();
//		//advertising_data_init();
//	  //advertising_iBeacon_init();
//	
//	
//	

//	
//	
//	// Start advertising.
//	//advertising_start();
//	
//	
//	
//	
//	
//	ble_stack_init();
//  gap_params_init();
//	connectable_adv_init();
//	advertising_data_init();
//	
//	
//	advertising_start();
//	
//	for(;;)
//	{
//		
//    	power_manage();
////		do{
////       __WFE();
////    }while (!NRF_RADIO->POWER);     
////    sd_power_dcdc_mode_set(1);       
////		
////    do{
////       __WFE();
////    }while (NRF_RADIO->POWER);      
////    sd_power_dcdc_mode_set(0);         
////		sd_power_mode_set(NRF_POWER_MODE_LOWPWR); 
//		
//  //  read_temperature();
//	//	advertising_temp_update();
//		
//	}
//	
//}


/////////////////////////////////////////////////////////////////////////////////
///// @file main.c
///// @author Olivier Tessier-Lariviere
///// @date 2016-05-10
///// @version 1.0
/////
///// This class advertise ... 
/////
/////////////////////////////////////////////////////////////////////////////////

//#include <stdint.h>
//#include "ble_advdata.h"
//#include "nordic_common.h"
//#include "softdevice_handler.h"
//#include "bsp.h"
//#include "app_timer.h"
//#include "nrf_temp.h"
//#include "ble_bas.h"

//#include "nrf.h"
//#include "app_error.h"
//#include "ble.h"
//#include "ble_hci.h" 
//#include "ble_srv_common.h"


//#define CENTRAL_LINK_COUNT          0                                 // Number of central links used by the application. When changing this number remember to adjust the RAM settings.
//#define PERIPHERAL_LINK_COUNT       0                                 // Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings

//#define NON_CONNECTABLE_ADV_TIMEOUT     0                                 // Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. 
//#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(100, UNIT_0_625_MS) // The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). 
//#define CONNECTABLE_ADV_INTERVAL      MSEC_TO_UNITS(20, UNIT_0_625_MS)              /**< The advertising interval for connectable advertisement (20 ms). This value can vary between 20ms to 10.24s. */
//#define CONNECTABLE_ADV_TIMEOUT       30                                            /**< Time for which the device must be advertising in connectable mode (in seconds). */

//#define TX_POWER_LEVEL									4                                 // the transmit power level can vary from -40dbm to 4dbm

//#define APP_TIMER_PRESCALER             0                                 /**< Value of the RTC1 PRESCALER register. */
//#define APP_TIMER_OP_QUEUE_SIZE         4                                 /**< Size of timer operation queues. */

//#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.1 seconds). */
//#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.2 second). */
//#define SLAVE_LATENCY                    0                                          /**< Slave latency. */
//#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

//#define APP_COMPANY_IDENTIFIER       0x0000     // * Wibicom identifier coming soon *

//#define DEVICE_NAME  "WIBEACON"


////static ble_bas_t  m_bas;                              // Structure used to identify the battery service. 
//typedef uint8_t 	nrf_power_mode_t;
//int32_t volatile temp;
//int8_t volatile cpt = 0;

//static ble_gap_adv_params_t     m_adv_params;                                       /**< Parameters to be passed to the stack when starting advertising. */
//static uint8_t                  m_char_value[20];                     /**< Value of the characteristic that will be sent as a notification to the central. */
////static uint8_t                  m_addl_adv_manuf_data[ADV_ADDL_MANUF_DATA_LEN];     /**< Value of the additional manufacturer specific data that will be placed in air (initialized to all zeros). */
//static ble_gatts_char_handles_t m_char_handles;                                     /**< Handles of local characteristic (as provided by the BLE stack).*/
//static uint16_t                 m_conn_handle = BLE_CONN_HANDLE_INVALID;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
//static uint16_t                 m_service_handle;                                   /**< Handle of local service (as provided by the BLE stack).*/
//static bool                     m_is_notifying_enabled = false;                     /**< Variable to indicate whether the notification is enabled by the peer.*/


//static ble_gap_adv_params_t m_adv_params;            // Parameters to be passed to the stack when starting advertising.

//static ble_uuid_t m_adv_uuids[] = {{BLE_UUID_GLUCOSE_SERVICE,            BLE_UUID_TYPE_BLE},
//                                   {BLE_UUID_BATTERY_SERVICE,            BLE_UUID_TYPE_BLE},
//                                   {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}}; /**< Universally unique service identifiers. */


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
//#include "ble_bas.h"
//#include "ble_dis.h"

#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_hci.h" 
#include "ble_srv_common.h"


//#include <stdint.h>
//#include "ble_advdata.h"
//#include "nordic_common.h"
//#include "softdevice_handler.h"
//#include "bsp.h"
//#include "app_timer.h"
//#include "nrf_temp.h"
//#include <ble_bas.h>

//#include "ble_dis.h"


//#include "nrf.h"
//#include "app_error.h"
//#include "ble.h"
//#include "ble_hci.h" 
//#include "ble_srv_common.h"


//#include <stdint.h>
//#include <string.h>
//#include "nordic_common.h"
//#include "nrf.h"
//#include "app_error.h"
//#include "ble.h"
//#include "ble_hci.h"
//#include "ble_srv_common.h"
//#include "ble_advdata.h"
//#include "ble_advertising.h"
//#include "ble_bas.h"
//#include "ble_dis.h"
//#ifdef BLE_DFU_APP_SUPPORT
//#include "ble_dfu.h"
//#include "dfu_app_handler.h"
//#endif // BLE_DFU_APP_SUPPORT
//#include "ble_conn_params.h"
//#include "boards.h"
//#include "softdevice_handler.h"
//#include "app_timer.h"
//#include "device_manager.h"
////#include "pstorage.h"
////#include "app_trace.h"
////#include "bsp.h"
////#include "nrf_delay.h"
//#include "bsp_btn_ble.h"


#define CENTRAL_LINK_COUNT          0                                 // Number of central links used by the application. When changing this number remember to adjust the RAM settings.
#define PERIPHERAL_LINK_COUNT       1                                 // Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings

#define NON_CONNECTABLE_ADV_TIMEOUT     0                                 // Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. 
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(100, UNIT_0_625_MS) // The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). 
#define CONNECTABLE_ADV_INTERVAL      MSEC_TO_UNITS(20, UNIT_0_625_MS)              /**< The advertising interval for connectable advertisement (20 ms). This value can vary between 20ms to 10.24s. */
#define CONNECTABLE_ADV_TIMEOUT       0                                            /**< Time for which the device must be advertising in connectable mode (in seconds). */

#define TX_POWER_LEVEL									4                                 // the transmit power level can vary from -40dbm to 4dbm

#define APP_TIMER_PRESCALER             0                                 /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                 /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                    0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

#define APP_COMPANY_IDENTIFIER       0x0000     // * Wibicom identifier coming soon *

#define DEVICE_NAME  "WIBEACON"


/**@brief 128-bit UUID base List. */
static const ble_uuid128_t m_base_uuid128 =
{
   {
       0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
       0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
   }
};

static uint8_t adv_data[] =                    // Information advertised by the Beacon.
{
		0x02, 0x15, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA
};

static uint8_t iBeacon_info[] =                    // Information advertised by the Beacon.
{
		0x02, 0x15, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x01, 0x00, 0x02, 0x01
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
    'w', 'i', 'b', 'i', 'c', 'o', 'm', 0x00     // URL with a maximum length of 17 bytes. Last byte is suffix (".com", ".org", etc.)
};

//static ble_bas_t  m_bas;                              // Structure used to identify the battery service. 

static ble_gap_adv_params_t     m_adv_params;                                       /**< Parameters to be passed to the stack when starting advertising. */
static ble_gatts_char_handles_t m_char_handles;                                     /**< Handles of local characteristic (as provided by the BLE stack).*/
static uint16_t                 m_conn_handle = BLE_CONN_HANDLE_INVALID;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
static uint16_t                 m_service_handle;                                   /**< Handle of local service (as provided by the BLE stack).*/
static bool                     m_is_notifying_enabled = false;                     /**< Variable to indicate whether the notification is enabled by the peer.*/
                                                    /**< Notification timer. */
#define APP_CFG_CHAR_LEN              20 
static uint8_t                  m_char_value[APP_CFG_CHAR_LEN];


/**@brief Function for the GAP initialization.
 *
 * @details This function shall be used to setup all the necessary GAP (Generic Access Profile)
 *          parameters of the device. It also sets the permissions.
 *
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME, 
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);
    
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    // Set GAP Peripheral Preferred Connection Parameters (converting connection interval from
    // milliseconds to required unit of 1.25ms).
    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
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
    m_adv_params.interval    = NON_CONNECTABLE_ADV_INTERVAL;
    m_adv_params.timeout     = NON_CONNECTABLE_ADV_TIMEOUT;
}


static uint32_t temperature_data_get(void)
{
    int32_t temp;
    uint32_t err_code;
    
    err_code = sd_temp_get(&temp);
    APP_ERROR_CHECK(err_code);
    
    temp = (temp / 4); //* 100;
    
    //int8_t exponent = -2;
    //return ((exponent & 0xFF) << 24) | (temp & 0x00FFFFFF);
		return temp;
}

/**@brief Function for initializing the Advertisement packet in the Eddystone format.
 *
 * @details This function initializes the data that is to be placed in an advertisement packet 
 *
 */
static void advertising_Eddystone_init(uint8_t* dataArray, uint16_t size)
{
    uint32_t                   err_code;
    ble_advdata_t              advdata;
    //ble_advdata_manuf_data_t   manuf_data;
		uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
		ble_uuid_t    adv_uuids[] = {{0X0F3E, BLE_UUID_TYPE_BLE}};

		
		// Service data (Eddystone)
		uint8_array_t data_array; 
		uint32_t temperature_data = temperature_data_get();
		eddystone_tlm_data[4] = (uint8_t) temperature_data;
		//eddystone_tlm_data[5] = (uint8_t) (temperature_data >> 8);
		
		data_array.p_data = dataArray;                 // Pointer to the data to advertise.
    data_array.size = size;
	
	  ble_advdata_service_data_t service_data;            // Structure to hold Service Data.
    service_data.service_uuid = 0xFEAA;                 // Eddystone UUID to allow discoverability on iOS devices.
    service_data.data = data_array;   
	


    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));
		
	
		//int8_t tx_power_level = TX_POWER_LEVEL;
		
		//advdata.p_tx_power_level = &tx_power_level;
		//advdata.include_appearance = true;
    //advdata.name_type             = BLE_ADVDATA_SHORT_NAME;
		//advdata.short_name_len				= 3;
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

/**@brief Function for initializing the Advertisement packet.
 *
 * @details This function initializes the data that is to be placed in an advertisement packet in 
 *          both connectable and non-connectable modes.
 *
 */
static void advertising_data_init(void)
{
	
	  uint32_t                   err_code;
    ble_advdata_t              advdata;
    ble_advdata_manuf_data_t   manuf_data;
    uint8_t                    flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;//BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;


    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));

    manuf_data.company_identifier = 0x00; // Company identifier coming soon. 
    manuf_data.data.size          = sizeof(adv_data);
    manuf_data.data.p_data        = (uint8_t *)adv_data;
		
	  advdata.name_type             = BLE_ADVDATA_NO_NAME;
    advdata.flags                 = flags;
    advdata.p_manuf_specific_data = &manuf_data;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

}

/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    uint32_t err_code;
    
    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for adding the Characteristic.
 *
 * @details This function adds the characteristic to the local db.
 *
 * @param[in] uuid_type Type of service UUID assigned by the SoftDevice.
 *
 */
//static void char_add(const uint8_t uuid_type)
//{
//    uint32_t            err_code;
//    ble_gatts_char_md_t char_md;
//    ble_gatts_attr_md_t cccd_md;
//    ble_gatts_attr_t    attr_char_value;
//    ble_uuid_t          char_uuid;
//    ble_gatts_attr_md_t attr_md;

//    memset(&cccd_md, 0, sizeof(cccd_md));

//    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

//    memset(&char_md, 0, sizeof(char_md));

//    char_md.char_props.read   = 1;
//    char_md.char_props.notify = 1;
//    char_md.p_char_user_desc  = NULL;
//    char_md.p_char_pf         = NULL;
//    char_md.p_user_desc_md    = NULL;
//    char_md.p_cccd_md         = &cccd_md;
//    char_md.p_sccd_md         = NULL;

//    char_uuid.type = uuid_type;
//    char_uuid.uuid = 0x0000;//0x1524; // Proprietary UUID for local characteristic.

//    memset(&attr_md, 0, sizeof(attr_md));

//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

//    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
//    attr_md.rd_auth = 0;
//    attr_md.wr_auth = 0;
//    attr_md.vlen    = 0;

//    memset(&attr_char_value, 0, sizeof(attr_char_value));

//    attr_char_value.p_uuid    = &char_uuid;
//    attr_char_value.p_attr_md = &attr_md;
//    attr_char_value.init_len  = 20; // Size of the characteristic value being notified (in bytes).
//    attr_char_value.init_offs = 0;
//    attr_char_value.max_len   = 20; // Size of the characteristic value being notified (in bytes).
//    attr_char_value.p_value   = m_char_value;

//    err_code = sd_ble_gatts_characteristic_add(m_service_handle,
//                                               &char_md,
//                                               &attr_char_value,
//                                               &m_char_handles);
//    APP_ERROR_CHECK(err_code);
//}


/**@brief Function for adding the Service.
 *
 * @details This function adds the service and the characteristic within it to the local db.
 *
 */
//static void service_add(void)
//{
//    ble_uuid_t  service_uuid;
//    uint32_t    err_code;
// 
//    service_uuid.uuid = 0x1523; //Proprietary UUID for local service.

//    err_code = sd_ble_uuid_vs_add(&m_base_uuid128, &service_uuid.type);
//    APP_ERROR_CHECK(err_code);
//    
//    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &m_service_handle);
//    APP_ERROR_CHECK(err_code);

//    // Add characteristics
//    char_add(service_uuid.type);
//}

 /**@brief Function for initializing services that will be used by the application.
  *
  * @details Initialize the Battery service.
  */
// static void battery_services_init(void)
// {
//    uint32_t       err_code;
//    ble_bas_init_t bas_init;
//   
// // Initialize Battery Service.
//    memset(&bas_init, 0, sizeof(bas_init));

//    // Here the sec level for the Battery Service can be changed/increased.
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.cccd_write_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.read_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);

//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_report_read_perm);

//    bas_init.evt_handler          = NULL;
//    bas_init.support_notification = true;
//    bas_init.p_report_ref         = NULL;
//    bas_init.initial_batt_level   = 100;

//    err_code = ble_bas_init(&m_bas, &bas_init);
//    APP_ERROR_CHECK(err_code);

//    // Initialize Device Information Service.
//		ble_dis_init_t dis_init;
//		
//    memset(&dis_init, 0, sizeof(dis_init));

//    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, (char *)DEVICE_NAME);

//    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
//    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

//    err_code = ble_dis_init(&dis_init);
//    APP_ERROR_CHECK(err_code);
// }


/**@brief Function for the Characteristic notification.
 *
 * @details Sends one characteristic value notification to peer if connected to it and the
 *          notification is enabled.
 */
//static void char_notify(void)
//{
//    uint32_t err_code;
//    uint16_t len = 20;

//    // Send value if connected and notifying.
//    if ((m_conn_handle != BLE_CONN_HANDLE_INVALID) && m_is_notifying_enabled)
//    {
//        ble_gatts_hvx_params_t hvx_params;

//        memset(&hvx_params, 0, sizeof(hvx_params));
//        len = sizeof(uint8_t);

//        hvx_params.handle   = m_char_handles.value_handle;
//        hvx_params.type     = BLE_GATT_HVX_NOTIFICATION;
//        hvx_params.offset   = 0;
//        hvx_params.p_len    = &len;
//        hvx_params.p_data   = m_char_value;

//        err_code = sd_ble_gatts_hvx(m_conn_handle, &hvx_params);
//        if ((err_code != NRF_SUCCESS) &&
//            (err_code != NRF_ERROR_INVALID_STATE) &&
//            (err_code != BLE_ERROR_NO_TX_PACKETS) &&
//            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
//        )
//        {
//            APP_ERROR_HANDLER(err_code);
//        }
//    }
//}

/**@brief Function for evaluating the value written in CCCD 
 *
 * @details This shall be called when there is a write event received from the stack. This 
 *          function will evaluate whether or not the peer has enabled notifications and
 *          start/stop notifications accordingly.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
//static void on_write(ble_evt_t * p_ble_evt)
//{
//    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

//    if ((p_evt_write->handle == m_char_handles.cccd_handle) && (p_evt_write->len == 2))
//    {
//        // CCCD written. Start notifications
//        m_is_notifying_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

//        if (m_is_notifying_enabled)
//        {
//   //         application_timers_start();
//            char_notify();
//        }
//        else
//        {
// //           application_timers_stop();
//        }
//    }
//}


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
   int32_t err_code;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            
            // application_timers_stop();
            
            // // Go to system-off mode            
            // err_code = sd_power_system_off();
            // APP_ERROR_CHECK(err_code);
            break;

        // case BLE_GATTS_EVT_SYS_ATTR_MISSING:
        //     err_code = sd_ble_gatts_sys_attr_set(m_conn_handle,
        //                                          NULL,
        //                                          0,
        //                                          BLE_GATTS_SYS_ATTR_FLAG_SYS_SRVCS | BLE_GATTS_SYS_ATTR_FLAG_USR_SRVCS);
        //     APP_ERROR_CHECK(err_code);
        //     break;

        // case BLE_GAP_EVT_TIMEOUT:
        //     if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
        //     { 
        //         // Go to system-off mode (this function will not return; wakeup will cause a reset).
        //         err_code = sd_power_system_off(); 
        //         APP_ERROR_CHECK(err_code);
        //     }
        //     break;
            
        case BLE_GATTS_EVT_WRITE:
            //on_write(p_ble_evt);
            break;

        default:
            break;
    }
  
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
    err_code = softdevice_ble_evt_handler_set(on_ble_evt);
    APP_ERROR_CHECK(err_code);
    
//    // Register with the SoftDevice handler module for BLE events.
//    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
//    APP_ERROR_CHECK(err_code);
}


// /**@brief Function for the Timer initialization.
//  *
//  * @details Initializes the timer module. This creates and starts application timers.
//  */
 static void timers_init(void)
 {
     //uint32_t err_code;

     // Initialize timer module.
     APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    
     // Create timers.
//     err_code = app_timer_create(&m_battery_timer_id,
//                                 APP_TIMER_MODE_REPEATED,
//                                 battery_level_meas_timeout_handler);
//     APP_ERROR_CHECK(err_code);
    
 }

 
// /**@brief Function that check for button press
//  *
//  * @details Change advertising type.
//  */
void button_check(void)
{
  bool is_button1_pressed; // Eddystone UID
	bool is_button2_pressed; // Eddystone URL
	bool is_button3_pressed; // Eddystone TLM
  bool is_button4_pressed; // iBeacon
	
	uint32_t err_code;
  // Initialize buttons
	err_code = bsp_init(BSP_INIT_BUTTONS, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
  APP_ERROR_CHECK(err_code);
	
  // Button 1
	err_code = bsp_button_is_pressed(0, &is_button1_pressed);
  APP_ERROR_CHECK(err_code);
	
  // Button 2
	err_code = bsp_button_is_pressed(1, &is_button2_pressed);
  APP_ERROR_CHECK(err_code);
	
  // Button 3
	err_code = bsp_button_is_pressed(2, &is_button3_pressed);
  APP_ERROR_CHECK(err_code);
	
  // Button 4
  err_code = bsp_button_is_pressed(3, &is_button4_pressed);
  APP_ERROR_CHECK(err_code);
  
  if(is_button1_pressed)
	{
		non_connectable_adv_init();
		advertising_Eddystone_init((uint8_t *)eddystone_uid_data, sizeof(eddystone_uid_data));
	}
	else if(is_button2_pressed)
	{
		non_connectable_adv_init();
		advertising_Eddystone_init((uint8_t *)eddystone_url_data, sizeof(eddystone_url_data));
	}
	else if(is_button3_pressed)
	{
		non_connectable_adv_init();
		advertising_Eddystone_init((uint8_t *)eddystone_tlm_data, sizeof(eddystone_tlm_data));
	}
  else if(is_button4_pressed)
	{
		//non_connectable_adv_init();
		//advertising_iBeacon_init();		
//		connectable_adv_init();
//	  service_add();
//		battery_services_init();
//		advertising_data_init();
		
	}
	else
	{
		 err_code = sd_power_system_off();  
     APP_ERROR_CHECK(err_code);
	}
}


/**@brief Function for the Power manager.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for application main entry.
 */
int main(void)
{
		uint32_t err_code;
	
		ble_stack_init();
		gap_params_init();
	
	    //timers_init();
		//button_check();
		non_connectable_adv_init();
		advertising_Eddystone_init((uint8_t *)eddystone_url_data, sizeof(eddystone_url_data));
	
	
	  //uint8_t  battery_level = 100;


   err_code = ble_bas_battery_level_update(&m_bas, battery_level);
   if ((err_code != NRF_SUCCESS) &&
       (err_code != NRF_ERROR_INVALID_STATE) &&
       (err_code != BLE_ERROR_NO_TX_PACKETS) &&
       (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
       )
   {
       APP_ERROR_HANDLER(err_code);
   }
	
	
	
	
	
	
	
	
		
		advertising_start();
	
	  //sd_power_dcdc_mode_set(1);
    //sd_ble_gap_tx_power_set(TX_POWER_LEVEL);
	

		// Enter main loop.
		for (;;)
		{
				power_manage();
		}
}

/** 
 * @}
 */
