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

// static variables
static char str[100]; // string for UART output

// global variables
unsigned int count = 10001; // number of ADC values below voltage cutoff
unsigned int previous = 0;  // previous ADC value
double prev_bpm = -1;       // previous BPM

// Interrupt Service Routine for Timer32-1
void Timer32_1_ISR(void)
{
    char str[30];
    unsigned int num = ADC_In(); // read ADC value

    // if previous ADC value was below voltage cutoff and current ADC
    // value is above voltage cutoff, calculate BPM
    if (num >= VOLTAGE_CUTOFF && previous < VOLTAGE_CUTOFF)
    {
        // calculate BPM and difference from previous BPM
        double bpm = (CAPTURE_FREQ * 60) / count;
        int abs = bpm - prev_bpm;
        if (abs < 0)
            abs = abs * -1;
        count = 0; // reset count
        // if BPM is between 40 and 220 and difference from previous BPM is
        // less than 11 (stops fluctuations), output BPM
        if (bpm > 40 && bpm < 220 && abs < 11)
        {
            sprintf(str, "Heart %d BPM\r\n", (int)bpm);
            uart0_put(str);
        }
        prev_bpm = bpm; // update previous BPM
    }
    else
    {
        count++;
    }               // increment count if under voltage cutoff
    previous = num; // update previous ADC value
}

int main(void)
{
    DisableInterrupts();
    // initialize hardware
    uart0_init();
    ADC0_InitSWTriggerCh6();
    // initialize adc capture timer with capture frequency
    Timer32_1_Init(&Timer32_1_ISR, SystemCoreClock / CAPTURE_FREQ, T32DIV1);
    EnableInterrupts();
    uart0_put("Initialized\r\n");
    while (1)
    {
    }
}
