#include "main.h"
#include "settings.h"
#include <stdio.h>

char uart3_rx_buf[128];
uint8_t uart3_rx_bit;

int main()
{
	printf("Init");
	initLeds();
	initUsart();
 
	__enable_irq(); //Глобальное включение прерывания
	NVIC_EnableIRQ(USART3_IRQn); //Включаем прерывания от UART
	NVIC_SetPriority(USART3_IRQn, 0); //Прерывание от UART, приоритет 0, самый высокий
	USART3->CR1 |= USART_CR1_RXNEIE; //Прерывание по приему
 
	while(1) {}
}

void USART3_IRQHandler (void)
{
	char uart_data;
	if (USART3->SR & USART_SR_RXNE) //Проверяем, прило ли чтонибудь в UART
	{
   		USART3->DR = USART3->DR; //Echo по приему, символ отправленный в консоль вернется
	        uart_data=USART3->DR; //Считываем то что пришло в переменную...
	   	uart3_rx_buf[uart3_rx_bit]=USART3->DR; //Помещаем принятый байт в буфер.
	   	uart3_rx_bit++; //Наращиваем счётчик байтов буфера.
 

		send_str("\n"); //Переходим на новую строку
	   	send_str("String: ");
	   	send_str(uart3_rx_buf); //Отправляем ее обратно в консоль

		memset(uart3_rx_buf, 0, sizeof(uart3_rx_buf)); //Очищаем буфер
		uart3_rx_bit=0; //Сбрасываем счетчик
		send_str("\n");
   	}
}

void send_to_uart(uint8_t data)
{
	while(!(USART3->SR & USART_SR_TC));
		USART3->DR=data;
}
 
//Функция отправляет строку в UART, по сути пересылая по байту в send_to_uart
void send_str(char * string)
{
	uint8_t i=0;
	while(string[i]) 
	{
  		send_to_uart(string[i]);
		i++;
	}
}

//Инициализируем USART2
void initUsart(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; //Структура содержащая настройки порта
	USART_InitTypeDef USART_InitStructure; //Структура содержащая настройки USART
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Включаем тактирование порта C
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //Включаем тактирование порта USART3
 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); //Подключаем PA3 к TX USART2
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3); //Подключаем PA2 к RX USART2
 
  //Конфигурируем PC10 как альтернативную функцию -> TX UART. Подробнее об конфигурации можно почитать во втором уроке.
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  //Конфигурируем PC11 как альтернативную функцию -> RX UART. Подробнее об конфигурации можно почитать во втором уроке.
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
//  USART_StructInit(&USART_InitStructure); //Инициализируем UART с дефолтными настройками: скорость 9600, 8 бит данных, 1 стоп бит
USART_InitStructure.USART_BaudRate = 9600; //Скорость обмена 9600 бод
USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
USART_InitStructure.USART_StopBits = USART_StopBits_1; //1 стоп-бит
USART_InitStructure.USART_Parity = USART_Parity_No ; //Без проверки четности
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART2
 

  USART_Init(USART3, &USART_InitStructure);
  USART_Cmd(USART3, ENABLE); //Включаем UART
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

