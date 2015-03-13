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
char uart1_message;
unsigned char call_floor,dest_floor; // where the calling came from and what is the goal of the call
unsigned char digit_switch;


void UART1IntHandler(void){
	// id kell még majd a pont renszer függvény, ami alapján betölti a targetlist tömbbe, az adott helyre rak egy 1 est, ahol nincs target az 0
	uint32_t ui32Status;

	ui32Status = UARTIntStatus(UART1_BASE, true);   // Get the interrrupt status.
	UARTIntClear(UART1_BASE, ui32Status);// Clear the asserted interrupts.

	while(UARTCharsAvail(UART1_BASE)){
		uart1_message = UARTCharGetNonBlocking(UART1_BASE);
		if(uart1_message == 'c'){ // this is looking for the begining of a call message, eg: c45- cal from 4th floor to 5th
			digit_switch = 1;
			break;
		}
		if(digit_switch == 1){// get in the call_Floor
			call_floor = uart1_message;
			digit_switch = 2;
			break;
		}
		if(digit_switch == 2){ // get in the destination
			dest_floor = uart1_message;
		}
		UARTCharPutNonBlocking(UART0_BASE,call_floor);
		UARTCharPutNonBlocking(UART0_BASE,dest_floor);
		//UARTCharPutNonBlocking(UART0_BASE,uart1_message);
	}
}// end of UARTIntHandler()

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

void UART1Init(void){
	// Enable the GPIO Peripheral used by the UART.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	// Enable UART1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    // Configure GPIO Pins for UART mode.
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioConfig(1, 115200, g_ui32SysClock);

	/*UARTConfigSetExpClk(UART1_BASE, g_ui32SysClock, 38400,
	                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	                             UART_CONFIG_PAR_NONE));*/
	// Enable the UART interrupt.
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);
} // end of UART1Init


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



