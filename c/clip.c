#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "clip.h"
#include "spi_session.h"
#include "uart_if.h"
#include "schedule.h"
#include "events.h"
#include "buttons.h"
#include "spi_phisical.h"
#include "parameters.h"

//----------------------------------------------------------------------------------------------------
unsigned char Clip_State,Care_Clip;
//----------------------------------------------------------------------------------------------------
void Init_Clip(void)
{
	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA3,	PRCM_RUN_MODE_CLK); 
	MAP_GPIODirModeSet		(GPIOA3_BASE, 	0x20, 		GPIO_DIR_MODE_IN);
	MAP_PinTypeGPIO 		(PIN_20, 	PIN_MODE_0, 	false);	
    	HWREG(0x4402E114)=HWREG(0x4402E114)|0x00000100; 			//pullup
 	Clip_State=1;
#ifdef HARD_V1			//se podria leer el clip, pero el hard es otro no me la quiero complicar...
 	Care_Clip=0; 		//NO me importa el clip
#else
 	Care_Clip=1; 		//me importa el clip
 	New_Periodic_Func_Schedule(27,Test_Clip);
#endif	
}
void 		Test_Clip	(void) 	
{
	Clip_State=MAP_GPIOPinRead(GPIOA3_BASE,0x20)==0x00 || Care_Clip==0; ///si es cero esta enganchado... sino no.
	Atomic_Send_Event(Clip_State?Clip_On_Event:Clip_Off_Event,Spi_Phisical());	
	DBG_PRINT("Clip State=%d\r\n",Clip_State);	
}
unsigned char 	Read_Clip_State	(void) 	
{
	return Clip_State;	
}
void 	Dont_Care_Clip	(void) 		{Care_Clip=0;}
void 	Do_Care_Clip	(void) 		{Care_Clip=1;}
//----------------------------------------------------------------------------------------------------

