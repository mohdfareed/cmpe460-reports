#include "msp.h" 
#include "Common.h"
#include "leds.h"

void LED1_Init(void)
{
	// configure PortPin for LED1 as port I/O 
	P1->SEL0 &= ~LED1;
	P1->SEL1 &= ~LED1;

	// make built-in LED1 LED high drive strength
	P1->DS |= LED1;

	// make built-in LED1 out	 
	P1->DIR |= LED1;

	// turn off LED
	P1->OUT &= ~1;
}

void LED2_Init(void)
{
	// configure PortPin for LED2 as port I/O 
	P2->SEL0 &= ~(RED | GREEN | BLUE);
	P2->SEL1 &= ~(RED | GREEN | BLUE);

	// make built-in LED2 LEDs high drive strength
	P2->DS |= (RED | GREEN | BLUE);

	// make built-in LED2 out	 
	P2->DIR |= (RED | GREEN | BLUE);

	// turn off LED
	P2->OUT &= ~(RED | GREEN | BLUE); 
}

void LED1_On(void)
{
	P1->OUT |= LED1;
}

void LED1_Off(void)
{
	P1->OUT &= ~LED1;
}

void LED2_Set(unsigned int color)
{
	P2->OUT &= ~WHITE;
	P2->OUT |= color;
}
