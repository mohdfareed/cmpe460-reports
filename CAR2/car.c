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

#define ERROR_HISTORY_LENGTH (10)

#define STRAIGHT_PATH_THRESHOLD (0.8)
#define STRAIGHT_PATH_SPEED_FACTOR (1.0)

#define CURVED_PATH_THRESHOLD (2)
#define CURVED_PATH_SPEED_FACTOR (1.0)

#define IntegralErrorHistoryLength (3)
int integralErrorHistoryIndex = 0;
int integralErrorHistory[IntegralErrorHistoryLength];
double previousError = 0;
double previousError2 = 0;

double errorHistory[ERROR_HISTORY_LENGTH];
int errorHistoryIndex = 0;

//static char str[100];

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

    // configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~BIT1;
	P1->SEL1 &= ~BIT1;
	// configure as input
	P1->DIR &= ~BIT1;
	P1->REN |= BIT1;
	P1->OUT |= BIT1;  
}

int Switch_Pressed(void)
{ 
    if ( P1->IN & BIT4 && P1->IN & BIT1){ // if nothing pressed
        return 0;
    }
    else if (P1->IN & BIT4){    // if switch 1.1 pressed
        return 2;
    }
    else {                      // if switch 1.4 pressed
        return 1;
    }
}

void updateErrorHistory(double error) {
    errorHistory[errorHistoryIndex] = error;
    errorHistoryIndex = (errorHistoryIndex + 1) % ERROR_HISTORY_LENGTH;
}

int main(void)
{
    for (int i = 0; i <= IntegralErrorHistoryLength; i++){
        integralErrorHistory[i] = 0;
    }
	int i = 0;
    Switch_Init();

    double kp = -0.0048;
    double ki = 0.0017;
    double kd = 0.0011;

    int switchVal = 0;
    while (1){
        if (Switch_Pressed() == 1){
            switchVal = 1;
            break;
        }
        else if (Switch_Pressed() == 2) {
            switchVal = 2;
            break;
        }
    }

	// initializations
	DisableInterrupts();
	uart0_init();

	INIT_Motors();

	INIT_Camera();
	EnableInterrupts();

	while (1){
		if (g_sendData == TRUE)
		{
            double sum = 0.0;
            double totalLight = 0.0;
            double weighted_sum = 0.0;
            double integral = 0;

			for (i = 0; i < 128; i++){
                totalLight += line[i];
                if (line[i] > 16380){
                    sum += 1.0;
                    weighted_sum += i;
                }
			}
            double midpoint = weighted_sum / sum;
            double error = midpoint - 64.5;
            integralErrorHistory[integralErrorHistoryIndex] = error;
            integralErrorHistoryIndex = (integralErrorHistoryIndex + 1)%IntegralErrorHistoryLength;

			updateErrorHistory(error);

            double derivative = (error - 2*previousError + previousError2);
            previousError2 = previousError;
            previousError = error;

            for (i = 0; i <= IntegralErrorHistoryLength; i++){
                integral += integralErrorHistory[i];
            }
            integral = integral / IntegralErrorHistoryLength;

            double servoVal;
            servoVal = kp*error + 0.075;
            servoVal += ki*integral + kd*derivative;
            if (servoVal < 0.05) servoVal = 0.05;
            else if (servoVal > 0.1) servoVal = 0.1;
            TIMER_A2_PWM_DutyCycle(servoVal, 1);

            if (totalLight < 1750000){
                TIMER_A0_PWM_DutyCycle(0.0, LEFT_MOTOR_FORWARD);
                TIMER_A0_PWM_DutyCycle(0.0, RIGHT_MOTOR_FORWARD);
            }
            else if (switchVal == 1) {
                if (servoVal >= 0.075) servoVal -= 0.075;
                else servoVal = 0.075 - servoVal;
                double motorSpeed = -18.0*servoVal + 0.434;
                if (motorSpeed < 0.36) motorSpeed = 0.36;

				double sum = 0.0;
				for (int i = 0; i < ERROR_HISTORY_LENGTH; i++) {
					if (errorHistory[i] < 0) errorHistory[i]*=-1;
					sum += errorHistory[i];
				}
				if ((sum / ERROR_HISTORY_LENGTH) < STRAIGHT_PATH_THRESHOLD) {
					motorSpeed *= STRAIGHT_PATH_SPEED_FACTOR;
				} else if ((sum/ERROR_HISTORY_LENGTH) > CURVED_PATH_THRESHOLD) {
					motorSpeed *= CURVED_PATH_SPEED_FACTOR;
				}

                TIMER_A0_PWM_DutyCycle(motorSpeed, LEFT_MOTOR_FORWARD);
                TIMER_A0_PWM_DutyCycle(motorSpeed, RIGHT_MOTOR_FORWARD);
            }
            
            else {
                TIMER_A0_PWM_DutyCycle(0.36, LEFT_MOTOR_FORWARD);
                TIMER_A0_PWM_DutyCycle(0.36, RIGHT_MOTOR_FORWARD);
            }

			g_sendData = FALSE;
		}
	}
}
