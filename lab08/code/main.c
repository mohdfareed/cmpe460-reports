#include <stdio.h>
#include <stdlib.h>

#include "msp.h"
#include "uart.h"
#include "CortexM.h"
#include "Common.h"
#include "ADC14.h"

// constants
// #define LEFT_MOTOR_FORWARD (2)

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

int main(void)
{
    // initializations
    DisableInterrupts();
    uart0_init();
    EnableInterrupts();
    uart0_puts("Initialized\r\n");

    while (1)
    {
        // main loop
    }
}
