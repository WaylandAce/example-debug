#include "main.h"
#include <stdio.h>

int main()
{
    printf("Init");

    init();

    do {
        loop();
    } while (1);
}

void init()
{
    // Enable clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // Init stuff
    initLeds();
    initButton();
    initCan();
}

void loop()
{
    // reset all leds state
    GPIO_ResetBits(GPIOD, LEDS);

    if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON)) {
	canSend(MFSW, MFSW_VOL_DOWN);
    }
}

void initLeds()
{
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);
}

void initButton()
{
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = USER_BUTTON;
    GPIO_Init(GPIOA, &gpio);
}

void initCan()
{
    GPIO_InitTypeDef gpio;
    CAN_InitTypeDef can;

    gpio.GPIO_Pin   = CAN_RX_PIN | CAN_TX_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    /* Connect CAN_RX & CAN_TX to AF9 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); //CAN_RX = PA11
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); //CAN_TX = PA12

    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    // can.CAN_NART = DISABLE;
    can.CAN_NART = ENABLE; // non-automatic retransmission mode = ENABLE (To prevent endless spam)
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = DISABLE;
    can.CAN_Mode = CAN_Mode_Normal;

    can.CAN_SJW       = CAN_SJW_1tq; // synchronization jump width = 1
    can.CAN_BS1       = CAN_BS1_14tq; // 14 time quantum
    can.CAN_BS2       = CAN_BS2_6tq;
    can.CAN_Prescaler = 20; // 100 kbit/s

    // Bitrate = CAN_CLK / (CAN_Prescaler * (1 + CAN_BS1 + CAN_BS2 + CAN_SJW)).
    CAN_Init(CAN1, &can);
}

void delay(uint32_t ms)
{
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

void canSend(uint32_t id, const uint8_t *msg)
{
    CanTxMsg TxMessage;

    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = sizeof(msg);

    TxMessage.StdId = id;

    memcpy(&TxMessage.Data, &msg, sizeof TxMessage.Data);

//    TxMessage.DLC = 2;
//    TxMessage.Data = msg;
//    TxMessage.Data[0] = 0x39;
//    TxMessage.Data[1] = 0x06;

    uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
    if (CAN_TransmitStatus(CAN1, mailbox) == CANTXOK) {
        GPIO_SetBits(GPIOD, LED_GREEN);
    } else {
        GPIO_SetBits(GPIOD, LED_RED);
    }
}
