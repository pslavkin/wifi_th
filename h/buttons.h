#ifndef BUTTONS
#define BUTTONS
#include "state_machine.h"
//------------------------------------------------------
enum Buttons_Event_Code{
				Button_None_Event		= 0xD000,
				Button1_Event			= 0xD001,
				Button2_Event			= 0xD002,
				Button3_Event			= 0xD004,

				Button1_Pressed_Event		= 0xDA00,
				Button1_Holded_Event		= 0xDA01,
				Button1_Holding_Event		= 0xDA02,
				Button1_Released_Event		= 0xDA03,
				Button2_Pressed_Event		= 0xDA04,
				Button2_Holded_Event		= 0xDA05,
				Button2_Holding_Event		= 0xDA06,
				Button2_Released_Event		= 0xDA07,
				Button3_Pressed_Event		= 0xDA08,
				Button3_Holded_Event		= 0xDA09,
				Button3_Holding_Event		= 0xDA0A,
				Button3_Released_Event		= 0xDA0B,
				Button3_Holded_Released_Event	= 0xDA0C,
				Auto_Power_Off_Event  		= 0xDA0D,
				Button2_Trick_Event 		= 0xDA0E
	};
//------------------------------------------------------
extern void 		Init_Buttons		(void);
extern unsigned char 	Read_Button1		(void);
extern unsigned char 	Read_Button2		(void);
extern unsigned char 	Read_Button3		(void);
extern void 		Read_Buttons 		(void);
//------------------------------------------------------
extern const State** 	Buttons			(void);
extern void 		Buttons_Rti		(void);

#endif
