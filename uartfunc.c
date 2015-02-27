/*
 * uart.c
 *
 *  Created on: 2015.02.18.
 *      Author: Elek
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


int g_ui32SysClock;

void UARTInit(void)
{
	g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
			                                     SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
			                                     SYSCTL_CFG_VCO_480), 120000000);
   //
   // Enable the GPIO Peripheral used by the UART.
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
   //
   // Enable UART0
   //
   SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
   //
   // Configure GPIO Pins for UART mode.
   //
   GPIOPinConfigure(GPIO_PA0_U0RX);
   GPIOPinConfigure(GPIO_PA1_U0TX);
   GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
   //  IntEnable(INT_UART0);
   //  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
   //
   // Initialize the UART for console I/O.
   //
   UARTStdioConfig(0, 115200, g_ui32SysClock);
  // UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
    //	                          (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    	//                           UART_CONFIG_PAR_NONE));
}// end of UartInit()


void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
	// Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
    	//
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}// end of UARTSend



