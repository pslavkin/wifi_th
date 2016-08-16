#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "leds_session.h"
#include "state_machine.h"
#include "events.h"
#include "buttons.h"

static const  State 
	Off[];

const State *Leds_Session_Sm;
//-------------------------------------------------------------------
void Init_Leds(void)
{	
	//Led amarillo 0805
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA0, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_62,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA0_BASE, 	0x80, 		GPIO_DIR_MODE_OUT);

}
void Led_Run_Off(void)	{MAP_GPIOPinWrite(GPIOA0_BASE,0x80,0x00);}
void Led_Run_On	(void)	{MAP_GPIOPinWrite(GPIOA0_BASE,0x80,0x80);}
//-------------------------------------------------------------------
struct Led_Effect_Struct Led_Effects[]=
{
 {0x8000,0x000A,Led_Run_On	,Led_Run_Off},
};
void Set_Led_Effect		(unsigned char Led,unsigned int Effect)		{Led_Effects[Led].Effect=Led_Effects[Led].Temp_Effect=Effect;}
void Set_Temp_Led_Effect	(unsigned char Led,unsigned int Effect)		{Led_Effects[Led].Temp_Effect=Effect;}
void Set_Toogle_Led_Effect	(unsigned char Led)				{Led_Effects[Led].Temp_Effect&0x01?Led_Effects[Led].Off_Function():Led_Effects[Led].On_Function();}
void Set_Fixed_Led_Effect	(unsigned char Led,unsigned int Effect)		{Led_Effects[Led].Effect=Effect;}
void Led_Effects_Func		(void)										
{
 unsigned char Actual_Led;
 for(Actual_Led=0;Actual_Led<sizeof(Led_Effects)/sizeof(struct Led_Effect_Struct);Actual_Led++)
  {
   (Led_Effects[Actual_Led].Temp_Effect&0x0001)?Led_Effects[Actual_Led].On_Function():Led_Effects[Actual_Led].Off_Function();	//se usa el metodo del shifteop para trasladar la indo de la variable al estado del led...
   if(!(Led_Effects[Actual_Led].Temp_Effect>>=1)) Led_Effects[Actual_Led].Temp_Effect=Led_Effects[Actual_Led].Effect;
  }
}
//------------------------------------------------------------------
void Init_Leds_Session(void)
{
 Leds_Session_Sm=Off;
 Init_Leds();
}
const State** 	Leds_Session		(void) 			{return &Leds_Session_Sm;}
void 		Leds_Session_Rti	(void) 			{Led_Effects_Func();}
//-------------------------------------------------------------------
static const State Off[] =
{
 ANY_Event                      ,Rien			 	,Off,
};
//-------------------------------------------------------------------------------
