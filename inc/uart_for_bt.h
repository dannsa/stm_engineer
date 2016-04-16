/* uart_for_bt.h
 *
 *  Created on: 16.04.2016
 *      Author: JS
 */

 #ifndef UART_FOR_BT_H
 #define UART_FOR_BT_H

// Configuration
void usart2_configuration();
// One char send
void xuart_sendChar(uint8_t c, USART_TypeDef* USARTx);
// String send
void xuart_sendString(USART_TypeDef* USARTx, uint8_t* txBuff);
// Receive char
int xuart_getChar(USART_TypeDef* USARTx);

#endif /* UART_FOR_BT_H */
