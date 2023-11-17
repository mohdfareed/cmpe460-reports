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

// function prototypes
void Navigate(void);

/**
 * Drives the car at a given speed and direction
 *
 * speed - The speed to drive at (-1.0 - +1.0)
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
 * angle - The angle to steer at (-1.0 - +1.0)
 */
void Steer(double angle)
{
	// map angle to duty cycle (0.05 - 0.1)
	TIMER_A2_PWM_DutyCycle(0.05 + (0.05 * angle), SERVO);
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
	// initializations
	Delay(5000); // startup delay
	DisableInterrupts();
	uart0_init(); // for debugging
	InitMotors();
	InitCamera();
	EnableInterrupts();

	// startup car
	uart0_put("starting motors...\r\n");
	Drive(0.35);

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
		Drive(0.23);
	}

	// steer the car
	if (midpoint < 61)
	{
		Steer(1.0);
	}
	else if (midpoint > 66)
	{
		Steer(-1.0);
	}
	else
	{
		// FIXME: use Steer()
		double servoVal = -0.01 * midpoint + 0.71;
		TIMER_A2_PWM_DutyCycle(servoVal, 1);
	}
}