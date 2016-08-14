#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "buttons.h"
#include "spi_session.h"
#include "events.h"
#include "uart_if.h"
//----------------------------------------------------------------------------------------------------
static const  State 
 	Button_Undefined[],
	Button_None[],
	Button1_Pressed[],
	Button1_Holded[],
	Button2_Pressed[],
	Button2_Holded[],
	Button3_Pressed[],
	Button3_Holded[];

struct Struct_Buttons_State 
{
 unsigned char Button1_Counter;
 unsigned char Button2_Counter;
 unsigned char Button3_Counter;
};

static struct Struct_Buttons_State Status;
const State *Buttons_Sm;
//----------------------------------------------------------------------------------------------------
void Init_Buttons(void)
{
 	Buttons_Sm=Button_Undefined;
	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA1,	PRCM_RUN_MODE_CLK); 			//habilito clk para este periferico

	MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x02, 		GPIO_DIR_MODE_IN);	//0x02 es la posicion de puerto , de 0 a 7 dentro de esa base GPIOA1
	MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x04, 		GPIO_DIR_MODE_IN);	//0x02 es la posicion de puerto , de 0 a 7 dentro de esa base GPIOA1
	MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x08, 		GPIO_DIR_MODE_IN);	//0x02 es la posicion de puerto , de 0 a 7 dentro de esa base GPIOA1
	MAP_PinTypeGPIO 		(PIN_64, 	PIN_MODE_0, 	false);			//pin 15 se refiere al pin del IC. Es el GPIO22 en la datasheet
	MAP_PinTypeGPIO 		(PIN_01, 	PIN_MODE_0, 	false);			//pin 15 se refiere al pin del IC. Es el GPIO22 en la datasheet
	MAP_PinTypeGPIO 		(PIN_02, 	PIN_MODE_0, 	false);			//pin 15 se refiere al pin del IC. Es el GPIO22 en la datasheet

    	HWREG(0x4402E0C4)=HWREG(0x4402E0C4)|0x00000100;
    	HWREG(0x4402E0C8)=HWREG(0x4402E0C8)|0x00000100;
    	HWREG(0x4402E0CC)=HWREG(0x4402E0CC)|0x00000100;
}

const State** 	Buttons		(void) 			{return &Buttons_Sm;}
void 		Buttons_Rti	(void) 			{Atomic_Send_Event(ANY_Event,Buttons());}
//----------------------------------------------------------------------------------------------------
unsigned char Read_Button1(void)	{return MAP_GPIOPinRead(GPIOA1_BASE,0x02);}
unsigned char Read_Button2(void)	{return MAP_GPIOPinRead(GPIOA1_BASE,0x04);}
unsigned char Read_Button3(void)	{return MAP_GPIOPinRead(GPIOA1_BASE,0x08);}

