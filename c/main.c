// Standard includes/*{{{*/
#include <stdlib.h>
#include <string.h>

// simplelink includes 
#include "simplelink.h"
#include "wlan.h"
// driverlib includes 
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "prcm.h"
#include "uart.h"
#include "utils.h"
// common interface includes 
#include "udma_if.h"
#include "uart_if.h"
#include "common.h"
// app
#include "wdog.h"
#include "rti.h"
#include "events.h"
#include "everythings.h"
#include "state_machine.h"
#include "startup_gcc.h"
#include "wifi_phisical.h"
#include "wifi_session.h"
#include "wifi_network.h"
#include "wifi_socket.h"
/*}}}*/

void main()
{
	Init_Vector_Base(); 
	PRCMCC3200MCUInit();	
	UDMAInit();
	InitTerm();
	Init_Events();
	Init_Everythings();
	Init_Rti();
	Init_Wifi_Phisical();
	Init_Wifi_Session();
	Init_Wifi_Network();
	Init_Wifi_Socket();
	Init_Wdog();
	MAP_IntMasterEnable();

    for(;;)
	{
		Wdog_Clear();
		State_Machine();
	}	
}

