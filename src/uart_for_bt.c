/* uart_for_bt.h
 *
 *  Created on: 16.04.2016
 *      Author: JS
 */

#include "main.h"
#include "uart_for_bt.h"
#include "systick.h"


void bluetooth_config(void)
{
    GPIO_InitTypeDef  gpio;
    // Alternate function  PC10 - TX, PC11 - RX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
    // Modes
    gpio.GPIO_Pin = (GPIO_Pin_10 | GPIO_Pin_11);
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);

    // Usart init structure
    USART_InitTypeDef bt;

    // Configuration
    bt.USART_StopBits = USART_StopBits_1;
    bt.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    bt.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    bt.USART_WordLength = USART_WordLength_8b;
    // Fixed baudrate aligned with owned module
    bt.USART_BaudRate = 921600;
    bt.USART_Parity = USART_Parity_No;
    USART_Init(USART3, &bt);
    //Interrupt configuration for reception
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // Vector for interrupts
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannel = USART3_IRQn;
    // Enable and init
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    // Enable Uart
    USART_Cmd(USART3, ENABLE);
}

// One char send
void xuart_sendChar(uint8_t cx, USART_TypeDef* USARTx)
{
    // Wait until tx buffer empty
    while(!(USARTx->SR & USART_SR_TXE));
    USARTx->DR = cx;
}

// String send
void xuart_sendString(USART_TypeDef* USARTx, uint8_t* txBuff)
{
    while(*txBuff != 0)
    {
        xuart_sendChar(*txBuff, USARTx);
        txBuff++;
    }
}

// Receive char
char xuart_getChar (USART_TypeDef* USARTx)
{
    if (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == SET)
    {
        USART_ClearFlag(USARTx, USART_FLAG_RXNE);
        return USARTx->DR & 0xff;
    }
    return 0;
}

void xuart_sendADC(USART_TypeDef* USARTx, uint16_t x)
{
    char value[6];
    int i = 0;

    do
    {
        value[i++] = (char)(x % 10) + '0'; //convert integer to character
        x /= 10;
    } while(x);

    while(i) //send data
    {
        xuart_sendChar(value[--i], USARTx);
    }
}
void runConfigurationAtCommands()
{
    //xuart_sendString(USART3, "AT+UART=921600,0,0\r\n");
    //delay_ms(1);
    //xuart_sendString(USART3, "AT+NAME=STM\r\n");
    //delay_ms(1);
}


void xuart_putNumber(USART_TypeDef* USARTx, uint16_t x)
{
    char value[10]; //a temp array to hold results of conversion
    int i = 0; //loop index
    do
    {
      value[i++] = (char)(x % 10) + '0'; //convert integer to character
      x /= 10;
    } while(x);

    while(i) //send data
    {
      xuart_sendChar( value[--i], USARTx);
    }
    xuart_sendChar('\n', USARTx);
}
