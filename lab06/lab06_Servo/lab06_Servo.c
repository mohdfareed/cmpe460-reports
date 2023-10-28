/*
 * Main code for testing servo
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
	TIMER_A2_PWM_Init(60000, 0.075, 1); // 60000 cycle period -> 50Hz

	// Print welcome over serial
	uart0_put("Running... \n\r");

	for (;;){
        TIMER_A2_PWM_DutyCycle(0.05, 1);
        delay(1000);
        TIMER_A2_PWM_DutyCycle(0.075, 1);
        delay(1000);
        TIMER_A2_PWM_DutyCycle(0.1, 1);
        delay(1000);
        TIMER_A2_PWM_DutyCycle(0.075, 1);
        delay(1000);
    }

	return 0;
}
