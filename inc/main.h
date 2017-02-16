#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"

#include <string.h>

const uint16_t LEDS = GPIO_Pin_13 | GPIO_Pin_14;
const uint16_t USER_BUTTON = GPIO_Pin_0;

const uint16_t CAN_RX_PIN = GPIO_Pin_8;
const uint16_t CAN_TX_PIN = GPIO_Pin_9;

//uint8_t MFSW_NONE[]         = {0x39, 0x00}; // Button released
//uint8_t MFSW_PREV[]         = {0x39, 0x02};
//uint8_t MFSW_NEXT[]         = {0x39, 0x03};
//uint8_t MFSW_SCAN_UP[]      = {0x39, 0x04}; // not used for rns-e?
//uint8_t MFSW_SCAN_DOWN[]    = {0x39, 0x05}; // not used for rns-e?
uint8_t MFSW_VOL_UP[]       = {0x39, 0x06};
uint8_t MFSW_VOL_DOWN[]     = {0x39, 0x07};
//uint8_t MFSW_VOL_PRESSED[]  = {0x3B, 0xA7};

void init();
void loop();

void delay();

void initLeds();
void initButton();
void initCan();

void sendMessage(uint8_t *msg);
