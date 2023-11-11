#include "PWM.h"

const PWM_ConfigurationType gaStrPWM_Config[NUM_OPERATING_PWM_CHANNELS] = 
{
	{
		PWM_TIMER_0_CHANNEL,
		PWM_FAST,
		PWM_PRESCALE_64,
		PWM_NON_INVERTED,
		PWM_PRESCALE_64,  // This is the period value, which determines the frequency
		0U,    // Duty cycle (initially set to 25%)
		PWM_PRESCALE_64    // Prescale value
	}

};
