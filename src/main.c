#include "main.h"
#include "can.h"
#include "settings.h"

int main()
{
    gpioInit();
    canInit();

    do {
        loop();
    } while (1);
}

void gpioInit()
{
    // Enable clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // can gpio init
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);

    gpio.GPIO_Pin   = CAN_RX_PIN | CAN_TX_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    /* Connect CAN_RX & CAN_TX to AF9 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //CAN_RX = PA11
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //CAN_TX = PA12

    // leds init
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);

    // buttons init
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = USER_BUTTON;
    GPIO_Init(GPIOA, &gpio);
}

void loop()
{
    // reset all leds state
    GPIO_ResetBits(GPIOD, LEDS);

    if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON)) {
        if (canSend(MFSW, MFSW_VOL_DOWN) == CANTXOK) {
            GPIO_SetBits(GPIOD, LED_GREEN);
        } else {
            GPIO_SetBits(GPIOD, LED_RED);
        }
    }
}

void delay(uint32_t ms)
{
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

