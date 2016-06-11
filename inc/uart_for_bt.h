/* uart_for_bt.h
 *
 *  Created on: 16.04.2016
 *      Author: JS
 */

 #ifndef UART_FOR_BT_H
 #define UART_FOR_BT_H

// Configuration
void bluetooth_config(void);
// One char send
void xuart_sendChar(uint8_t c, USART_TypeDef* USARTx);
// String send
void xuart_sendString(USART_TypeDef* USARTx, uint8_t* txBuff);
// Receive char
char xuart_getChar(USART_TypeDef* USARTx);

void xuart_putNumber(USART_TypeDef* USARTx, uint16_t x);
void xuart_sendADC(USART_TypeDef* USARTx, uint16_t x);
// Change configuration for bt module HC-05
void runConfigurationAtCommands(void);
#endif /* UART_FOR_BT_H */
