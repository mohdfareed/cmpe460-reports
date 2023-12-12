/*
 * Main module for testing the PWM Code for the K64F
 *
 * Author:
 * Created:
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu>
 * LJBeato
 * 2021
 */

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

/**
 * Waits for a delay (in milliseconds)
 *
 * del - The delay in milliseconds
 */
void delay(int del)
{
	volatile int i;
	for (i = 0; i < del * 278; i++)
	{
		; // Do nothing
	}
}

int main(void)
{
	// Initialize UART and PWM
	uart0_init();
	TIMER_A0_PWM_Init(300, 0.0, 1); // period = 300 cycles -> 10kHz
	TIMER_A0_PWM_Init(300, 0.0, 4);

	// Print welcome over serial
	uart0_put("Running... \n\r");

	// Generate 20% duty cycle at 10kHz
    // TIMER_A0_PWM_DutyCycle(0.2, 1);

	for(;;)  //loop forever
	{
		uint16_t dc = 0;
		uint16_t freq = 10000; // Frequency = 10 kHz
		uint16_t dir = 0;
		char c = 48;
		int i=0;

		// 0 to 100% duty cycle in forward direction
		for (i=0; i<100; i++) {
			TIMER_A0_PWM_DutyCycle((double)i/100.0, 1);
			delay(10);
		}

		// 100% down to 0% duty cycle in the forward direction
		for (i=100; i>=0; i--) {
			TIMER_A0_PWM_DutyCycle((double)i/100.0, 1);
			delay(10);
		}

		// 0 to 100% duty cycle in reverse direction
		for (i=0; i<100; i++) {
			TIMER_A0_PWM_DutyCycle((double)i/100.0, 4);
			delay(10);
		}

		// 100% down to 0% duty cycle in the reverse direction
		for (i=100; i>=0; i--) {
			TIMER_A0_PWM_DutyCycle((double)i/100.0, 4);
			delay(10);
		}

	}

	return 0;
}
