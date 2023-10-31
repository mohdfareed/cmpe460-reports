#include "Common.h"

#define RED 1      // P2.0 RED LED
#define GREEN 2    // P2.1 GREEN LED
#define BLUE 4     // P2.2 BLUE LED
#define CYAN 6
#define MAGENTA 5
#define YELLOW 3
#define WHITE 7

#define LED1 1

void LED1_Init(void);
void LED2_Init(void);
void LED2_Set(unsigned int color);
void LED1_Off(void);
void LED1_On(void);
