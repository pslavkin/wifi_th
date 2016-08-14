#include "adc.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "rom_map.h"
#include "pin.h"
#include "bat.h"
#include "schedule.h"
#include "buttons.h"
#include "events.h"
#include "state_machine.h"
#include "spi_session.h"
#include "../driverlib/adc.h"
#include "uart_if.h"
#include "common.h"
#include "gpio.h"
#include "prcm.h"
#include "spi_phisical.h"
#include "events.h"
#include "parameters.h"
#include "debug.h"
#include "leds_session.h"

unsigned char 		Bat_Level,Charger_Status,Care_Charger;
unsigned short int 	Charger_Level;
//-------------------------------------------------------------
void 	Dont_Care_Charger	(void) 		{Care_Charger=0;}
void 	Do_Care_Charger		(void) 		{Care_Charger=1;}
//-------------------------------------------------------------
void 		Init_Charger	(void)	
{
 	Charger_Level=0;
#ifndef HARD_V1						//en la version 1 el charger es un GPIO no un ADC.. asi que lo vuelo para estos 5 protos V1 y no registro si enchufan o no el cargador.. lo dejo en cero y ya..2
 	New_Periodic_Func_Schedule(21,Test_Charger);
#endif
}
void 		Test_Charger	(void) 	
{
	Charger_Level=Care_Charger?((Charger_Level+Read_Adc(ADC_CH_2))/2):0;
	Atomic_Send_Event(Is_Charger_Connected()?Charger_Connected_Event:Charger_Disconnected_Event,Spi_Session());
	DBG_BAT_PRINT("Cargador conectado? %d\r\n",Is_Charger_Connected());
}
unsigned char 	Is_Charger_Connected	(void) 	
{
	return Charger_Level>CHARGER_THRESHOLD;
}
//-------------------------------------------------------------
void 		Init_Charger_Status	(void)	
{
 	Charger_Status=0;
	Care_Charger=1;
	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA0,	PRCM_RUN_MODE_CLK); 
	MAP_GPIODirModeSet		(GPIOA0_BASE, 	0x01, 		GPIO_DIR_MODE_IN);
	MAP_PinTypeGPIO 		(PIN_50, 	PIN_MODE_0, 	false);	
    	HWREG(0x4402E0A0)=HWREG(0x4402E0A0)|0x00000100; 			//pullup
#ifndef HARD_V1						//en la version 1 el charger status esta en otro pin.. lo vuelo y considero que NO esta enchufado
 	New_Periodic_Func_Schedule(23,Test_Charger_Status);
#endif
}
void 		Test_Charger_Status	(void) 	
{
	Charger_Status=MAP_GPIOPinRead(GPIOA0_BASE,0x01)==0x00;
	DBG_BAT_PRINT("Cargando? %d\r\n",Charger_Status);	
}
unsigned char	Is_Bat_Charging	(void) 	
{
	return Charger_Status;
}

//-------------------------------------------------------------
void 		Init_Bat	(void)	
{
 	Bat_Level=50;
#ifndef HARD_V1						//en la version 1 no testeo bata, considero que siempre esta al 50 y chau...
 	New_Periodic_Func_Schedule(9,Test_Bat);
#endif
}
void 		Test_Bat	(void) 	
{
	unsigned int Value=Read_Adc(ADC_CH_1);
	unsigned char New_Value;
	DBG_BAT_PRINT("Testing Bat %d\r\n",Value);	
		if(Value<RAW_BAT_MIN) New_Value=0;
	else 	if(Value>RAW_BAT_MAX) New_Value=100;
	else 	New_Value=((Value-RAW_BAT_MIN)*100)/(RAW_BAT_MAX-RAW_BAT_MIN);
	if(New_Value>Bat_Level) Bat_Level+=Bat_Level<100?1:0;
	else 			Bat_Level-=Bat_Level>0  ?1:0;
}
unsigned char 	Read_Bat_Level	(void) 	
{
	return Bat_Level;	
}
//-------------------------------------------------------------

