/*
 * 
 *
 * Author:
 * Created:
 * Modified:
 * LJBeato
 * 2021
 */

#include "msp.h"
#include "uart.h"
#include "TimerA.h"

#define DC1_FORWARD (1)
#define DC1_REVERSE (2)
#define DC1_ENABLE (BIT6)

#define DC2_FORWARD (3)
#define DC2_REVERSE (4)
#define DC2_ENABLE (BIT7)

#define SERVO 1

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

/**
 * Initialize the car motors
 */
void Init_Car_Motors(void)
{
	// P3.6/7 must be in GPIO mode and set low
	P3->SEL0 &= ~(DC1_ENABLE | DC2_ENABLE);
	P3->SEL1 &= ~(DC1_ENABLE | DC2_ENABLE);
	P3->DIR |= (DC1_ENABLE | DC2_ENABLE);
	P3->OUT &= ~(DC1_ENABLE | DC2_ENABLE);

	// DC Motor 1 PWM
	TIMER_A0_PWM_Init(300, 0.0, DC1_FORWARD); // period = 300 cycles -> 10kHz
	TIMER_A0_PWM_Init(300, 0.0, DC1_REVERSE);
	// DC Motor 2 PWM
	TIMER_A0_PWM_Init(300, 0.0, DC2_FORWARD); // period = 300 cycles -> 10kHz
	TIMER_A0_PWM_Init(300, 0.0, DC2_REVERSE);
	// Servo PWM
	TIMER_A2_PWM_Init(60000, 0.075, SERVO); // 60000 cycle period -> 50Hz

	// set P3.6/7 high
	P3->OUT |= (DC1_ENABLE | DC2_ENABLE);
}

int main(void)
{
    
    // Motor1 Enable P3.6
    P3->SEL0 &= ~BIT6;
    P3->SEL1 &= ~BIT6;
    P3->DIR |= BIT6;
    P3->OUT &= ~BIT6;
    
    P3->SEL0 &= ~BIT7;
    P3->SEL1 &= ~BIT7;
    P3->DIR |= BIT7;
    P3->OUT &=  ~BIT7;

    delay(5000);

    P3->OUT |= BIT6;
    P3->OUT |= BIT7;
    
    TIMER_A0_PWM_Init(300, 0.0, 1);
    TIMER_A0_PWM_Init(300, 0.0, 2);
    TIMER_A0_PWM_Init(300, 0.0, 3);
    TIMER_A0_PWM_Init(300, 0.0, 4);
    TIMER_A2_PWM_Init(60000, 0.075, 1);
    
	for (;;)
	{   
        delay(2000);
        TIMER_A0_PWM_DutyCycle(0.0, 4);
        TIMER_A0_PWM_DutyCycle(0.3, 1);
        delay(2000);
        TIMER_A0_PWM_DutyCycle(0.0, 1);
        TIMER_A0_PWM_DutyCycle(0.3, 2);
        delay(2000);
        TIMER_A0_PWM_DutyCycle(0.0, 2);
        TIMER_A0_PWM_DutyCycle(0.3, 3);
        delay(2000);
        TIMER_A0_PWM_DutyCycle(0.0, 3);
        TIMER_A0_PWM_DutyCycle(0.3, 4);
        delay(2000);
        TIMER_A0_PWM_DutyCycle(0.0, 4);
        delay(2000);
        TIMER_A2_PWM_DutyCycle(0.05, 1);
        delay(2000);
        TIMER_A2_PWM_DutyCycle(0.1, 1);
        delay(2000);
        TIMER_A2_PWM_DutyCycle(0.075, 1);
	}
	return 0;
}
