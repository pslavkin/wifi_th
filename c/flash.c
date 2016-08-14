#include "hw_types.h"/*{{{*/
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "flash.h"
#include "state_machine.h"
#include "events.h"
#include "buttons.h"
#include "simplelink.h"
#include "schedule.h"
#include "uart_if.h"/*}}}*/
static const  State 
	Off[];

const State 	*Flash_Sm;
long 		*lFileHandle;
unsigned long 	*ulToken;
//-------------------------------------------------------------------
void Create_File(void)
{
	long lRetVal = -1;
	UART_PRINT("grabando file\r\n");
	lRetVal = sl_FsOpen("a",FS_MODE_OPEN_CREATE(65536,_FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE), ulToken, lFileHandle);
	UART_PRINT("paso por aca1\r\n");
	if(lRetVal < 0) {
		lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);										// File may already be created
		UART_PRINT("err1\r\n");
	}
	else {
        	lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
        	if (SL_RET_CODE_OK != lRetVal) UART_PRINT("err2\r\n");
	}

	lRetVal = sl_FsOpen("a", FS_MODE_OPEN_WRITE, ulToken, lFileHandle);
	if(lRetVal < 0) {
		lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
		UART_PRINT("err3\r\n");
	}
	lRetVal = sl_FsWrite(*lFileHandle,0,"hola estoy grabado\r\n", 20);
	UART_PRINT("Grabo\r\n");
	if (lRetVal < 0) {
	        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
		UART_PRINT("err4\r\n");
	}

	lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
	UART_PRINT("Cierro file\r\n");
	if (SL_RET_CODE_OK != lRetVal) {
	UART_PRINT("err5\r\n");
	}
}

//-------------------------------------------------------------------
void Init_Flash(void)
{
 Flash_Sm=Off;
// New_None_Periodic_Func_Schedule(100,Create_File);
}
const State** 	Flash		(void) 			{return &Flash_Sm;}
void 		Flash_Rti	(void) 			{Atomic_Send_Event(ANY_Event,Flash());}

//-------------------------------------------------------------------
static const State Off[] =
{
 ANY_Event                      ,Rien			 	,Off,
};
//-------------------------------------------------------------------------------
