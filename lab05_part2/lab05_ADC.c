/*
* Rochester Institute of Technology
* Department of Computer Engineering
* CMPE 460  Interfacing Digital Electronics
* LJBeato
* 1/14/2021
*
* Filename: main_timer_template.c
*/
#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "switches.h"
// The sprintf function seemed to cause a hange in the interrupt service routine.
// I think if we increase the HEAP size, it will work
// change to Heap_Size       EQU     0x00000200 in startup_msp432p401r_uvision.s


BOOLEAN Timer1RunningFlag = FALSE;
BOOLEAN Timer2RunningFlag = FALSE;

unsigned long MillisecondCounter = 0;


// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
    char str[30];
    unsigned int num = ADC_In();
    sprintf(str, "\r\nADC input: 0x%08x\r\n", num);
    uart0_put(str);
    sprintf(str, "ADC input:  %d\r\n", num);
    uart0_put(str);
    double voltage = num * 2.5 / 16384.0;
    sprintf(str, "Voltage:    %fV\r\n", voltage);
    uart0_put(str);
    double celsius = voltage*100 - 50;
    sprintf(str, "Celsius:    %f\r\n", celsius);
    uart0_put(str);
    double fahrenheit = celsius * 1.8 + 32;
    sprintf(str, "Fahrenheit: %f\r\n", fahrenheit);
    uart0_put(str);
}

void Switch1_Interrupt_Init(void)
{
	// disable interrupts
	DisableInterrupts();
	// initialize the Switch as per previous lab
	Switch1_Init();

	// 7-0 PxIFG RW 0h Port X interrupt flag
	// 0b = No interrupt is pending.
	// 1b = Interrupt is pending.
	//  clear flag1 (reduce possibility of extra interrupt)
	P1->IFG &= ~2; // P1.1

	// 7-0 PxIE RW 0h Port X interrupt enable
	// 0b = Corresponding port interrupt disabled
	// 1b = Corresponding port interrupt enabled
	//  arm interrupt on  P1.1
	P1->IE |= 2;

	// 7-0 PxIES RW Undefined Port X interrupt edge select
	// 0b = PxIFG flag is set with a low-to-high transition.
	// 1b = PxIFG flag is set with a high-to-low transition
	//  now set the pin to cause falling edge interrupt event
	//  P1.1 is falling edge event
	P1->IES |= 2;

	// now set the pin to cause falling edge interrupt event
	NVIC_IPR8 = (NVIC_IPR8 & 0x00FFFFFF) | 0x40000000; // priority 2

	// enable Port 1 - interrupt 35 in NVIC
	NVIC_ISER1 = 0x00000008;

	// enable interrupts  (// clear the I bit	)
	EnableInterrupts();
}

void PORT1_IRQHandler(void)
{
    // First we check if it came from Switch1 ?
	if (P1->IFG & BIT1) // we start a timer to toggle the LED1 1 second ON and 1 second OFF
	{
		// acknowledge P1.1 is pressed, by setting BIT1 to zero - remember P1.1 is switch 1
		// clear flag, acknowledge
		P1->IFG &= ~BIT1;

		if (Timer1RunningFlag){
			TIMER32_CONTROL1 &= ~BIT7;
		}
		else{
			TIMER32_CONTROL1 |= BIT7;
		}
		
		// update state of timer
		Timer1RunningFlag = ~Timer1RunningFlag;
	}
}

// main
int main(void)
{
	//initializations
	uart0_init();
	uart0_put("\r\nLab5 ADC demo\r\n");
    Switch1_Interrupt_Init();
    // Set the Timer32-2 to 2Hz (0.5 sec between interrupts)
	Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock / 2, T32DIV1); // initialize Timer A32-1

	ADC0_InitSWTriggerCh6();
	EnableInterrupts();
    while(1){}
}