void Read_Buttons(void)
{
	unsigned int Buttons_Event=Button_None_Event;
	if(!Read_Button1())	Buttons_Event|=Button1_Event;
	if(!Read_Button2())	Buttons_Event|=Button2_Event;
	if(!Read_Button3())	Buttons_Event|=Button3_Event;
	Atomic_Send_Event(Buttons_Event,Buttons());
}
//----------------------------------------------------------------------------------------------------
//-----BUTTON1------
void Button1_Press(void)
{
	Status.Button1_Counter=0; 
	Atomic_Send_Event(Button1_Pressed_Event,Spi_Session());
	DBG_PRINT("Button1 Press\r\n");
}
void Button1_Hold(void)
{
	if(Status.Button1_Counter>4) 
   	{
		DBG_PRINT("Button1 Hold\r\n");
		Atomic_Send_Event(Button1_Holded_Event,Buttons());
		Atomic_Send_Event(Button1_Holded_Event,Spi_Session());
		Status.Button1_Counter=0; 
	}	
	else Status.Button1_Counter++;
}
void Button1_Holding(void)
{
	if(++Status.Button1_Counter>4) 
   	{
	 Atomic_Send_Event(Button1_Holding_Event,Spi_Session());
	 Status.Button1_Counter=0;
	}
}
void Button1_Release(void)
{
	DBG_PRINT("Button1 Released\r\n");
	Atomic_Send_Event(Button1_Released_Event,Spi_Session());
}
//-----BUTTON2------
void Button2_Press(void)
{
	Status.Button2_Counter=0; 
	Atomic_Send_Event(Button2_Pressed_Event,Spi_Session());
	DBG_PRINT("Button2 Press\r\n");
}
void Button2_Hold(void)
{
	if(Status.Button2_Counter>6) 
   	{
		DBG_PRINT("Button2 Hold\r\n");
		Atomic_Send_Event(Button2_Holded_Event,Buttons());
		Atomic_Send_Event(Button2_Holded_Event,Spi_Session());
		Status.Button2_Counter=0; 
	}	
	else Status.Button2_Counter++;
}
void Button2_Holding(void)
{
	if(++Status.Button2_Counter>4) 
   	{
	 Atomic_Send_Event(Button2_Holding_Event,Spi_Session());
	 Status.Button2_Counter=0;
	}
}
void Button2_Release(void)
{
	DBG_PRINT("Button2 Released\r\n");
	Atomic_Send_Event(Button2_Released_Event,Spi_Session());
}
void Button2_Trick(void)
{
 Atomic_Send_Event(Button2_Trick_Event,Spi_Session());
}
//-----BUtton3------
void Button3_Press(void)
{
	Status.Button3_Counter=0; 
	Atomic_Send_Event(Button3_Pressed_Event,Spi_Session());
	DBG_PRINT("Button3 Press\r\n");
}
void Button3_Hold(void)
{
	if(Status.Button3_Counter>6) 
   	{
		DBG_PRINT("Button3 Hold\r\n");
		Atomic_Send_Event(Button3_Holded_Event,Buttons());
		Atomic_Send_Event(Button3_Holded_Event,Spi_Session());
		Status.Button3_Counter=0; 
	}	
	else Status.Button3_Counter++;
}
void Button3_Holding(void)
{
	if(++Status.Button3_Counter>4) 
   	{
	 Atomic_Send_Event(Button3_Holding_Event,Spi_Session());
	 Status.Button3_Counter=0;
	}
}
void Button3_Release(void)
{
	DBG_PRINT("Button3 Released\r\n");
	Atomic_Send_Event(Button3_Released_Event,Spi_Session());
}
void Button3_Holded_Released(void)
{
	 Atomic_Send_Event(Button3_Holded_Released_Event,Spi_Session());
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
 Button2_Event			,Button2_Press			,Button2_Pressed,
 Button3_Event			,Button3_Press			,Button3_Pressed,
 ANY_Event                      ,Rien			 	,Button_None,
};
//---BUTTON1-----
static const State Button1_Pressed[] =
{
 Button1_Event			,Button1_Hold			,Button1_Pressed,
 Button1_Holded_Event		,Rien				,Button1_Holded,
 Button_None_Event		,Button1_Release 		,Button_None,
 Button2_Event			,Button2_Trick			,Button_Undefined, //lo uso para entrar en modo wifi DCI
 ANY_Event                      ,Rien			 	,Button1_Pressed,
};
static const State Button1_Holded[] =
{
 Button1_Event			,Button1_Holding 		,Button1_Holded,
 Button_None_Event		,Rien 		 		,Button_None,
 ANY_Event                      ,Rien			 	,Button1_Holded,
};
//---BUtton2-----
static const State Button2_Pressed[] =
{
 Button2_Event			,Button2_Hold			,Button2_Pressed,
 Button2_Holded_Event		,Rien				,Button2_Holded,
 Button_None_Event		,Button2_Release 		,Button_None,
 ANY_Event                      ,Rien			 	,Button2_Pressed,
};
static const State Button2_Holded[] =
{
 Button2_Event			,Button2_Holding 		,Button2_Holded,
 Button_None_Event		,Rien 		 		,Button_None,
 ANY_Event                      ,Rien			 	,Button2_Holded,
};
//---BUtton3-----
static const State Button3_Pressed[] =
{
 Button3_Event			,Button3_Hold			,Button3_Pressed,
 Button3_Holded_Event		,Rien				,Button3_Holded,
 Button_None_Event		,Button3_Release 		,Button_None,
 ANY_Event                      ,Rien			 	,Button3_Pressed,
};
static const State Button3_Holded[] =
{
 Button3_Event			,Button3_Holding 		,Button3_Holded,
 Button_None_Event		,Button3_Holded_Released	,Button_None,
 ANY_Event                      ,Rien			 	,Button3_Holded,
};
