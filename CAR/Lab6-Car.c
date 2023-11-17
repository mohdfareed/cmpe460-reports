/*
 *
 */

#include "msp.h"
#include "uart.h"
// #include "Timer32.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "ControlPins.h"
#include "TimerA.h"
#include "SysTickTimer.h"

#define DC1_FORWARD (1)
#define DC1_REVERSE (2)
#define DC1_ENABLE (BIT6)

#define DC2_FORWARD (3)
#define DC2_REVERSE (4)
#define DC2_ENABLE (BIT7)

#define SERVO 1

static int i = 0;

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

// ADC_In() gets the latest value from the ADC
// ADC will be P4.7 A6
// SI Pin will be P5.5 A0
// CLK Pin will be P5.4 A1

// static long pixelCounter = 0;

uint16_t line[128];
BOOLEAN g_sendData;

static char str[100];

void INIT_Camera(void)
{
    g_sendData = FALSE;
    ControlPin_SI_Init();
    ControlPin_CLK_Init();
    ADC0_InitSWTriggerCh6();
}

int main(void)
{
    DisableInterrupts();
    uart0_init();
    uart0_put("Starting\r\n");
    
    //delay_ms(4000);

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
    TIMER_A2_PWM_Init(60000, 0.0, 1);

    //delay_ms(2000);

    P3->OUT |= BIT6;
    P3->OUT |= BIT7;

    INIT_Camera();
    EnableInterrupts();

    uart0_put("yeehaw\r\n");

    for (;;)
    {
//        delay_ms(2000);
//        TIMER_A0_PWM_DutyCycle(0.0, 4);
//        TIMER_A0_PWM_DutyCycle(0.3, 1);
        //        delay_ms(2000);
        //        TIMER_A0_PWM_DutyCycle(0.0, 1);
        //        TIMER_A0_PWM_DutyCycle(0.3, 2);
        //        delay_ms(2000);
        //        TIMER_A0_PWM_DutyCycle(0.0, 2);
        //        TIMER_A0_PWM_DutyCycle(0.3, 3);
        //        delay_ms(2000);
        //        TIMER_A0_PWM_DutyCycle(0.0, 3);
        //        TIMER_A0_PWM_DutyCycle(0.3, 4);
        //        delay_ms(2000);
        //        TIMER_A0_PWM_DutyCycle(0.0, 4);

//        uart0_put("Controlling Servo\r\n");
//        delay_ms(2000);
//        TIMER_A2_PWM_DutyCycle(0.05, 1);
//        delay_ms(2000);
//        TIMER_A2_PWM_DutyCycle(0.1, 1);
//        delay_ms(2000);
//        TIMER_A2_PWM_DutyCycle(0.075, 1);

        SI_Handler();
        uart0_put("looping...\n\r");
        delay_ms(1000);
        if (g_sendData == TRUE)
        {
            // send the array over uart
            sprintf(str, "%i\n\r", -1); // start value
            uart0_put(str);

//            for (i = 0; i < 128; i++)
//            {
//                sprintf(str, "%i\n\r", line[i]);
//                uart0_put(str);
//            }

            sprintf(str, "%i\n\r", -2); // end value
            uart0_put(str);
            g_sendData = FALSE;
        }
        
        delay_ms(1000);

        // milestone 1
        // read line of data
        // filter line to find edges
        // compare distance from center to edges
        // decide which way to turn and amount
        // turn wheels and adjust speed
    }
}