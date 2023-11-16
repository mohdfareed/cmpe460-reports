#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "ControlPins.h"
#include "TimerA.h"

uint16_t line[128];
BOOLEAN g_sendData;

static char str[100];

/**
 * Waits for a delay (in milliseconds)
 *
 * del - The delay in milliseconds
 */
void delay_ms(int del)
{
	volatile int i;
	for (i = 0; i < del * 2224; i++)
	{
		; // Do nothing
	}
}

void INIT_Camera(void)
{
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

void INIT_Motors(void)
{
	// Motor1 Enable P3.6
	P3->SEL0 &= ~BIT6;
	P3->SEL1 &= ~BIT6;
	P3->DIR |= BIT6;
	P3->OUT &= ~BIT6;

	P3->SEL0 &= ~BIT7;
	P3->SEL1 &= ~BIT7;
	P3->DIR |= BIT7;
	P3->OUT &= ~BIT7;

	// Init_PWM_INTERRUPTS();
	TIMER_A0_PWM_Init(2400, 0.0, 1);
	TIMER_A0_PWM_Init(2400, 0.0, 2);
	TIMER_A0_PWM_Init(2400, 0.0, 3);
	TIMER_A0_PWM_Init(2400, 0.0, 4);
	TIMER_A2_PWM_Init(60000, 0.0, 1);

	delay_ms(250);
	P3->OUT |= BIT6;
	P3->OUT |= BIT7;
}

int main(void)
{
	int i = 0;

	// initializations
	DisableInterrupts();
	uart0_init();

	delay_ms(2500);
	INIT_Motors();

	INIT_Camera();
	EnableInterrupts();

	while (1)
	{
		if (g_sendData == TRUE)
		{
			// send the array over uart
			sprintf(str, "%i\n\r", -1); // start value
			uart0_put(str);

			for (i = 0; i < 128; i++)
			{
				sprintf(str, "%i\n\r", line[i]);
				uart0_put(str);
			}

			sprintf(str, "%i\n\r", -2); // end value
			uart0_put(str);

			g_sendData = FALSE;
		}

		uart0_put("Controlling DC Motors\r\n");
		TIMER_A0_PWM_DutyCycle(0.0, 4);
		TIMER_A0_PWM_DutyCycle(0.3, 1);
		delay_ms(1000);
		TIMER_A0_PWM_DutyCycle(0.0, 1);
		TIMER_A0_PWM_DutyCycle(0.3, 2);
		delay_ms(1000);
		TIMER_A0_PWM_DutyCycle(0.0, 2);
		TIMER_A0_PWM_DutyCycle(0.3, 3);
		delay_ms(1000);
		TIMER_A0_PWM_DutyCycle(0.0, 3);
		TIMER_A0_PWM_DutyCycle(0.3, 4);
		delay_ms(1000);
		TIMER_A0_PWM_DutyCycle(0.0, 4);

		uart0_put("Controlling Servo\r\n");
		TIMER_A2_PWM_DutyCycle(0.05, 1);
		delay_ms(1000);
		TIMER_A2_PWM_DutyCycle(0.1, 1);
		delay_ms(1000);
		TIMER_A2_PWM_DutyCycle(0.075, 1);
		delay_ms(1000);
	}
}
