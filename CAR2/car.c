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

#define LEFT_MOTOR_FORWARD 2
#define LEFT_MOTOR_BACKWARD 1
#define RIGHT_MOTOR_FORWARD 4
#define RIGHT_MOTOR_BACKWARD 3

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
	TIMER_A2_PWM_Init(60000, 0.075, 1);

	delay_ms(250);
	P3->OUT |= BIT6;
	P3->OUT |= BIT7;
}

void Switch_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~BIT4;
	P1->SEL1 &= ~BIT4;
	// configure as input
	P1->DIR &= ~BIT4;
	P1->REN |= BIT4;
	P1->OUT |= BIT4;          
}

BOOLEAN Switch1_Pressed(void)
{
	// check if pressed
	if ( P1->IN & BIT4 ) // if not pressed
		return FALSE;
	return TRUE;            // if pressed
}

int main(void)
{
	int i = 0;
    Switch_Init();
    while (!Switch1_Pressed());

	// initializations
	DisableInterrupts();
	uart0_init();

	INIT_Motors();

	INIT_Camera();
	EnableInterrupts();

	while (1)
	{

        
		if (g_sendData == TRUE)
		{
            int sum = 0;
            int weighted_sum = 0;

			for (i = 0; i < 128; i++)
			{
				sum += line[i];
                weighted_sum += i*line[i];
			}
            double midpoint = (double)weighted_sum / (double) sum;
            if (sum < 1750000){
                TIMER_A0_PWM_DutyCycle(0.0, LEFT_MOTOR_FORWARD);
                TIMER_A0_PWM_DutyCycle(0.0, RIGHT_MOTOR_FORWARD);
            }
            else {
                double motorSpeed = sum*0.0000004899135 - 0.6573487031700287;
                TIMER_A0_PWM_DutyCycle(motorSpeed, LEFT_MOTOR_FORWARD);
                TIMER_A0_PWM_DutyCycle(motorSpeed, RIGHT_MOTOR_FORWARD);
            }
            sprintf(str, "%f", midpoint);
            uart0_put(str);
            uart0_put("\r\n");
            
            if (midpoint < 61){
                sprintf(str, "%f", 0.1);
                TIMER_A2_PWM_DutyCycle(0.1, 1);
            }else if (midpoint > 66){
                sprintf(str, "%f", 0.05);
                TIMER_A2_PWM_DutyCycle(0.05, 1);
            } else {
                double servoVal;
                if (midpoint <= 63.5){
                    servoVal = (1.0/250.0)*(midpoint-63.5)*(midpoint-63.5)+0.075;
                }
                else {
                    servoVal = (-1.0/250.0)*(midpoint-63.5)*(midpoint-63.5)+0.075;
                }
                sprintf(str, "%f", servoVal);
                TIMER_A2_PWM_DutyCycle(servoVal, 1);
            }
            uart0_put(str);
            uart0_put("\r\n");
			g_sendData = FALSE;
		}
        delay_ms(1);
	}
}
