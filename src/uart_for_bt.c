/* uart_for_bt.h
 *
 *  Created on: 16.04.2016
 *      Author: JS
 */

#include "main.h"
#include "uart_for_bt.h"

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
