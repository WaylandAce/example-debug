#ifndef main_h
#define main_h

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include <string.h>

void initUsart();
void initLeds();
void delay(uint32_t ms);
void sendUart();
unsigned char USART_ReadByteSync(USART_TypeDef *USARTx);

#endif /* main_h */
