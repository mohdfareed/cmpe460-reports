#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"
#include "Timer32.h"

// constants
#define CAPTURE_FREQ (122)    // frequency of ADC data capture
#define VOLTAGE_CUTOFF (5000) // voltage cutoff for a heartbeat

unsigned int count = 10001;
unsigned int previous = 0;
double prev_bpm = -1;

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
    
    // calculate beat per minute

    if (num >= VOLTAGE_CUTOFF && previous < VOLTAGE_CUTOFF){  
        double bpm = 7200/count;
        int abs = bpm - prev_bpm;
        if (abs < 0) abs = abs*-1;
        count = 0;
        if (bpm > 40 && bpm < 220 && abs < 11){
            // print bpm
            sprintf(str, "Heart %d BPM\r\n", (int)bpm);
            uart0_put(str);
        }
        prev_bpm = bpm;
    }
    else {
        count++;
    }
    previous = num;

}

int main(void)
{
    // initializations
    DisableInterrupts();
    uart0_init();
    Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock / CAPTURE_FREQ, T32DIV1);
    ADC0_InitSWTriggerCh6();
    EnableInterrupts();
    uart0_put("Initialized\r\n");

    while (1)
    {
    }
}
