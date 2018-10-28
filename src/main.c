#include "main.h"
#include "settings.h"
#include <stdio.h>

int main()
{
    printf("Init");
    initLeds();
    initUsart();
    sendUart();
}

void sendUart()
{
//	GPIO_SetBits(GPIOD, LED_GREEN);


	for (;;)
	{
		USART_SendData(USART3, 0x55);
//		USART_ReadByteSync(USART3);

//		GPIO_SetBits(GPIOD, LED_GREEN);
	}

}

unsigned char USART_ReadByteSync(USART_TypeDef *USARTx)
{
    while ((USARTx->SR & USART_SR_RXNE) == 0)
    {
    }

    return (unsigned char)USART_ReceiveData(USARTx);
}

void initUsart() {
    USART_InitTypeDef usartConfig;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_APB1Periph_USART3, ENABLE);
    USART_Cmd(USART3, ENABLE); 

    usartConfig.USART_BaudRate = 9600; 
    usartConfig.USART_WordLength = USART_WordLength_8b; 
    usartConfig.USART_StopBits = USART_StopBits_1; 
    usartConfig.USART_Parity = USART_Parity_No;
    usartConfig.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &usartConfig);

    GPIO_InitTypeDef gpioConfig;

    //PC10 = USART3.TX => Alternative Function Output
    gpioConfig.GPIO_Mode = GPIO_Mode_AF;
    gpioConfig.GPIO_Pin = GPIO_Pin_10;
    gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpioConfig);

    //PC11 = USART3.RX => Input
    gpioConfig.GPIO_Mode = GPIO_Mode_AIN;
    gpioConfig.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &gpioConfig);

    GPIO_SetBits(GPIOD, LED_RED);
}

void initLeds()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef gpio;

    // leds init
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOD, &gpio);

    GPIO_SetBits(GPIOD, LED_BLUE);
}

void delay(uint32_t ms)
{
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

