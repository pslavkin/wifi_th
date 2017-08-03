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
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse) 
{
	DBG_HTTP_SESSION_PRINT	("---->http Event: llego algo!\r\n");
	Set_Temp_Led_Effect(Led_THR,0x00FF);
	switch (pHttpEvent->Event) {
		case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:
			DBG_HTTP_SESSION_PRINT	("---->http Event: llego Get %s\r\n",pHttpEvent->EventData.httpTokenName.data);
			if(!strncmp("__SL_G_XXY",pHttpEvent->EventData.httpTokenName.data,pHttpEvent->EventData.httpTokenName.len)) {
				DBG_HTTP_SESSION_PRINT	("---->http procesando XXY\r\n");
                		unsigned char * ptr = pHttpResponse->ResponseData.token_value.data;
                		ptr[0] = 'O';
                		ptr[1] = 'K';
                		pHttpResponse->ResponseData.token_value.len = 2;
			}
                break;
		case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT:
			{
				unsigned char T[5];
				unsigned int Token;
				DBG_HTTP_SESSION_PRINT	("---->http Event: llego post %s\r\n",pHttpEvent->EventData.httpPostData.token_name.data);
				strncpy(T+2,(pHttpEvent->EventData.httpPostData.token_name.data)+7,3);
				T[0]=T[1]='0';
				Token=Dec_Bcd2Int(T);
				switch (Token)	{
					case 11:
						DBG_HTTP_SESSION_PRINT	("---->http procesando 11 %s\r\n",pHttpEvent->EventData.httpPostData.token_value.data);
						break;
				}
			}	
                break;
		default:
				DBG_HTTP_SESSION_PRINT	("---->http Event: no se que llego...\r\n");
		
	}
 }
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 Enable_Http_Event		,Welcome_Enable_Http				,Welcome,
 ANY_Event			,Rien						,Welcome,
};
