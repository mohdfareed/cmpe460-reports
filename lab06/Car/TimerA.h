int TIMER_A0_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin);
void TIMER_A0_PWM_DutyCycle(double percentDutyCycle, uint16_t pin);

int TIMER_A2_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin);
void TIMER_A2_PWM_DutyCycle(double percentDutyCycle, uint16_t pin);

void TIMER_A2_PWM_Interrupts(uint16_t period, double percentDutyCycle, void (*handler)(void));
void DisableInterrupts(void);
void EnableInterrupts(void);
