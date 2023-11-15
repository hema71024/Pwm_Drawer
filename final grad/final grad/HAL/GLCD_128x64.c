/*
 * GLCD_128x64.c
 *
 * Created: 12/9/2022 00:54:35
 *  Author: Alaa Attia
 */ 



#include "GLCD_128x64.h"
#include <util/delay.h>


uint8 Current_X = 0;
uint8 Current_Y = 0;
uint8 Current_Page = 0;

void GLCD_Init(void) {



	/* Initialize Port Directions */
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_RS_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_RW_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_OUTPUT);

	DIO_SetPortDirection(GLCD_DATA_PORT, DIO_OUTPUT);
	/* Keep reset pin inactive (high) */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_HIGH);
	/* Initialization of GLCD Sequence */
	GLCD_SelectPage(GLCD_BOTH_SIDES);
	GLCD_SendInstruction(GLCD_DISP_OFF);
	GLCD_SendInstruction(GLCD_SETX + 0);  /* line=0 */
	Current_X = 0;
	GLCD_SendInstruction(GLCD_SETY + 0);  /* column=0 */
	Current_Y = 0;
	GLCD_SendInstruction(GLCD_SETZ + 0);  /* start_line=0 */
	GLCD_SendInstruction(GLCD_DISP_ON);
	GLCD_SelectPage(GLCD_PAGE_0);
	Current_Page = 0;
}

void GLCD_SelectPage(GLCD_SelectSide Side) {
	switch (Side) {
		case GLCD_NO_SIDES:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_LOW);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_LOW);
		break;
		case GLCD_PAGE_0:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_HIGH);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_LOW);
		break;
		case GLCD_PAGE_1:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_LOW);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_HIGH);
		break;
		case GLCD_BOTH_SIDES:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_HIGH);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_HIGH);
		break;
		default: break;
	}
}

void GLCD_SendInstruction(GLCD_CommandWord Instruction) {
	/* Select Instruction Register */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RS_PIN, GLCD_INSTR_REG);
	/* Select Write Operation */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RW_PIN, GLCD_WMODE);
	/* Send Command on Data Pins */
	DIO_WritePort(GLCD_DATA_PORT, Instruction);
	/* Enable Pulse of min. width 1us */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_HIGH);
	_delay_us(5);
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_LOW);
}

void GLCD_SendData(uint8 Data) {
	/* Select Data Register */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RS_PIN, GLCD_DATA_REG);
	/* Select Write Operation */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RW_PIN, GLCD_WMODE);
	/* Send Command on Data Pins */
	DIO_WritePort(GLCD_DATA_PORT, Data);
	/* Enable Pulse of min. width 1us */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_HIGH);
	_delay_us(5);
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_LOW);
}

void GLCD_Reset(void) {
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_LOW);
	_delay_us(10);

	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_HIGH);
}

void GLCD_DisplaySpecialPattern(uint64 Pattern) {
	for(uint8 i = 0; i < 5; i++) {
		GLCD_SendInstruction(GLCD_SETX + Current_X);
		GLCD_SendInstruction(GLCD_SETY + Current_Y);
		GLCD_SendData((Pattern>>(i*8U)) & 0x00000000FFUL);
		Current_Y++;
		if (Current_Y >= GLCD_MAX_PAGE_PIXEL_WIDTH && Current_Page == 0) {
			GLCD_SelectPage(GLCD_PAGE_1);
			Current_Y = 0;
		}
	}
}

