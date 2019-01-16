#include "hw_types.h"
#include "hw_memmap.h"
#include "uart_if.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "state_machine.h"
#include "events.h"
#include <one_wire_phisical.h>
#include <type_conversion.h>
#include <one_wire_network.h>
#include "str.h"
#include <debug.h>
#include <checksum.h>
#include <one_wire_transport.h>
#include <tmp_control.h>
#include <wifi_socket.h>
#include <schedule.h>

static const State 
	Fail[],
	Above[],
	Inside[],
	Below[];

const State* Tmp_Control_Sm;

static const signed int 	Tmp_Min=2000,	Tmp_Max=3000,	Tmp_Windows=100;
static const unsigned int 	Tmp_Email=0,	Tmp_Relay=0,	Tmp_Sd=0;		//no puede ser char porque el minimo a grabar en flash es un entero...
signed int Windows;
//-------------------------------------------------------
void Test_Tmp(void)	
{
 unsigned int Event=Inside_Event;
#ifdef TWO_WIRE
 if(!Read_Two_Wire_Status()) Event=Tmp_Fail_Event;
  else
   {
    signed int Tmp=SHT71_Bin_T()+Windows;
#else
 if(!On_Line_Nodes() | !Read_One_Wire_Crc(0)) Event=Tmp_Fail_Event;
  else
   {
    signed int Tmp=Read_One_Wire_T(0)+Windows;
#endif
         if(Tmp>Tmp_Max) Event=Above_Event;
    else if(Tmp<Tmp_Min) Event=Below_Event;
   }
 Atomic_Send_Event(Event,Tmp_Control());
}
void Windows2Inside	(void)	{Windows=0;}
void Windows2Above	(void)	{Windows=+Tmp_Windows;}
void Windows2Fail	(void)	{Windows=0;}
void Windows2Below	(void)	{Windows=-Tmp_Windows;}
//-------------------------------------------------------------------------------------
void Save_Tmp_Control_State(unsigned char* Buf)
{
 static unsigned char const* const States[]={"Fail ","Hot  "," Ok  ","Cold "};
 unsigned char State;
      if(Tmp_Control_Sm==Fail)   State=0;
 else if(Tmp_Control_Sm==Above)  State=1;
 else if(Tmp_Control_Sm==Inside) State=2;
 else if(Tmp_Control_Sm==Below)  State=3;
 String_Copy(States[State],Buf,5);
}
void Send_Tmp_Control_State2Tcp(void)
{
 unsigned char State[5];
 Save_Tmp_Control_State(State);
 Send_Data2Socket(State,5);
}
void Tmp_Send_Email		(void)	{}//if(Tmp_Email) 	Send_Email();}
void Tmp_Stamp_Alarm_Log	(void)	{}//if(Tmp_Sd) 	Stamp_Alarm_Log();}
void Tmp_Set_Relay		(void)	{}//if(Tmp_Relay) 	Play_Relay_On_Effect();}
void Tmp_Reset_Relay		(void)	{}//if(Tmp_Relay) 	Cancell_Relay_Effect();}
//-------------------------------------------------------------------------------------
void Save_Max_Tmp4Loaded_Tmp	(void)		{}//Save_Int2Flash((unsigned int*)&Tmp_Max,Signed_2Dec_Fix_Point_Bcd2Signed_Int(Socket_Rx_Buffer()));}
void Save_Min_Tmp4Loaded_Tmp	(void)		{}//Save_Int2Flash((unsigned int*)&Tmp_Min,Signed_2Dec_Fix_Point_Bcd2Signed_Int(Socket_Rx_Buffer()));}
void Save_Windows_Tmp4Loaded_Tmp(void)		{}//Save_Int2Flash((unsigned int*)&Tmp_Windows,Signed_2Dec_Fix_Point_Bcd2Signed_Int(Socket_Rx_Buffer()));}
void Send_Max_Tmp2Tcp		(void)		
{
 unsigned char Buf[7];
 Signed_Int2_2Dec_Fix_Point_Bcd(Buf,Tmp_Max);
 Send_Data2Socket(Buf,7);
}
void Send_Min_Tmp2Tcp		(void)	
{
 unsigned char Buf[7];
 Signed_Int2_2Dec_Fix_Point_Bcd(Buf,Tmp_Min);
 Send_Data2Socket(Buf,7);
}
void Send_Windows_Tmp2Tcp	(void)	
{
 unsigned char Buf[7];
 Signed_Int2_2Dec_Fix_Point_Bcd(Buf,Tmp_Windows);
 Send_Data2Socket(Buf,7);
}
void Send_Enable2Tcp			(void)	{Send_Data2Socket("Enable\r\n",8);}
void Send_Disable2Tcp			(void)	{Send_Data2Socket("Disable\r\n",9);}
void Toogle_Tmp_Email			(void)	{}//Save_Int2Flash(&Tmp_Email,Tmp_Email?0:1);}
void Send_Tmp_Email2Tcp			(void)	{}//Tmp_Email?Send_Enable2Tcp():Send_Disable2Tcp();}
void Toogle_Tmp_Relay			(void)	{}//Save_Int2Flash(&Tmp_Relay,Tmp_Relay?0:1);}
void Send_Tmp_Relay2Tcp			(void)	{}//Tmp_Relay?Send_Enable2Tcp():Send_Disable2Tcp();}
void Toogle_Tmp_Sd			(void)	{}//Save_Int2Flash(&Tmp_Sd,Tmp_Sd?0:1);}
void Send_Tmp_Sd2Tcp			(void)	{}//Tmp_Sd?Send_Enable2Tcp():Send_Disable2Tcp();}
//-------------------------------------------------------------------------------------
void Init_Tmp_Control			(void) 	
{
 Tmp_Control_Sm=Fail;
 Windows=0;
 New_Offset_Periodic_Func_Schedule(TEST_TMP_PERIOD*3*5,TEST_TMP_PERIOD*5,Test_Tmp);	//el qer testeo un poco mas tarde para que de tiempo a levantar el sensor... sino manda un mail de alarma cada vez que prende con FAIL......
}
const State** Tmp_Control		(void) 	{return &Tmp_Control_Sm;}
void 	Tmp_Control_Rti			(void)	{Atomic_Send_Event(ANY_Event,Tmp_Control());}
//-------------------------------------------------------------------------------------
void Set_Alarm				(void)	{Tmp_Set_Relay();Tmp_Send_Email();Tmp_Stamp_Alarm_Log();}
void Reset_Alarm			(void)	{Tmp_Reset_Relay();Tmp_Send_Email();Tmp_Stamp_Alarm_Log();}
void Reset_Alarm_And_Windows2Inside	(void)	{Reset_Alarm();	Windows2Inside();}
void Set_Alarm_And_Windows2Above	(void)	{Set_Alarm();	Windows2Above();}
void Set_Alarm_And_Windows2Below	(void)	{Set_Alarm();	Windows2Below();}
void Set_Alarm_And_Windows2Fail		(void)	{Set_Alarm();	Windows2Fail();}
//-------------------------------------------------------------------------------------
static const State Fail[] =
{
 Inside_Event			,Reset_Alarm_And_Windows2Inside				,Inside,
 Above_Event			,Set_Alarm_And_Windows2Above				,Above,
 Below_Event			,Set_Alarm_And_Windows2Below				,Below,
 Tmp_Fail_Event			,Windows2Fail						,Fail,
 ANY_Event			,Rien							,Fail,
};
static const State Above[] =
{
 Inside_Event			,Reset_Alarm_And_Windows2Inside				,Inside,
 Above_Event			,Windows2Above						,Above,
 Below_Event			,Set_Alarm_And_Windows2Below				,Below,
 Tmp_Fail_Event			,Set_Alarm_And_Windows2Fail				,Fail,
 ANY_Event			,Rien							,Above,
};
static const State Inside[] =
{
 Inside_Event			,Windows2Inside						,Inside,
 Above_Event			,Set_Alarm_And_Windows2Above				,Above,
 Below_Event			,Set_Alarm_And_Windows2Below				,Below,
 Tmp_Fail_Event			,Set_Alarm_And_Windows2Fail				,Fail,
 ANY_Event			,Rien							,Inside,
};
static const State Below[] =
{
 Inside_Event			,Reset_Alarm_And_Windows2Inside				,Inside,
 Above_Event			,Set_Alarm_And_Windows2Above				,Above,
 Below_Event			,Windows2Below						,Below,
 Tmp_Fail_Event			,Set_Alarm_And_Windows2Fail				,Fail,
 ANY_Event			,Rien							,Below,
};
//-------------------------------------------------------------------------------------

