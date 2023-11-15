
#ifndef PWM_H_
#define PWM_H_
#include "../EXTRA/ATmega32A_Config.h"
#include "../EXTRA/Std_Types.h"
#include "../EXTRA/BIT.h"


#define PWM_TIMER_0_FAST_BIT			(3U)
#define PWM_TIMER_0_INVERTING_BIT		(4U)

#define PWM_TIMER_1_FAST_BIT			(3U)
#define PWM_TIMER_1_A_INVERTING_BIT		(6U)
#define PWM_TIMER_1_B_INVERTING_BIT		(4U)

#define PWM_TIMER_2_FAST_BIT			(3U)
#define PWM_TIMER_2_INVERTING_BIT		(4U)




// Channels that could be used:

typedef enum {
	PWM_TIMER_0_CHANNEL,
	PWM_TIMER_1_CHANNEL_A,
	PWM_TIMER_1_CHANNEL_B,
	PWM_TIMER_2_CHANNEL
}PWM_ChannelType;



// Possible PWM Modes
typedef enum {
	PWM_PHASE_CORRECT,
	PWM_FAST
}PWM_ModeType;


//Prescale types
typedef enum {
	PWM_NO_CLK_SRC,
	PWM_PRESCALE_1,
	PWM_PRESCALE_8,
	PWM_PRESCALE_32,
	PWM_PRESCALE_64,
	PWM_PRESCALE_128,
	PWM_PRESCALE_256,
	PWM_PRESCALE_1024,
	PWM_EXT_CLK_FALLING_EDGE,
	PWM_EXT_CLK_RISING_EDGE
}PWM_ClockPrescaleType;

typedef enum {
	PWM_NON_INVERTED,
	PWM_INVERTED
}PWM_OutputState;




typedef struct {
	PWM_ChannelType Timer_Chnl;
	PWM_ModeType Mode;
	PWM_ClockPrescaleType Clk_Prescale;
	PWM_OutputState State;
	uint16 Prescale_Value;
	uint16 Period_ms;
	uint8 Duty_Percent;
}PWM_Config;


             /*				 Prototypes for PWM Module                      */

void PWM_Init(const PWM_Config* Config_Ptr);
void PWM_SetDuty(PWM_OutputState State, uint16 Duty_Percent);



extern const PWM_Config PWM_Setup[1];

#endif /* PWM_H_ */