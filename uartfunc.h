/*
 * uart.h
 *
 *  Created on: 2015.02.18.
 *      Author: Elek
 */

#ifndef UART_H_
#define UART_H_

extern void UARTInit(void);
extern void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

#endif /* UART_H_ */
