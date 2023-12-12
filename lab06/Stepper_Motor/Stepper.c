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
	// Initialize UART
	uart0_init();
    
    // Initialize GPIO
    P4->SEL0 = 0;
    P4->SEL1 = 0;
    P4->DIR = 30;
    P4->OUT = 0;

	// Print welcome over serial
	uart0_put("Running... \n\r");

    int forward = 1;
    int phase = 0;
    
	for(;;){  //loop forever
		// Turn off all coils
        P4->OUT = 0;
        
        // Set one pin high at a time
        if (forward){
            if (phase == 0){
                P4->OUT = 2;}
            else if (phase == 1){
                P4->OUT = 4;}
            else if (phase == 2){
                P4->OUT = 8;}
            else {
                P4->OUT = 16;}
            phase = (phase+1)%4;
        }
        
        else{
            if (phase == 0){
                P4->OUT = 16;}
            else if (phase == 1){
                P4->OUT = 8;}
            else if (phase == 2){
                P4->OUT = 4;}
            else {
                P4->OUT = 2;}
            phase = (phase+1)%4;
        }
        delay(10);
	}

	return 0;
}
