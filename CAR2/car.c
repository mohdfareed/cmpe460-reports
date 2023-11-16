#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "ControlPins.h"

uint16_t line[128];
BOOLEAN g_sendData;

static char str[100];

void INIT_Camera(void)
{
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

int main(void)
{
	int i = 0;

	// initializations
	DisableInterrupts();
	uart0_init();

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

		// do a small delay
		Clock_Delay1ms();
	}
}