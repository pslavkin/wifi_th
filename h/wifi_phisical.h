#ifndef  WIFI_PHISICAL
#define  WIFI_PHISICAL

#include "state_machine.h"
//----------------------------------------------------------------------------------------------------
enum Wifi_Phisical_Event_Code	{
					Wlan_Disconnected_Event 	= 0x0B00,
					Turn_Wifi_On_Event 		= 0x0B01,
					Turn_Wifi_Off_Event 		= 0x0B02
		           	};
//----------------------------------------------------------------------------------------------------
#define SECURITY_TYPE       SL_SEC_TYPE_WPA  	//SL_SEC_TYPE_OPEN	/* Security type (OPEN or WEP or WPA*/
#define SSID_LEN_MAX        16
#define BSSID_LEN_MAX       14

extern void 		Wifi_Phisical_Rti	(void);
extern void 		Init_Wifi_Phisical	(void);
extern void 		Clear_SSID_BSSID	(void);
extern const State** 	Wifi_Phisical		(void);
//----------------------------------------------------------------------------------------------------
extern void 		Turn_On_Wifi		(void); 
extern void 		Turn_Off_Wifi		(void); 
//----------------------------------------------------------------------------------------------------
extern void 		Set_Ssid_Name 		(unsigned char *Value,unsigned char Length);
//----------------------------------------------------------------------------------------------------
extern void 		Set_Security_Key	(unsigned char *Value,unsigned char Length); 
extern void 		Connect2Pyj		(void);
//----------------------------------------------------------------------------------------------------
#endif

