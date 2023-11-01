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

static int i = 0;

void Servo_Interrupt(void);

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

void Init_PWM_INTERRUPTS(void)
{
    // generate pwm using timer A2 on P5.2 instead of P5.6
    P5->SEL0 |= BIT2;
    P5->SEL1 &= ~BIT2;
    P5->DIR |= BIT2;
    P5->OUT &= ~BIT2;

    TIMER_A2_PWM_Interrupts(60000, 0.075, &Servo_Interrupt);
}

void Servo_Interrupt(void)
{
    // toggle P5.2 to generate pwm
	
    P5->OUT ^= BIT2;
//	  i = P5->OUT & BIT2;
//		if (i)
//		{
//			uart0_putchar('|');
//		}
//		else
//		{
//			uart0_putchar('_');
//		}
}

int main(void)
{
	  uart0_init();
	  uart0_put("Starting\r\n");
	
    // Motor1 Enable P3.6
    P3->SEL0 &= ~BIT6;
    P3->SEL1 &= ~BIT6;
    P3->DIR |= BIT6;
    P3->OUT &= ~BIT6;

    P3->SEL0 &= ~BIT7;
    P3->SEL1 &= ~BIT7;
    P3->DIR |= BIT7;
    P3->OUT &= ~BIT7;
    
    //Init_PWM_INTERRUPTS();
		TIMER_A0_PWM_Init(300, 0.0, 1);
    TIMER_A0_PWM_Init(300, 0.0, 2);
    TIMER_A0_PWM_Init(300, 0.0, 3);
    TIMER_A0_PWM_Init(300, 0.0, 4);
	  TIMER_A2_PWM_Init(60000, 0.0, 1);
	
		delay(2000);

    P3->OUT |= BIT6;
    P3->OUT |= BIT7;
		
    //EnableInterrupts();
		//uart0_put("Interrupt enabled\r\n");

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
			
			uart0_put("Controlling Servo\r\n");
        delay(2000);
        TIMER_A2_PWM_DutyCycle(0.05, 1);
        delay(2000);
        TIMER_A2_PWM_DutyCycle(0.1, 1);
        delay(2000);
        TIMER_A2_PWM_DutyCycle(0.075, 1);
    }
}
