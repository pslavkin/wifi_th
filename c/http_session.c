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
static void  Welcome_B	(void)	{}
static void  Welcome_C	(void)	{}
static void  Welcome_D	(void)	{}
static void  Welcome_E	(void)	{}
static void  Welcome_F	(void)	{}
//----------------------------------------------------------------------------------------------------
//
//

void Get_000(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G000\r\n");Ans[0]='0';*Len=1;}
void Get_001(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G001\r\n");Ans[0]='1';*Len=1;}
void Get_002(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G002\r\n");Ans[0]='2';*Len=1;}
void Get_003(unsigned char* Ans, unsigned char* Len) 	{DBG_HTTP_SESSION_PRINT	("G003\r\n");Ans[0]='3';*Len=1;}

void Post_000(void) 					{DBG_HTTP_SESSION_PRINT	("P000\r\n");}
void Post_001(void) 					{DBG_HTTP_SESSION_PRINT	("P001\r\n");}
void Post_002(void) 					{DBG_HTTP_SESSION_PRINT	("P002\r\n");}
void Post_003(void) 					{DBG_HTTP_SESSION_PRINT	("P003\r\n");}


void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse) 
{
	unsigned char Token_String[5]="00000"; //del tipo __S_P_123 y me quedo con los ultimos 5 digitos.. en este caso P_123, luego relleno con ceros el P y el _ y puedo convertir a integer para hacer un case hasta 999 tokens de get y otro tanto de post..sufi...
	unsigned short int Token_Number;
	unsigned char* Ans_String;
	unsigned char* Ans_Len;
	DBG_HTTP_SESSION_PRINT	("---->http Event\r\n");
	Set_Temp_Led_Effect(Led_THR,0x00FF);
	switch (pHttpEvent->Event) {
		case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:
			strncpy(Token_String+2,(pHttpEvent->EventData.httpTokenName.data)+7,3);
			Token_Number=Dec_Bcd2Int(Token_String);
			DBG_HTTP_SESSION_PRINT	("---->http GET number=%d string=%s \r\n",Token_Number,pHttpEvent->EventData.httpTokenName.data);
			Ans_String=pHttpResponse->ResponseData.token_value.data;
			Ans_Len=&pHttpResponse->ResponseData.token_value.len;
			switch (Token_Number)	{
				case 000: Get_000(Ans_String,Ans_Len); break;
				case 001: Get_001(Ans_String,Ans_Len); break;
				case 002: Get_002(Ans_String,Ans_Len); break;
				case 003: Get_003(Ans_String,Ans_Len); break;
				}
                break;
		case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT:
			strncpy(Token_String+2,(pHttpEvent->EventData.httpPostData.token_name.data)+7,3);
			Token_Number=Dec_Bcd2Int(Token_String);
			DBG_HTTP_SESSION_PRINT	("---->http POST number=%d string=%s \r\n",Token_Number,pHttpEvent->EventData.httpPostData.token_value.data);
			switch (Token_Number)	{
				case 000: Post_000(); break;
				case 001: Post_001(); break;
				case 002: Post_002(); break;
				case 003: Post_003(); break;
			}
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
