#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"

// sin wave constants
double Vmax = 3.3;
double Omega = 2 * 3.14159 / 100.0;
char str[100];

// write DAC value
void DAC_Write(unsigned int data)
{
	unsigned char byte1, byte2;
	P2->OUT &= ~BIT3; /* assert slave select */

	byte1 = (unsigned char)(0x30) | (data >> 4);
	while (!(EUSCI_B0->IFG & 2))
		; /* wait for transmit buffer empty */

	EUSCI_B0->TXBUF = byte1; /* write to SPI transmit buffer */
	while (EUSCI_B0->STATW & 1)
		; /* wait for transmit done */

	byte2 = (unsigned char)(data << 4);
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
	EUSCI_B0->CTLW0 = 0x0001; /* disable UCB0 during config */

	/* clock phase/polarity:11, MSB first, 8-bit, master, 3-pin SPI,
	synchronous mode, use SMCLK as clock source */
	EUSCI_B0->CTLW0 = 0xE9C1;
	EUSCI_B0->BRW = 3;			/* 3 MHz / 1 = 1 MHz */
	EUSCI_B0->CTLW0 &= ~0x0001; /* enable UCB0 after config */

	P1->SEL0 |= 0x60; /* P1.5, P1.6 for UCB0CLK, UCB0SIMO */
	P1->SEL1 &= ~0x60;
	P2->DIR |= 8; /* P2.3 set as output for slave select */
	P2->OUT |= 8; /* slave select idle high */
}

// simple delay function
void myDelay(void)
{
	volatile int j = 0;
	for (j = 0; j < 100000; j++)
		;
}

/////////////////////////////////////////////////////
//
// main function
//
/////////////////////////////////////////////////////
int main(void)
{
	// initializations
	float f_data;
	unsigned int data;
	int offset;

	Init_SPI();
	uart0_init();

	while (1)
	{
		data = new double[100];
		for (offset = 0; offset < 100; offset++)
		{
			f_data = Vmax * sin(Omega * (float)offset) + 128.0;
			data = (unsigned int)(f_data + .5);
			DAC_Write(data);

			sprintf(str, "data = %d\n\r", data);
			uart0_put(str);
		}
		myDelay();
	}
}
