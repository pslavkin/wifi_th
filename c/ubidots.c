#include "wlan.h"
#include "uart_if.h"
#include "common.h"
#include "ubidots.h"
#include "wifi_socket.h"
#include "wifi_network.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "type_conversion.h"
#include "parameters.h"
#include "debug.h"
#include "string.h"

static const State   
	Welcome[];

const State* 		Ubidots_Sm;
const State* 		Ubidots_App(void)	{return Welcome;}
//------------------------------------------------------------------
void 		Init_Ubidots	(void) 
{ 
 Ubidots_Sm=Welcome;
}	
const State** 	Ubidots		(void) 			{return &Ubidots_Sm;} 
void 		Ubidots_Rti	(void)			{Atomic_Send_Event(Rti_Event,Ubidots());}
//------------------------------------------------------------------
unsigned char Ubidots_Header[]=
{
"POST /api/v1.6/devices/wifigate HTTP/1.1\r\n"
"Host: things.ubidots.com\r\n"
"X-Auth-Token:z5eOJ2j4ONC8RUUOSgDUETZ5OA1jR4\r\n"
"Content-Type: application/json\r\n"
"Content-Length: 41\r\n"
"\r\n"
};
unsigned char Ubidots_Data[]=
{
"{\"temperatura\": +025.45, \"humedad\" : 123}\r\n"
};
//------------WELCOME----------------------------------------------------------------------------------------
static void  Welcome_A	(void)	
{
	static unsigned char H;
	Temp2_Fix_Point_Bcd(Ubidots_Data+16,0);
	Ubidots_Data[16]=' ';
	Ubidots_Data[17]=' ';
	if(H++>100) H=0;
	Char2Bcd(Ubidots_Data+37,H);
	if(Ubidots_Data[37]=='0') Ubidots_Data[37]=' ';
	if(Ubidots_Data[38]=='0') Ubidots_Data[38]=' ';
	Send_Data2Socket(Ubidots_Header,sizeof(Ubidots_Header)-1);
	Send_Data2Socket(Ubidots_Data,sizeof(Ubidots_Data)-1);
	New_None_Periodic_Schedule(100,Close_Socket_Event,Actual_Sm4App());
}
static void  Welcome_B	(void)	{}
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 Send_Temp_Hum_Event		,Welcome_A					,Welcome,
 'B'				,Welcome_B					,Welcome,
 ANY_Event			,Rien						,Welcome,
};
//----------------------------------------------------------------------------------------------------

