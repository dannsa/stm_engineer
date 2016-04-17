/* uart_for_bt.h
 *
 *  Created on: 16.04.2016
 *      Author: JS
 */

#include "main.h"
#include "uart_for_bt.h"

// Configuration for USART2 on Port_A PIN_2_TX, PIN3_RX
void usart2_configuration()
{
    //Clocks
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (0x8 << 4);  // alternate function
    GPIOA->OSPEEDR |= (0xf << 4);  // max speed
    GPIOA->PUPDR |= (0x5 << 4);  // pull up
    GPIOA->AFR[0] |= (0x77 << 8);  // AF7 USART2 alternate function for pin2,3

    // Conf
    USART_InitTypeDef uart;
    uart.USART_BaudRate = 9600;
    uart.USART_WordLength = USART_WordLength_8b;
    uart.USART_StopBits = USART_StopBits_1;
    uart.USART_Parity = USART_Parity_No;
    uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &uart);

    // TODO interupt configuration

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannel = USART2_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    // Enable
    USART_Cmd(USART2, ENABLE);
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
int xuart_getChar (USART_TypeDef* USARTx)
{
    if (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == SET)
    {
        USART_ClearFlag(USARTx, USART_FLAG_RXNE);
        return USARTx->DR & 0x01ff;
    }
    return 0;
}
