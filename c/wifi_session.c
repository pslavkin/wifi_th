#include "wlan.h"
#include "uart_if.h"
#include "common.h"
#include "wifi_session.h"
#include "wifi_socket.h"
#include "wifi_network.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "type_conversion.h"
#include "buttons.h"
#include "parameters.h"

static const State   
	Welcome[],
	Info[],
	Network[],
	Virtual_Buttons[];

const State* 		Wifi_Session_Sm;
const State* 		Wifi_Session_App(void)	{return Welcome;}
//------------------------------------------------------------------
static void Print_Welcome_State		(void)	{DBG_PRINT("---->Welcome State\n\r");}
//------------------------------------------------------------------
void 		Init_Wifi_Session	(void) 
{ 
 Wifi_Session_Sm=Welcome;
}	
const State** 	Wifi_Session		(void) 			{return &Wifi_Session_Sm;} 
void 		Wifi_Session_Rti	(void)			{Atomic_Send_Event(Rti_Event,Wifi_Session());}
//------------------------------------------------------------------
unsigned char Welcome_Help_Data[]=
{
"www.disenioconingenio.com.ar\r\n"
"Ethergate wifi v1.0\r\n"
"v1.0.0\r\n"
"A Info\r\n"
"B Network\r\n"
"C Buttons\r\n"
"? Help\r\n"
};
unsigned char Info_Help_Data[]=
{
"A Read ID\r\n"
"< Back\r\n"
"? Help\r\n"
};
unsigned char Network_Help_Data[]=
{
"A Read Ip\r\n"
"B Change Ip\r\n"
"C Read Port\r\n"
"D Set Port\r\n"
"E Clients\r\n"
"< Back\r\n"
"? Help\r\n"
};
unsigned char Virtual_Buttons_Help_Data[]=
{
"Buttons\r\n"
"A 1 Pressed\r\n"
"B 1 Holded\r\n"
"C 1 Holding\r\n"
"D 1 Released\r\n"
"< Back\r\n"
"? Help\r\n"
};
//------------WELCOME----------------------------------------------------------------------------------------
void  Welcome_A	(void)	{}
void  Welcome_B	(void)	{}
void  Welcome_C	(void)	{}
//------------ INFO----------------------------------------------------------------------------------------
void  Info_A		(void)	{Send_Data2Socket(SERIAL_ID,SERIAL_ID_LENGTH);}
//------------ NETWORK----------------------------------------------------------------------------------------
void  Network_A		(void)	{Send_Ip2Socket(Read_Actual_Ip());}
void  Network_B		(void)	{}
void  Network_C		(void)	{Send_Int_NLine2Socket(Actual_Port4App());}
void  Network_D		(void)	{}
void  Network_E		(void)	{Send_Char_NLine2Socket(Number_Of_Socket_Opened());}
//------------ Virtual Buttons----------------------------------------------------------------------------------------
void  Virtual_Buttons_A		(void)	{}//Atomic_Send_Event(Button1_Pressed_Event,		Spi_Session());}
void  Virtual_Buttons_B		(void)	{}//Atomic_Send_Event(Button1_Holded_Event,		Spi_Session());}
void  Virtual_Buttons_C		(void)	{}//Atomic_Send_Event(Button1_Holding_Event,		Spi_Session());}
void  Virtual_Buttons_D		(void)	{}//Atomic_Send_Event(Button1_Released_Event,		Spi_Session());}
//----------------------------------------------------------------------------------------------------
void  Welcome_Help		(void)	{Send_Data2Socket(Welcome_Help_Data,		sizeof(Welcome_Help_Data)-1);}
void  Info_Help			(void)	{Send_Data2Socket(Info_Help_Data,		sizeof(Info_Help_Data)-1);}
void  Network_Help		(void)	{Send_Data2Socket(Network_Help_Data,		sizeof(Network_Help_Data)-1);}
void  Virtual_Buttons_Help	(void)	{Send_Data2Socket(Virtual_Buttons_Help_Data,	sizeof(Virtual_Buttons_Help_Data)-1);}
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 'A'				,Welcome_A					,Info,
 'B'				,Welcome_B					,Network,
 'C'				,Welcome_C					,Virtual_Buttons,
 '?'				,Welcome_Help					,Welcome,
 ANY_Event			,Rien						,Welcome,
};
static const State Info[] =
{
 'A'				,Info_A						,Info, 	// Read ID
 '<'				,Rien 						,Welcome,
 '?'				,Info_Help					,Info,
 ANY_Event			,Rien						,Info,
};
static const State Network[] =
{
 'A'				,Network_A					,Network, 	// Read Ip
 'B'				,Network_B					,Network, 	// Change Ip
 'C'				,Network_C					,Network, 	// Read Port Base
 'D'				,Network_D					,Network, 	// Change Port Base
 'E'				,Network_E					,Network, 	// numero de conexiones abiertas, entre clientes y servidores...
 '<'				,Rien 						,Welcome,
 '?'				,Network_Help					,Network,
 ANY_Event			,Rien						,Network,
};
static const State Virtual_Buttons[] =
{
 'A'				,Virtual_Buttons_A					,Virtual_Buttons,
 'B'				,Virtual_Buttons_B					,Virtual_Buttons,
 'C'				,Virtual_Buttons_C					,Virtual_Buttons,
 'D'				,Virtual_Buttons_D					,Virtual_Buttons,
 '<'				,Rien 							,Welcome,
 '?'				,Virtual_Buttons_Help					,Virtual_Buttons,
 ANY_Event			,Rien							,Virtual_Buttons,
};
//----------------------------------------------------------------------------------------------------

