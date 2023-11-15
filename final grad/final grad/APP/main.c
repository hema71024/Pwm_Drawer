
#include "main.h"

	
const PWM_Config PWM_Setup[1] =
{
	{
		PWM_TIMER_0_CHANNEL,
		PWM_FAST,
		PWM_PRESCALE_8,   // You can change the prescaler to test for different PWM Waves
		PWM_INVERTED,
		256U,
		4U,
		30U    // You can change the Duty cycle value from here to check for different cycle percentages
	}
};


void PWMDrawer_Init(void) {

		GLCD_Init();
		GLCD_ClearDisplay();
		PWM_Init(&PWM_Setup[0]);		// Use Timer 0 to generate PWM on OC0=PB3 
		ICU_Init(&ICU_SetUp);			// Timer 1 is used for Input Capture to get the PWM Signal 
}

void PWM_Measure(PWM_Signal_SetUp* PWM_ptr) {
	ICU_DutyCycleType Duty_Cycle_EXM = {0, 0};
	ICU_GetDutyCycle(ICU_TIMER_1, 8U, &Duty_Cycle_EXM);
	PWM_ptr->PeriodT_ms = Duty_Cycle_EXM.Period_Time;
	PWM_ptr->HighT_ms = Duty_Cycle_EXM.High_Time;
	
	PWM_ptr->Freq_kHz = (1.0 / (PWM_ptr->PeriodT_ms));
	PWM_ptr->Duty_Percent = (uint8)((PWM_ptr->HighT_ms) * 100.0 / (PWM_ptr->PeriodT_ms));
}

void PWM_DrawWave(PWM_Signal_SetUp* Signal_Data_Ptr, float32 Scale_ms) {
	GLCD_ClearDisplay(); //Clear anything on the display at first
	
	
	                   /* Display  Frequency on Line 0 and Display Duty cycle percentage on line 2 */
					   
					   
	GLCD_GoToLine(0);
	char FreqKHz[15] = "Freq=\0";
	GLCD_DisplayString(FreqKHz);
	GLCD_DisplayFloat(Signal_Data_Ptr->Freq_kHz);
	char KHz[5] = " kHz\0";
	GLCD_DisplayString(KHz);

	GLCD_GoToLine(2);
	char Duty_Val[15] = "Duty=\0";
	GLCD_DisplayString(Duty_Val);
	GLCD_DisplayInteger(Signal_Data_Ptr->Duty_Percent);
	char Percent[5] = " %\0";
	GLCD_DisplayString(Percent);




	                                /* Display Time in ms on Line 4 */
									
	GLCD_GoToLine(4);
	char Time_in_ms[15] = "Time=\0";
	GLCD_DisplayString(Time_in_ms);
	GLCD_DisplayFloat(Signal_Data_Ptr->PeriodT_ms);
	char milisecs[5] = "ms\0";
	GLCD_DisplayString(milisecs);
	
	
	                                /* Display PWM Square wave from Line 6 */
						   
						   
	GLCD_GoToLine(6);
	uint8 pixel_width_count = 0;
	uint8 i;
	while (pixel_width_count < GLCD_MAX_LINE_PIXEL_WIDTH - 8U) {
		GLCD_DisplaySpecialPattern(PWM_RISING_EDGE_GLCD); pixel_width_count += 5;
		for (i = 0; i < ((uint8)(Signal_Data_Ptr->HighT_ms / Scale_ms)); i++) {
			GLCD_DisplaySpecialPattern(PWM_RISE_GLCD); pixel_width_count += 5;
		}
		GLCD_DisplaySpecialPattern(PWM_FALLING_EDGE_GLCD); pixel_width_count += 5;
		for (i = 0; i < ((uint8)(Signal_Data_Ptr->Low_Time_ms / Scale_ms)); i++) {
			GLCD_DisplaySpecialPattern(PWM_FALL_GLCD); pixel_width_count += 5;
		}
	}

}
