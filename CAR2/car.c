#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "ControlPins.h"
#include "TimerA.h"

// constants
#define LEFT_MOTOR_FORWARD (2)
#define LEFT_MOTOR_BACKWARD (1)
#define RIGHT_MOTOR_FORWARD (4)
#define RIGHT_MOTOR_BACKWARD (3)
#define SERVO (1)

// global variables
uint16_t line[128];
BOOLEAN g_sendData;

// static variables
static char str[100];
static int i;
static int sum;
static int weighted_sum;
static double midpoint;
static double motorSpeed;
static double steeringAngle;

// function prototypes
void Navigate(void);

/**
 * Drives the car at a given speed and direction
 *
 * speed - The speed to drive at in range [-1.0, 1.0], where 1.0 is
 *         full speed forward and -1.0 is full speed backward
 */
void Drive(double speed)
{
	if (speed > 0.0)
	{
		TIMER_A0_PWM_DutyCycle(0.0, LEFT_MOTOR_BACKWARD);
		TIMER_A0_PWM_DutyCycle(0.0, RIGHT_MOTOR_BACKWARD);

		TIMER_A0_PWM_DutyCycle(speed, LEFT_MOTOR_FORWARD);
		TIMER_A0_PWM_DutyCycle(speed, RIGHT_MOTOR_FORWARD);
	}
	else
	{
		TIMER_A0_PWM_DutyCycle(0.0, LEFT_MOTOR_FORWARD);
		TIMER_A0_PWM_DutyCycle(0.0, RIGHT_MOTOR_FORWARD);

		TIMER_A0_PWM_DutyCycle(speed, LEFT_MOTOR_BACKWARD);
		TIMER_A0_PWM_DutyCycle(speed, RIGHT_MOTOR_BACKWARD);
	}
}

/**
 * Steers the car at a given angle
 *
 * angle - The angle to steer at in range [0.05, 0.1], where 0.05 is full
 * 	       left and 0.1 is full right
 */
void Steer(double angle)
{
	TIMER_A2_PWM_DutyCycle(angle, SERVO);
}

/**
 * Checks if switch 1 is pressed
 *
 * Returns TRUE if pressed, FALSE otherwise
 */
BOOLEAN Switch1Pressed(void)
{
	// check if pressed
	if (P1->IN & BIT4) // if not pressed
		return FALSE;
	return TRUE; // if pressed
}

/**
 * Waits for a delay (in milliseconds)
 *
 * ms - The delay in milliseconds
 */
void Delay(int ms)
{
	volatile int i;
	for (i = 0; i < ms * 2224; i++)
	{
	} // 2224 for 24 MHz
}

void InitSwitch(void)
{
	P1->SEL0 &= ~BIT4;
	P1->SEL1 &= ~BIT4;
	P1->DIR &= ~BIT4;
	P1->REN |= BIT4;
	P1->OUT |= BIT4;
}

void InitCamera(void)
{
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

void InitMotors(void)
{
	P3->SEL0 &= ~BIT6;
	P3->SEL1 &= ~BIT6;
	P3->DIR |= BIT6;
	P3->OUT &= ~BIT6;

	P3->SEL0 &= ~BIT7;
	P3->SEL1 &= ~BIT7;
	P3->DIR |= BIT7;
	P3->OUT &= ~BIT7;

	TIMER_A0_PWM_Init(2400, 0.0, 1);
	TIMER_A0_PWM_Init(2400, 0.0, 2);
	TIMER_A0_PWM_Init(2400, 0.0, 3);
	TIMER_A0_PWM_Init(2400, 0.0, 4);
	TIMER_A2_PWM_Init(60000, 0.0, 1);

	Delay(250);
	P3->OUT |= BIT6;
	P3->OUT |= BIT7;
}

int main(void)
{
	// wait for start signal
	InitSwitch();
	while (!Switch1Pressed())
		;

	// initializations
	DisableInterrupts();
	uart0_init(); // for debugging
	InitMotors();
	InitCamera();
	EnableInterrupts();
	uart0_puts("Initialized\r\n");

	while (1)
	{
		// wait for line data
		if (g_sendData == TRUE)
		{
			// navigate and wait for next frame
			Navigate();
			g_sendData = FALSE;
		}
		Delay(1);
	}
}

void Navigate(void)
{
	// calculate line characteristics
	sum = 0;
	weighted_sum = 0;
	for (i = 0; i < 128; i++)
	{
		sum += line[i];
		weighted_sum += i * line[i];
	}
	midpoint = (double)weighted_sum / (double)sum;

	// drive the car
	if (sum < 1800000)
	{
		Drive(0.0);
	}
	else
	{
		motorSpeed = sum * 0.000000572 - 0.83030303;
		Drive(motorSpeed);
	}

	// steer the car
	if (midpoint < 60)
	{
		Steer(1.0);
	}
	else if (midpoint > 67)
	{
		Steer(-1.0);
	}
	else
	{
		steeringAngle = -0.00714285 * midpoint + 0.528571;
		Steer(steeringAngle);
	}
}
