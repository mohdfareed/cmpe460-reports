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

// static variables
static char str[100];

// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
    char str[30];
    unsigned int num = ADC_In();

    // calculate beat per minute

    if (num >= VOLTAGE_CUTOFF && previous < VOLTAGE_CUTOFF)
    {
        double bpm = 7200 / count;
        int abs = bpm - prev_bpm;
        if (abs < 0)
            abs = abs * -1;
        count = 0;
        if (bpm > 40 && bpm < 220 && abs < 11)
        {
            // print bpm
            sprintf(str, "Heart %d BPM\r\n", (int)bpm);
            uart0_put(str);
        }
        prev_bpm = bpm;
    }
    else
    {
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
