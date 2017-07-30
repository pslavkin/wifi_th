#ifndef  HTTP_SESSION
#define  HTTP_SESSION

#include "state_machine.h"
//----------------------------------------------------------------------------------------------------
enum Http_Session_Event_Code	{
					Enable_Http_Event		= 0x7C00
		           	};
//----------------------------------------------------------------------------------------------------
extern void 		Http_Session_Rti	(void);
extern void 		Init_Http_Session	(void);
extern const State** 	Http_Session		(void);
//----------------------------------------------------------------------------------------------------
extern const State* 	Http_Session_App	(void);
//----------------------------------------------------------------------------------------------------

#endif

