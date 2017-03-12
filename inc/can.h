#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

#include <string.h>

void canInit();
uint8_t canSend(uint32_t id, const uint8_t *msg);
