#include "main.h"

static uint8_t lastButtonStatus = RESET;

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
    static uint32_t counter = 0;

    uint8_t currentButtonStatus = GPIO_ReadInputDataBit(GPIOA, USER_BUTTON);

    if (lastButtonStatus != currentButtonStatus && currentButtonStatus != RESET) {
        ++counter;
        sendMessage();
        
        GPIO_ResetBits(GPIOD, LEDS);
        GPIO_SetBits(GPIOD, LED[counter % 4]);
    }
    lastButtonStatus = currentButtonStatus;
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
    GPIO_InitTypeDef gpio;
    CAN_InitTypeDef can;

    /* Configure CAN1 RX pin */
    gpio.GPIO_Pin = GPIO_Pin_8;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOB, &gpio);

    /* Configure CAN1 TX pin */
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    can.CAN_TTCM=DISABLE;
    can.CAN_ABOM=DISABLE;
    can.CAN_AWUM=DISABLE;
    can.CAN_NART=DISABLE;
    can.CAN_RFLM=DISABLE;
    can.CAN_TXFP=DISABLE;
    can.CAN_Mode=CAN_Mode_LoopBack;
    //can.CAN_Mode=CAN_Mode_Normal;
    can.CAN_SJW=CAN_SJW_1tq;
    can.CAN_BS1=CAN_BS1_2tq;
    can.CAN_BS2=CAN_BS2_3tq;
    can.CAN_Prescaler=48;

    CAN_Init(CAN1, &can);
}

void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

void sendMessage()
{
	CanTxMsg TxMessage;
	TxMessage.StdId=0x71;
  
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.DLC=8;
	TxMessage.ExtId=6;
	TxMessage.Data[0]=0xaa;
	TxMessage.Data[1]=0xbb;
	TxMessage.Data[2]=0xcc;
	TxMessage.Data[3]=0x11;
	TxMessage.Data[4]=0x22;
	TxMessage.Data[5]=0x33;
	TxMessage.Data[6]=0x44;
	TxMessage.Data[7]=0x55;
	
	//uint8_t result = 
	CAN_Transmit(CAN1, &TxMessage);  
}
