#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "buttons.h"
#include "events.h"
#include "uart_if.h"
//----------------------------------------------------------------------------------------------------
static const  State 
 	Button_Undefined[],
	Button_None[],
	Button1_Pressed[],
	Button1_Holded[];

struct Struct_Buttons_State 
{
 unsigned char Button1_Counter;
};

static struct Struct_Buttons_State Status;
const State *Buttons_Sm;
//----------------------------------------------------------------------------------------------------
void Init_Buttons(void)
{
 	Buttons_Sm=Button_Undefined;
	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA2,	PRCM_RUN_MODE_CLK); 			//habilito clk para este periferico y espero 3 clks antes de acceder
	MAP_GPIODirModeSet		(GPIOA2_BASE, 	0x01, 		GPIO_DIR_MODE_IN);	//que SI sea entrada
	MAP_PinTypeGPIO 		(PIN_07, 	PIN_MODE_0, 	false);			//pin 07<>gpio 16 que NO sea salida
    	HWREG(0x4402E0E0)=HWREG(0x4402E0E0)|0x00000100;						//pullup gpio16
}

const State** 	Buttons		(void) 			{return &Buttons_Sm;}
void 		Buttons_Rti	(void) 			{Atomic_Send_Event(ANY_Event,Buttons());}
//----------------------------------------------------------------------------------------------------
unsigned char Read_Button1(void)	{return MAP_GPIOPinRead(GPIOA2_BASE,0x01);}

void Read_Buttons(void)
{
	unsigned int Buttons_Event=Button_None_Event;
	if(!Read_Button1())	Buttons_Event|=Button1_Event;
	Atomic_Send_Event(Buttons_Event,Buttons());
}
//----------------------------------------------------------------------------------------------------
//-----BUTTON1------
void Button1_Press(void)
{
	Status.Button1_Counter=0; 
//	Atomic_Send_Event(Button1_Pressed_Event,Spi_Session());
	DBG_PRINT("Button1 Press\r\n");
}
void Button1_Hold(void)
{
	if(Status.Button1_Counter>4) 
   	{
		DBG_PRINT("Button1 Hold\r\n");
//		Atomic_Send_Event(Button1_Holded_Event,Buttons());
//		Atomic_Send_Event(Button1_Holded_Event,Spi_Session());
		Status.Button1_Counter=0; 
	}	
	else Status.Button1_Counter++;
}
void Button1_Holding(void)
{
	if(++Status.Button1_Counter>4) 
   	{
//	 Atomic_Send_Event(Button1_Holding_Event,Spi_Session());
	 Status.Button1_Counter=0;
	}
}
void Button1_Release(void)
{
	DBG_PRINT("Button1 Released\r\n");
//	Atomic_Send_Event(Button1_Released_Event,Spi_Session());
}
//----------------------------------------------------------------------------------------------------
static const State Button_Undefined[] =
{
 Button_None_Event              ,Rien                           ,Button_None,
 ANY_Event                      ,Rien                           ,Button_Undefined,
};
static const State Button_None[] =
{
 Button1_Event			,Button1_Press			,Button1_Pressed,
 ANY_Event                      ,Rien			 	,Button_None,
};
//---BUTTON1-----
static const State Button1_Pressed[] =
{
 Button1_Event			,Button1_Hold			,Button1_Pressed,
 Button1_Holded_Event		,Rien				,Button1_Holded,
 Button_None_Event		,Button1_Release 		,Button_None,
 ANY_Event                      ,Rien			 	,Button1_Pressed,
};
static const State Button1_Holded[] =
{
 Button1_Event			,Button1_Holding 		,Button1_Holded,
 Button_None_Event		,Rien 		 		,Button_None,
 ANY_Event                      ,Rien			 	,Button1_Holded,
};
