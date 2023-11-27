#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "Timer32.h"

// constants
#define CAPTURE_FREQ (10)    // frequency of ADC data capture
#define VOLTAGE_CUTOFF (0.0) // voltage cutoff for a heartbeat

// global variables
// uint16_t line[128];

// static variables
static char str[100];

/**
 * Waits for a delay (in milliseconds)
 *
 * ms - The delay in milliseconds
 */
void Delay(int ms)
{
    volatile int i;
    for (i = 0; i < ms * 2224; i++) // FIXME: fix value for 48 MHz clock
    {
    } // 2224 for 24 MHz
}

// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
    char str[30];
    unsigned int num = ADC_In();
    double voltage = num * 2.5 / 16384.0;

    // calculate beat per minute
    double bpm = 0.0;
    if (voltage > 0.0)
    {
        bpm = voltage;
    }

    // print bpm
    sprintf(str, "Heart %d BPM\r\n", (int)bpm);
    uart0_put(str);
}

int main(void)
{
    // initializations
    DisableInterrupts();
    uart0_init();
    Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock / CAPTURE_FREQ, T32DIV1);
    ADC0_InitSWTriggerCh6();
    EnableInterrupts();
    uart0_puts("Initialized\r\n");

    while (1)
    {
    }
}
