#ifndef  WIFI_NETWORK
#define  WIFI_NETWORK

#include "state_machine.h"
//----------------------------------------------------------------------------------------------------
enum Wifi_Network_Event_Code	{
					Wifi_Network_Null_Event 	= 0x0C01
		           	};
//----------------------------------------------------------------------------------------------------
extern void 		Wifi_Network_Rti	(void);
extern void 		Init_Wifi_Network	(void);
extern const State** 	Wifi_Network		(void);
//----------------------------------------------------
extern unsigned int	Read_Actual_Ip		(void);

#endif

