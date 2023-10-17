#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "msp.h"
#include "uart.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"

char str[100];					// UART string
unsigned int data[NUM_SAMPLES]; // precomputed sine wave values
double delayCycles = 1000;		// delay cycles for 1 Hz sine wave

// sin wave constants
#define NUM_SAMPLES 1000				  // number of samples
double Vmax = 3.3;						  // max voltage
double Omega = 2 * 3.14159 / NUM_SAMPLES; // angular frequency

// write DAC value
void DAC_Write(unsigned int value)
{
	unsigned char byte1, byte2;
	P2->OUT &= ~BIT3; /* assert slave select */

	byte1 = (unsigned char)(0x30) | (value >> 4);
	while (!(EUSCI_B0->IFG & 2))
		; /* wait for transmit buffer empty */

	EUSCI_B0->TXBUF = byte1; /* write to SPI transmit buffer */
	while (EUSCI_B0->STATW & 1)
		; /* wait for transmit done */

	byte2 = (unsigned char)(value << 4);
	while (!(EUSCI_B0->IFG & 2))
		; /* wait for transmit buffer empty */

	EUSCI_B0->TXBUF = byte2; /* write to SPI transmit buffer */
	while (EUSCI_B0->STATW & 1)
		; /* wait for transmit done */

	P2->OUT |= BIT3; /* de-assert slave select */
}

// initialize SPI
void Init_SPI(void)
{
	EUSCI_B0->CTLW0 = 0x0001;	/* disable UCB0 during config */
	EUSCI_B0->CTLW0 = 0xE9C1;	/* config settings */
	EUSCI_B0->BRW = 3;			/* 3 MHz / 1 = 1 MHz */
	EUSCI_B0->CTLW0 &= ~0x0001; /* enable UCB0 after config */

	P1->SEL0 |= 0x60;
	P1->SEL1 &= ~0x60; /* P1.5, P1.6 for UCB0CLK, UCB0SIMO */
	P2->DIR |= 8;	   /* P2.3 set as output for slave select */
	P2->OUT |= 8;	   /* slave select idle high */
}

// precompute sine wave values
void computeSineValues(void)
{
	double f_val;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		f_val = Vmax * sin(Omega * i);
		data[i] = (unsigned int)(f_val + 0.5); // rounding
	}
}

// simple delay function
void myDelay(void)
{
	volatile int j = 0;
	for (j = 0; j < delayCycles; j++)
		;
}

// main function
int main(void)
{
	int index = 0;

	// initialization
	Init_SPI();
	uart0_init();
	computeSineValues();

	while (1)
	{
		DAC_Write(data[index]);

		sprintf(str, "data = %d\n\r", data[index]);
		uart0_put(str);

		if (++index >= NUM_SAMPLES)
			index = 0;

		myDelay(); // adjusted to generate 1 Hz sine wave
	}
}