void GLCD_DisplayCharacter(char Data) {
	if ((Current_Page == 0) | (Current_Y < GLCD_MAX_PAGE_PIXEL_WIDTH - 7U)) {     // Check that there's sufficient space to add a character
		
		switch (Data) {
			case 'A': case 'a': GLCD_DisplaySpecialPattern(GLCD_CHAR_A); break;
			case 'F': case 'f': GLCD_DisplaySpecialPattern(GLCD_CHAR_F); break;
			case 'R': case 'r': GLCD_DisplaySpecialPattern(GLCD_CHAR_R); break;
			case 'E': case 'e': GLCD_DisplaySpecialPattern(GLCD_CHAR_E); break;
			case 'Q': case 'q': GLCD_DisplaySpecialPattern(GLCD_CHAR_Q); break;
			case 'U': case 'u': GLCD_DisplaySpecialPattern(GLCD_CHAR_U); break;
			case 'N': case 'n': GLCD_DisplaySpecialPattern(GLCD_CHAR_N); break;
			case 'C': case 'c': GLCD_DisplaySpecialPattern(GLCD_CHAR_C); break;
			case 'Y': case 'y': GLCD_DisplaySpecialPattern(GLCD_CHAR_Y); break;
			case 'D': case 'd': GLCD_DisplaySpecialPattern(GLCD_CHAR_D); break;
			case 'T': case 't': GLCD_DisplaySpecialPattern(GLCD_CHAR_T); break;
			case 'K': case 'k': GLCD_DisplaySpecialPattern(GLCD_CHAR_K); break;
			case 'H': case 'h': GLCD_DisplaySpecialPattern(GLCD_CHAR_H); break;
			case 'Z': case 'z': GLCD_DisplaySpecialPattern(GLCD_CHAR_Z); break;
			case 'M': case 'm': GLCD_DisplaySpecialPattern(GLCD_CHAR_M); break;
			case 'S': case 's': GLCD_DisplaySpecialPattern(GLCD_CHAR_S); break;
			case 'I': case 'i': GLCD_DisplaySpecialPattern(GLCD_CHAR_I); break;
			case '%': GLCD_DisplaySpecialPattern(GLCD_CHAR_PERCENT); break;
			case '=': GLCD_DisplaySpecialPattern(GLCD_CHAR_EQUAL); break;
			case '.': GLCD_DisplaySpecialPattern(GLCD_CHAR_POINT); break;
			case ' ': GLCD_DisplaySpecialPattern(GLCD_CHAR_SPACE); break;
			case '-': GLCD_DisplaySpecialPattern(GLCD_CHAR_NEGATIVE); break;
			case '0': GLCD_DisplaySpecialPattern(GLCD_CHAR_0); break;
			case '1': GLCD_DisplaySpecialPattern(GLCD_CHAR_1); break;
			case '2': GLCD_DisplaySpecialPattern(GLCD_CHAR_2); break;
			case '3': GLCD_DisplaySpecialPattern(GLCD_CHAR_3); break;
			case '4': GLCD_DisplaySpecialPattern(GLCD_CHAR_4); break;
			case '5': GLCD_DisplaySpecialPattern(GLCD_CHAR_5); break;
			case '6': GLCD_DisplaySpecialPattern(GLCD_CHAR_6); break;
			case '7': GLCD_DisplaySpecialPattern(GLCD_CHAR_7); break;
			case '8': GLCD_DisplaySpecialPattern(GLCD_CHAR_8); break;
			case '9': GLCD_DisplaySpecialPattern(GLCD_CHAR_9); break;
			default: break;
		}
		GLCD_SendInstruction(GLCD_SETX + Current_X);
		GLCD_SendInstruction(GLCD_SETY + Current_Y);
		GLCD_SendData(0);
		Current_Y++;
		if (Current_Y >= GLCD_MAX_PAGE_PIXEL_WIDTH && Current_Page == 0) {
			GLCD_SelectPage(GLCD_PAGE_1);
			Current_Page = 0;
		}
	}
}

void GLCD_DisplayString(char* Data_Ptr) {
	while (*Data_Ptr != '\0') {
		GLCD_DisplayCharacter(*Data_Ptr);
		Data_Ptr++;
	}
}

void GLCD_DisplayInteger(sint32 Data) {
	if (Data < 0) {
		GLCD_DisplayCharacter('-');
		Data = -1 * Data;
	}
	uint8 u8Local_Digit = 0U;
	char str_reversed[5] = "";
	char* str_ptr = str_reversed;
	do {
		u8Local_Digit = Data%10;
		*str_ptr = (char)(48 + u8Local_Digit); // Ascii of Zero + the digit to get the Ascii of that digits
		Data = Data/10;
		str_ptr++;
	} while (Data > 0);
	do {
		str_ptr--;
		GLCD_DisplayCharacter(*str_ptr);
	} while (str_ptr != str_reversed);
}

void GLCD_DisplayFloat(float32 Data) {
	/* Will display only 3 Decimal Places */
	// Separate integer from decimal to be able to represent the floating point number
	uint16 IntegerVal = (uint16)Data;
	GLCD_DisplayInteger(IntegerVal);
	GLCD_DisplayCharacter('.');
	float32 DecimalPart = (Data - IntegerVal);
	uint8 Digit = 0;
	for (uint8 i = 0; i < 3; i++) {
		Digit = (uint8)(DecimalPart * 10.0);
		GLCD_DisplayCharacter((char)(48+Digit));
		DecimalPart = ((DecimalPart*10.0) - Digit);
	}
}

void GLCD_GoToLine(uint8 Line) {
	if (Line < GLCD_MAX_LINES) {
		GLCD_SelectPage(GLCD_PAGE_0);
		Current_X = Line;
		GLCD_SendInstruction(GLCD_SETX + Current_X);
		Current_Y = 0;
		GLCD_SendInstruction(GLCD_SETY + Current_Y);
	}
}

void GLCD_ClearDisplay(void) {
	for (uint8 Line = 0; Line < GLCD_MAX_LINES; Line++) {
		GLCD_GoToLine(Line);
		GLCD_SelectPage(GLCD_BOTH_SIDES);
		for (uint8 i = 0; i < GLCD_MAX_PAGE_PIXEL_WIDTH; i++) {
			GLCD_SendInstruction(GLCD_SETY + i);
			GLCD_SendData(0);
		}
	}
	GLCD_Reset();
	GLCD_SelectPage(GLCD_PAGE_0);
	Current_Page = 0;
	GLCD_GoToLine(0);
}
