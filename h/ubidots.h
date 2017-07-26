#ifndef  UBIDOTS
#define  UBIDOTS

#include <state_machine.h>
//----------------------------------------------------------------------------------------------------
enum Ubidots_Event_Code	{
					Send_Temp_Hum_Event		= 0x1C00
		           	};
//----------------------------------------------------------------------------------------------------
extern void 		Ubidots_Rti	(void);
extern void 		Init_Ubidots	(void);
extern const State** 	Ubidots		(void);
//----------------------------------------------------------------------------------------------------
extern const State* 	Ubidots_App	(void);
//----------------------------------------------------------------------------------------------------

#endif

