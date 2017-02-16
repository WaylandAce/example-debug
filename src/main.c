#include "main.h"


int main() {
    init();

    do {
        loop();
    } while (1);
}

void init() {
    initLeds();
    initButton();
    initCan();
}

void loop() {
    if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON)) {
	sendMessage(MFSW_VOL_DOWN);
        GPIO_SetBits(GPIOG, LEDS);
    } else {
        GPIO_ResetBits(GPIOG, LEDS);
    }
}

void initLeds() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);
}

void initButton() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = USER_BUTTON;
    GPIO_Init(GPIOA, &gpio);
}

void initCan()
{
    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio;
    CAN_InitTypeDef can;

    gpio.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    /* Connect CAN_RX & CAN_TX to AF9 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_CAN1); //CAN_RX = PD0
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_CAN1); //CAN_TX = PD1

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

void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

void sendMessage(uint8_t *msg)
{
    CanTxMsg TxMessage;

    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    // Specifies the length of the frame that will be transmitted. This parameter can be a value between 0 and 8
    TxMessage.DLC = sizeof(msg); // 2

    // Specifies the standard identifier. This parameter can be a value between 0 and 0x7FF.
    TxMessage.StdId = 0x5c1;

    memcpy(&TxMessage.Data, &msg, sizeof TxMessage.Data);

//    TxMessage.Data  = msg;
//    TxMessage.Data[0] = 0x39;
//    TxMessage.Data[1] = 0x06;

    //uint8_t result =
    CAN_Transmit(CAN1, &TxMessage);
}
