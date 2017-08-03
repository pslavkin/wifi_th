#include <string.h>
#include "wlan.h"
#include "uart_if.h"
#include "common.h"
#include "http_session.h"
#include "wifi_session.h"
#include "wifi_socket.h"
#include "wifi_network.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "type_conversion.h"
#include "buttons.h"
#include "parameters.h"
#include "flash.h"
#include "debug.h"
#include "string.h"
#include "one_wire_transport.h"
#include "tmp_control.h"
#include "leds_session.h"

static const State   
	Welcome[];

const State* 		Http_Session_Sm;
const State* 		Http_Session_App(void)	{return Welcome;}
//------------------------------------------------------------------
void 			Init_Http_Session	(void) 
{ 
 Http_Session_Sm=Welcome;
}	
const State** 	Http_Session		(void) 			{return &Http_Session_Sm;} 
void 		Http_Session_Rti	(void)			{Atomic_Send_Event(Rti_Event,Http_Session());}
//------------WELCOME----------------------------------------------------------------------------------------
static void  Welcome_Enable_Http	(void)	
{
 int Ret;
 sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID); 			//para asegurar el encendido hay que apagarlo y luego prenderlo.. 
 Ret=sl_NetAppStart(SL_NET_APP_HTTP_SERVER_ID); 			//prendo
 DBG_HTTP_SESSION_PRINT	("---->http: Habilitado %d \r\n",Ret);
}
//-------------------Funciones GET---------------------------------------------------------------------------------
unsigned char Get_Number=0;
void (*Get[])(unsigned char* Ans,unsigned char *Len);

void Get_000(unsigned char* Ans, unsigned char* Len) 	
{
	DBG_HTTP_SESSION_PRINT	("G000\r\n");
	Temp2_Fix_Point_Bcd(Ans,0);
	*Len=7;
}
void Get_001(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G001\r\n");Ans[0]='1';*Len=1;}
void Get_002(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G002\r\n");Ans[0]='2';*Len=1;}
void Get_003(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G003\r\n");Ans[0]='3';*Len=1;}

void (*Get[])(unsigned char* Ans,unsigned char *Len) = {
Get_000,
Get_000,
Get_002,
Get_003
};
//-------------------Funciones POST---------------------------------------------------------------------------------
void Post_000(unsigned char* Data,unsigned char Len)	{DBG_HTTP_SESSION_PRINT	("P000\r\n");}
void Post_001(unsigned char* Data,unsigned char Len)	{DBG_HTTP_SESSION_PRINT	("P001\r\n");}
void Post_002(unsigned char* Data,unsigned char Len)	{DBG_HTTP_SESSION_PRINT	("P002\r\n");}
void Post_003(unsigned char* Data,unsigned char Len)	{DBG_HTTP_SESSION_PRINT	("P003\r\n");}

void (*Post[])(unsigned char* Data,unsigned char Len) = {
Post_000,
Post_001,
Post_002,
Post_003
};

//----------------Callback-----------------------------------------------------------
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse) 
{
	switch (pHttpEvent->Event) {
		case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:				//se esperan solo gets del tipo __SL_G_000, si llega convierto el 000 a numero y llamo a la funcion numero 000 del arreglo de funciones de un solo tiro!! fua!
			Get[X_Digits_Dec_Bcd2Int(3,pHttpEvent->EventData.httpTokenName.data+7)](pHttpResponse->ResponseData.token_value.data,&pHttpResponse->ResponseData.token_value.len);
                break;
		case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT:			//se esperan post del tipo __SL_P_000. se convierte a numero y se dispara la funcion dentro del arreglo de funciones... if less code!!
			Post[X_Digits_Dec_Bcd2Int(3,pHttpEvent->EventData.httpPostData.token_name.data+7)](pHttpEvent->EventData.httpPostData.token_value.data,pHttpEvent->EventData.httpPostData.token_value.len);
                break;
		default:
			DBG_HTTP_SESSION_PRINT	("---->http Event: invalid\r\n");
		
	}
 }
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 Enable_Http_Event		,Welcome_Enable_Http				,Welcome,
 ANY_Event			,Rien						,Welcome,
};
