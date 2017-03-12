#ifndef settings_h
#define settings_h

const uint16_t LED_GREEN = GPIO_Pin_12;
const uint16_t LED_ORANGE = GPIO_Pin_13;
const uint16_t LED_RED = GPIO_Pin_14;
const uint16_t LED_BLUE = GPIO_Pin_15;

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

const uint16_t USER_BUTTON = GPIO_Pin_0;

const uint16_t CAN_RX_PIN = GPIO_Pin_11;
const uint16_t CAN_TX_PIN = GPIO_Pin_12;

//uint8_t MFSW_NONE[]         = {0x39, 0x00}; // Button released
//uint8_t MFSW_PREV[]         = {0x39, 0x02};
//uint8_t MFSW_NEXT[]         = {0x39, 0x03};
//uint8_t MFSW_SCAN_UP[]      = {0x39, 0x04}; // not used for rns-e?
//uint8_t MFSW_SCAN_DOWN[]    = {0x39, 0x05}; // not used for rns-e?
const uint8_t MFSW_VOL_UP[]       = {0x39, 0x06};
const uint8_t MFSW_VOL_DOWN[]     = {0x39, 0x07};
//uint8_t MFSW_VOL_PRESSED[]  = {0x3B, 0xA7};

const uint32_t MFSW = 0x5c1;


#endif /* settings_h */
