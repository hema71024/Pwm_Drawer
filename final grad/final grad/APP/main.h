
#ifndef MAIN1_H_
#define MAIN1_H_


#include "../EXTRA/ATmega32A_Config.h"
#include "../EXTRA/Std_Types.h"
#include "../HAL/GLCD_128x64.h"
#include "../MCAL/PWM/PWM.h"
#include "../MCAL/ICU/ICU.h"




            /*				Wave Patterns (Font = 5 x 8)               */

#define PWM_RISE_GLCD					(0x0202020202UL)
#define PWM_FALL_GLCD					(0x4040404040UL)
#define PWM_RISING_EDGE_GLCD					(0x02027E4040UL)
#define PWM_FALLING_EDGE_GLCD				(0x40407E0202UL)





typedef struct {
	uint8 Duty_Percent;
	float32 HighT_ms;
	float32 Low_Time_ms;
	float32 PeriodT_ms;
	float32 Freq_kHz;
}PWM_Signal_SetUp;


                  /*			 Prototypes for main            */

void PWMDrawer_Init(void);
void PWM_Measure(PWM_Signal_SetUp* Signal_Data_Ptr);
void PWM_DrawWave(PWM_Signal_SetUp* Signal_Data_Ptr, float32 Scale_ms);




#endif /* MAIN1_H_ */