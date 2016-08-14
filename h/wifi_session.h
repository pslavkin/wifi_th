#ifndef  WIFI_SESSION
#define  WIFI_SESSION

#include "state_machine.h"
//----------------------------------------------------------------------------------------------------
enum Wifi_Session_Event_Code	{
					Wifi_Session_Null_Event		= 0x0C00
		           	};
//----------------------------------------------------------------------------------------------------
extern void 		Wifi_Session_Rti	(void);
extern void 		Init_Wifi_Session	(void);
extern const State** 	Wifi_Session		(void);
//----------------------------------------------------------------------------------------------------
extern const State* 	Wifi_Session_App	(void);
//----------------------------------------------------------------------------------------------------

#endif

