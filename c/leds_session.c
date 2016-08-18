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

	//Led TR 
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA1, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_64,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x02, 		GPIO_DIR_MODE_OUT);
	//Led TG 
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA1, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_01,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x04, 		GPIO_DIR_MODE_OUT);
	//Led TB 
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA1, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_02,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x08, 		GPIO_DIR_MODE_OUT);
	
	//Led THR 
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA2, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_15,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA2_BASE, 	0x40, 		GPIO_DIR_MODE_OUT);
	//Led THG 
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA3, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_18,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA3_BASE, 	0x10, 		GPIO_DIR_MODE_OUT);
	//Led THG 
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA3, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_20,	PIN_MODE_0, 	false);
    MAP_GPIODirModeSet		(GPIOA3_BASE, 	0x20, 		GPIO_DIR_MODE_OUT);
}
void Led_Run_Off(void)	{MAP_GPIOPinWrite(GPIOA0_BASE,0x80,0x00);}
void Led_Run_On	(void)	{MAP_GPIOPinWrite(GPIOA0_BASE,0x80,0x80);}
void Led_TR_Off(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x02,0x02);}
void Led_TR_On	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x02,0x00);}
void Led_TG_Off(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x04,0x04);}
void Led_TG_On	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x04,0x00);}
void Led_TB_Off(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x08,0x08);}
void Led_TB_On	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x08,0x00);}
void Led_THR_Off(void)	{MAP_GPIOPinWrite(GPIOA2_BASE,0x40,0x40);}
void Led_THR_On	(void)	{MAP_GPIOPinWrite(GPIOA2_BASE,0x40,0x00);}
void Led_THG_Off(void)	{MAP_GPIOPinWrite(GPIOA3_BASE,0x10,0x10);}
void Led_THG_On	(void)	{MAP_GPIOPinWrite(GPIOA3_BASE,0x10,0x00);}
void Led_THB_Off(void)	{MAP_GPIOPinWrite(GPIOA3_BASE,0x20,0x20);}
void Led_THB_On	(void)	{MAP_GPIOPinWrite(GPIOA3_BASE,0x20,0x00);}
//-------------------------------------------------------------------
struct Led_Effect_Struct Led_Effects[]=
{
 {0x8000,0x000A,Led_Run_On	,Led_Run_Off},
 {0x8800,0x800A,Led_TR_On	,Led_TR_Off},
 {0x8080,0x80A0,Led_TG_On	,Led_TG_Off},
 {0x8008,0x8A00,Led_TB_On	,Led_TB_Off},
 {0x8800,0x800A,Led_THR_On	,Led_THR_Off},
 {0x8080,0x80A0,Led_THG_On	,Led_THG_Off},
 {0x8008,0x8A00,Led_THB_On	,Led_THB_Off},
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
