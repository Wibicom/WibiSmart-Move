
#ifndef CUSTOM_BOARD_H
#define CUSTOM_BOARD_H

#define BSP_BUTTON_0   25 // 13 on DK
#define BSP_BUTTON_0_MASK (1<<BSP_BUTTON_0)

// Defines the UART connection with J-Link.
#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6
#define CTS_PIN_NUMBER 7
#define RTS_PIN_NUMBER 5
#define HWFC           true

// Arduino board mappings
#define ARDUINO_SCL_PIN             14    // SCL signal pin
#define ARDUINO_SDA_PIN             13    // SDA signal pin

#define ARDUINO_A4_PIN              28    // Analog channel 4
#define ARDUINO_A5_PIN              29    // Analog channel 5

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#endif
