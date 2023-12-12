#include "msp.h" 
#include "Common.h"

#define SWITCH1 2  // P1.1 
#define SWITCH2 16 // P1.4

void Switches_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~(SWITCH1 | SWITCH2);
	P1->SEL1 &= ~(SWITCH1 | SWITCH2);
	// configure as input
	P1->DIR &= ~(SWITCH1 | SWITCH2);
	P1->REN |= (SWITCH1 | SWITCH2);
	P1->OUT |= (SWITCH1 | SWITCH2);          
}

void Switch1_Init(void){
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~SWITCH1;
	P1->SEL1 &= ~SWITCH1;
	// configure as input
	P1->DIR &= ~SWITCH1;
	P1->REN |= SWITCH1;
	P1->OUT |= SWITCH1;
}

void Switch2_Init(void){
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~SWITCH2;
	P1->SEL1 &= ~SWITCH2;
	// configure as input
	P1->DIR &= ~SWITCH2;
	P1->REN |= SWITCH2;
	P1->OUT |= SWITCH2;
}
