#include "main.h"
//#include "can.h"
#include "settings.h"
#include <stdio.h>

int main()
{
    printf("Init");
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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // can gpio init
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);

    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); //CAN_RX = PB8
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); //CAN_TX = PB9

    /* Configure CAN RX and TX pins */
    gpio.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // leds init
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);

    // buttons init
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Pin = USER_BUTTON;
    GPIO_Init(GPIOA, &gpio);
}

void loop()
{
    // reset all leds state
//    GPIO_ResetBits(GPIOD, LEDS);

    if (GPIO_ReadInputDataBit(GPIOA, USER_BUTTON)) {
        switch(canSend(MFSW, MFSW_VOL_DOWN)) {
		case CANTXOK:
		//	GPIO_SetBits(GPIOD, LED_GREEN);
		break;
                case CANTXFAILED:
                  //      GPIO_SetBits(GPIOD, LED_RED);
                break;
                case CANTXPENDING:
                    //    GPIO_SetBits(GPIOD, LED_BLUE);
                break;
//                case CANTXOK:
  //                      GPIO_SetBits(GPIOD, LED_GREEN);
    //            break;
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

void canInit()
{
    CAN_InitTypeDef can;
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    //can.CAN_NART = DISABLE;
    can.CAN_NART = ENABLE; // non-automatic retransmission mode = ENABLE (To prevent endless spam)
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = DISABLE;
    can.CAN_Mode = CAN_Mode_Normal; // CAN_MODE_SILENT

    can.CAN_SJW       = CAN_SJW_1tq; // synchronization jump width = 1
    can.CAN_BS1       = CAN_BS1_6TQ;
    can.CAN_BS2       = CAN_BS2_3TQ;
    can.CAN_Prescaler = 42; // 100 kbit/s

    // Bitrate = CAN_CLK / (CAN_Prescaler * (1 + CAN_BS1 + CAN_BS2 + CAN_SJW)).
    CAN_Init(CAN1, &can);
}

uint8_t canSend(uint32_t id, const uint8_t *msg)
{
    CanTxMsg TxMessage;

    TxMessage.RTR   = CAN_RTR_DATA;
    TxMessage.IDE   = CAN_ID_STD;
    TxMessage.DLC   = sizeof(msg);
    TxMessage.StdId = id;

    for (int i = 0; i < TxMessage.DLC; i++) {
        TxMessage.Data[i] = msg[i];
    }

    uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);

    int i = 0;

    uint8_t status;
    while (1) {
	 GPIO_ResetBits(GPIOD, LEDS);
         status = CAN_TransmitStatus(CAN1, mailbox);
         if (status == CANTXOK) {
              GPIO_SetBits(GPIOD, LED_GREEN);
              break;
         } else if (status == CANTXFAILED) {
              GPIO_SetBits(GPIOD, LED_RED);
              break;
         } else if (i == 0xFFFF) {
              GPIO_SetBits(GPIOD, LED_ORANGE);
              break;
         } else {
              GPIO_SetBits(GPIOD, LED_BLUE);
         }
    }

    return status;
}

