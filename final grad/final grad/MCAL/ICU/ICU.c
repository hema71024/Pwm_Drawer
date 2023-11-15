
#include "ICU.h"
#include "../EXTRA/BIT.h"
#include "../EXTRA/ATmega32A_Config.h"
#include "../EXTRA/Std_Types.h"

const ICU_Config ICU_SetUp =
{
	ICU_TIMER_1,
	ICU_PRESCALE_8,
	ICU_TRIGGER_FALLING_EDGE,
	ICU_INTERRUPT_DISABLED
};



/************************************************************************/
/*					Functions Implementations                           */
/************************************************************************/
void ICU_Init(const ICU_Config* Config_Ptr) {
	/* Set ICP1 Pin (on PD6) as Input to trigger the ICU */
	CLEAR_BIT(*PORTD_DIR_REG, 6U);
	/* Clear Configurations on TCCR1A and TCCR1B */
	*TIMER1_CTRL_REG_A = 0x00U;
	*TIMER1_CTRL_REG_B = 0x00U;
	/* Timer 1 mode is set to normal */
	/* Set Prescaler */
	*TIMER1_CTRL_REG_B |= (Config_Ptr->Clk_Prescale);
	/* Set Trigger Edge (Falling or Rising) */
	*TIMER1_CTRL_REG_B |= ((Config_Ptr->Trigger_Edge)<<ICU_TIMER1_EDGE_SELECT_BIT);
	/* Set Interrupt State */
	*TIMERS_INT_MASK_REG |= ((Config_Ptr->Int_State)<<ICU_TIMER1_INT_BIT);
	/* Clear Timer 1 and ICR Values */
	*(uint16*)TIMER1_CNTR_REG_L = 0x0000U;
	*(uint16*)TIMER1_INP_CAPT_REG_L = 0x0000U;
}

void ICU_SetTriggerEdge(ICU_TimerChannelNo Timer_Chnl, ICU_TriggerEdgeType Edge) {
	CLEAR_BIT(*TIMER1_CTRL_REG_B, ICU_TIMER1_EDGE_SELECT_BIT);
    *TIMER1_CTRL_REG_B |= (Edge<<ICU_TIMER1_EDGE_SELECT_BIT);

}

void ICU_ClearTimerValue(ICU_TimerChannelNo Timer_Chnl) {
	
	*(uint16*)TIMER1_CNTR_REG_L = 0x0000U;
		
	
}



uint16 ICU_GetICValue(ICU_TimerChannelNo Timer_Chnl) {
	uint16 u16Local_Value = 0U;
    u16Local_Value = (*(uint16*)TIMER1_INP_CAPT_REG_L);

	
	return u16Local_Value;
}



ICU_StatusType ICU_GetStatus(ICU_TimerChannelNo Timer_Chnl) {
	
		if (READ_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_INT_BIT) == 1) {
			SET_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_INT_BIT);  /* Clear Flag */
			return ICU_EVENT_CATURED;
		}
		
		else {
			return ICU_RUNNING;
		}
	
	return ICU_RUNNING;
}




void ICU_GetDutyCycle(ICU_TimerChannelNo Timer_Chnl, uint16 Prescale_Value, ICU_DutyCycleType* Duty_Ptr) {
/* Clear Input Capture Flag for safety */
	SET_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_INT_BIT);
	ICU_SetTriggerEdge(ICU_TIMER_1, ICU_TRIGGER_RISING_EDGE);
	/* Wait for the start of the period at rising edge */
	while (ICU_GetStatus(ICU_TIMER_1) == ICU_RUNNING) {
		; // Do Nothing
	}

	uint32 u32Local_ON_Counts = 0U;
	ICU_SetTriggerEdge(ICU_TIMER_1, ICU_TRIGGER_FALLING_EDGE);
	ICU_ClearTimerValue(ICU_TIMER_1);
	/* Clear Input Capture Flag for safety */
	SET_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_INT_BIT);
	/* Wait for the end of the high time at falling edge */
	while (ICU_GetStatus(ICU_TIMER_1) == ICU_RUNNING) {
		if (READ_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_OVF_FLAG_BIT) == 1) {
			SET_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_OVF_FLAG_BIT);
			u32Local_ON_Counts += ICU_TIMER1_RESOLUTION;
		}
	}
	u32Local_ON_Counts += ICU_GetICValue(ICU_TIMER_1);

	uint32 u32Local_OFF_Counts = 0U;
	ICU_SetTriggerEdge(ICU_TIMER_1, ICU_TRIGGER_RISING_EDGE);
	ICU_ClearTimerValue(ICU_TIMER_1);
	/* Clear Input Capture Flag for safety */
	SET_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_INT_BIT);
	/* Wait for the end of the period at rising edge */
	while (ICU_GetStatus(ICU_TIMER_1) == ICU_RUNNING) {
		if (READ_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_OVF_FLAG_BIT) == 1) {
			SET_BIT(*TIMERS_INT_FLAG_REG, ICU_TIMER1_OVF_FLAG_BIT);
			u32Local_OFF_Counts += ICU_TIMER1_RESOLUTION;
		}
	}
	u32Local_OFF_Counts = ICU_GetICValue(ICU_TIMER_1);

	Duty_Ptr->High_Time = (float32)(u32Local_ON_Counts * Prescale_Value * 1000.0 / F_CPU);
	Duty_Ptr->Period_Time = (float32)((u32Local_ON_Counts+u32Local_OFF_Counts) * Prescale_Value * 1000.0 / F_CPU);
}




