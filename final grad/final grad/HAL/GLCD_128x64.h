

                                             /* GRAPHICAL LIQUID CRYSTAL DISPLAY (GLCD), Size: 128x64   */
		                                     /*  in X-Axis: 64 pixels (0-63)                            */
		                                     /*  in Y-Axis: 128 Pixels ( 2 Sides divided into 0-63 twice)*/


#ifndef GLCD_128X64_H_
#define GLCD_128X64_H_

#include "../EXTRA/ATmega32A_Config.h"
#include "../MCAL/DIO/DIO.h"

#include "GLCD_Char_Patterns.h"



#define GLCD_MAX_LINES							(8U)
#define GLCD_MAX_LINE_PIXEL_WIDTH				(128U)
#define GLCD_MAX_PAGE_PIXEL_WIDTH				(64U)

#define GLCD_CTRL_PORT							(DIO_PORT_B)
#define GLCD_ENABLE_PIN							(DIO_PIN_0)
#define GLCD_RW_PIN								(DIO_PIN_1)
#define GLCD_RS_PIN								(DIO_PIN_2)
#define GLCD_PAGE_SELECT_PIN_1					(DIO_PIN_4)
#define GLCD_PAGE_SELECT_PIN_2					(DIO_PIN_5)
#define GLCD_RESET_PIN							(DIO_PIN_6)


#define GLCD_DATA_PORT							(DIO_PORT_A)


                      /*			GLCD_128x64 Data types               */
		 
		 
typedef enum {
	GLCD_WMODE,  /* Write Mode for GLCD*/
	GLCD_RMODE   /* Read Mode for GLCD*/
}GLCD_RWType;


typedef enum {
	GLCD_DISP_OFF	      = (0x3EU),  // Turn Off GLCD Display
	GLCD_DISP_ON		  = (0x3FU),  // Turn ON GLCD Display
	GLCD_SETY	          = (0x40U),  //Start address
	GLCD_SETX		      = (0xB8U),  //Page0
	GLCD_SETZ	          = (0xC0U)   // Zeroth Line
}GLCD_CommandWord;

typedef enum {
	GLCD_INSTR_REG,
	GLCD_DATA_REG
}GLCD_RegisterSelectType;

typedef enum {
	GLCD_NO_SIDES,
	GLCD_PAGE_0,
	GLCD_PAGE_1,
	GLCD_BOTH_SIDES
}GLCD_SelectSide;




                      /*			Function Prototypes for GLCD_128x64               */

void GLCD_Init(void);
void GLCD_SelectPage(GLCD_SelectSide Side);
void GLCD_SendInstruction(GLCD_CommandWord Instruction);
void GLCD_SendData(uint8 Data);
void GLCD_Reset(void);
void GLCD_DisplayCharacter(char Data);
void GLCD_DisplayString(char* Data_Ptr);
void GLCD_DisplayInteger(sint32 Data);
void GLCD_DisplayFloat(float32 Data);
void GLCD_ClearDisplay(void);
void GLCD_GoToLine(uint8 Line);
void GLCD_DisplaySpecialPattern(uint64 Pattern);




#endif /* GLCD_128X64_H_ */