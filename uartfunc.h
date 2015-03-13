/*
 * uart.h
 *
 *  Created on: 2015.02.18.
 *      Author: Elek
 */

#ifndef UART_H_
#define UART_H_

extern char uart1_message;

extern void UARTInit(void);
extern void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
extern void UART1Init(void);
void UART1IntHandler(void);

#endif /* UART_H_ */
