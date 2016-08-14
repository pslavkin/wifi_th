#ifndef  FLASH
#define  FLASH

#include "state_machine.h"
//-----------------------------------------------------------
enum Flash_Event_Code{
				Flash_Null_Event	= 0xD000
		           };
//-----------------------------------------------------------
extern void 		Flash_Rti	(void);
extern void 		Init_Flash	(void);
extern const State** 	Flash		(void);
//---------------------------------------------------------

#endif

