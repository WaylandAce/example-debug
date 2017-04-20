#ifndef main_h
#define main_h

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"

#include <string.h>

void gpioInit();
void loop();
void delay();
void canInit();
uint8_t canSend(uint32_t id, const uint8_t *msg);

#endif /* main_h */
