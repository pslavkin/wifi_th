#include "everythings.h"
#include "buttons.h"
#include "leds_session.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "common.h"
#include "flash.h"
#include "uart_if.h"
#include "nonos.h"
#include "parameters.h"
#include "one_wire_transport.h"

const State   
        Free_State1[],
	Free_State2[],
	Free_State3[],
	Free_State4[],
	Free_State5[],
	Free_State6[],
	Free_State7[],
	Free_State8[],
	Free_State9[],
	Free_State10[];

const State* Everythings_Sm;				//variable que lleva cuenta del estado de la maquina de estados de "detodo un poco"...
//------------------------------------------------------------------
void 		Init_Everythings	(void) 			
{ 
 Everythings_Sm=Free_State1;
 Init_Schedule(); 
 Init_Buttons();
 Init_Leds_Session();
 Init_Flash();
 Init_One_Wire_Transport();
}	
const State** 	Everythings		(void) 			{return &Everythings_Sm;} 			//devuelve la direccion de la maquina de estados Everythings para poder mandarle mensajes.
void 		Everythings_Rti		(void)			{Send_Event(ANY_Event,Everythings());}		//manda mensajes ANY a tiempos predefinidos...
//----------------------------------------------------------------------------------------------------
void 		Simple_Link_Main_Loop	(void) 			{_SlNonOsMainLoopTask();}
//----------------------------------------------------------------------------------------------------
const State Free_State1[] =
{
 ANY_Event			,Rien 					,Free_State2,
};
const State Free_State2[] =
{
 ANY_Event			,Rien					,Free_State3,
};
const State Free_State3[] =
{
 ANY_Event			,Leds_Session_Rti			,Free_State4,
};
const State Free_State4[] =
{
 ANY_Event			,Rien 					,Free_State5,
};
const State Free_State5[] =
{
 ANY_Event			,Schedule 				,Free_State6,
};
const State Free_State6[] =
{
 ANY_Event			,Rien		 			,Free_State7,
};
const State Free_State7[] =
{
 ANY_Event			,Rien					,Free_State8,
};
const State Free_State8[] =
{
 ANY_Event			,Read_Buttons 				,Free_State9,
};
const State Free_State9[] =
{
 ANY_Event			,Simple_Link_Main_Loop 			,Free_State10,
};
const State Free_State10[] =
{
 ANY_Event			,Rien					,Free_State1,
};
//-------------------------------------------------------------------------------
