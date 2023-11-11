#include "../lib/ATmega32A_Config.h"
#include "../MCAL/PWM/PWM_Drawer.h"
#include <util/delay.h>
#include <stdio.h>

void PWM_ChangeFrequency(uint16_t prescaler, uint16_t period) {
	printf("Changing PWM frequency...\n");
	
	PWM_ConfigurationType newConfig = {
		.Tmr_Channel = PWM_TIMER_0_CHANNEL,
		.Mode = PWM_FAST,
		.Clk_Prescale = prescaler,
		.State = PWM_NON_INVERTED,
		.Prescale_Value = 64U,
		.Period_ms = period,
		.Duty_Percent = 50U
	};

	PWM_Init(&newConfig);
	printf("PWM frequency changed.\n");
}

int main(void) {
	PWMDrawer_Init();
PWMDrawer_SignalAttributesType Signal_Data = {0, 0, 0, 0, 0};
float32 Scale_ms = 0.25;

// Set the desired duty cycle
uint8_t dutyCycle = 40;  // Set your desired duty cycle (e.g., 50%)

// Set the PWM frequency (you can calculate this based on your desired frequency)
uint16_t newTop = (F_CPU / (PWM_PRESCALE_256 * 1000 * 2));  // Change 2 to your desired frequency
PWM_ChangeFrequency(PWM_PRESCALE_256, newTop);

while (1) {
	// Set the duty cycle and update the PWM signal
	PWM_SetDuty(PWM_TIMER_0_CHANNEL, PWM_NON_INVERTED, dutyCycle);

	// Measure the signal
	PWMDrawer_MeasureSignal(&Signal_Data);

	// Draw the signal
	PWMDrawer_DrawSignal(&Signal_Data, Scale_ms);

	// Delay for a short duration
	_delay_ms(20);
}
}
