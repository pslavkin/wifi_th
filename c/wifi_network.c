#include "hw_types.h"
#include "wlan.h"
#include "common.h"
#include "state_machine.h"
#include "wifi_phisical.h"
#include "wifi_session.h"
#include "wifi_network.h"
#include "wifi_socket.h"
#include "events.h"
#include "schedule.h"
#include "uart_if.h"
#include "debug.h"
#include "leds_session.h"

static const State   
        Ip_Released[],
        Ip_Acquired[];
//----------------------------------------------------------------------------------------------------
const State* 		Wifi_Network_Sm;
static SlNetAppEvent_t  *g_pNetAppEvent;
unsigned long  		g_ulGatewayIP; 	//Network Gateway IP address
unsigned long  		g_ulIpAddr;
//----------------------------------------------------------------------------------------------------
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
	g_pNetAppEvent=pNetAppEvent;
	Atomic_Send_Event(g_pNetAppEvent->Event,Wifi_Network());
}
void Ip_Release(void)
{
	DBG_WIFI_NETWORK_PRINT				("[NETAPP EVENT] Unexpected event [0x%x] \n\r", g_pNetAppEvent->Event);
	DBG_WIFI_NETWORK_PRINT				("---->IP Released\r\n");
	Atomic_Send_Event2All_Sockets(Ip_Released_Event);
}
void Ip_Acquire(void)
{
	SlIpV4AcquiredAsync_t *pEventData 	= NULL;
	//Ip Acquired Event Data		
	pEventData 				= &g_pNetAppEvent->EventData.ipAcquiredV4;
	g_ulIpAddr 				= pEventData->ip;
	//Gateway IP address
	g_ulGatewayIP 				= pEventData->gateway;
	DBG_WIFI_NETWORK_PRINT			("IP=%d.%d.%d.%d,Gateway=%d.%d.%d.%d\n\r", SL_IPV4_BYTE(g_ulIpAddr,3), SL_IPV4_BYTE(g_ulIpAddr,2), SL_IPV4_BYTE(g_ulIpAddr,1), SL_IPV4_BYTE(g_ulIpAddr,0), SL_IPV4_BYTE(g_ulGatewayIP,3), SL_IPV4_BYTE(g_ulGatewayIP,2), SL_IPV4_BYTE(g_ulGatewayIP,1), SL_IPV4_BYTE(g_ulGatewayIP,0));
	Atomic_Send_Event2All_Sockets(Ip_Acquired_Event);
	Set_Led_Effect(Led3,0xA800);		//3 pulsos
}
unsigned int Read_Actual_Ip(void)	{return g_ulIpAddr;}
//----------------------------------------------------------------------------------------------------
void Print_Error_Ip_Aquired		(void)	{DBG_WIFI_NETWORK_PRINT("---->Error Ip Aquired\n\r");}
void Print_Error_Ip_Released		(void)	{DBG_WIFI_NETWORK_PRINT("---->Error Ip Released\n\r");}
void Print_Ip_Acquired			(void)	{DBG_WIFI_NETWORK_PRINT("---->Ip Aquired State\n\r");}
void Print_Ip_Released			(void)	{DBG_WIFI_NETWORK_PRINT("---->Ip Released State\n\r");}
//------------------------------------------------------------------
void		Clear_Ip		(void)
{
	g_ulGatewayIP  = 0;
	g_ulIpAddr 	= 0;
}
void 		Init_Wifi_Network	(void) 
{ 
	Wifi_Network_Sm=Ip_Released;
	Clear_Ip();
// 	New_Periodic_Schedule(2,Print_State_Event,Wifi_Network());
}	
const State** 	Wifi_Network		(void) 			{return &Wifi_Network_Sm;} 
void 		Wifi_Network_Rti	(void)			{Atomic_Send_Event(Rti_Event,Wifi_Network());}
//----------------------------------------------------------------------------------------------------
static const State Ip_Released[] =
{
 SL_NETAPP_IPV4_IPACQUIRED_EVENT	,Ip_Acquire 					,Ip_Acquired,
 Print_State_Event			,Print_Ip_Released				,Ip_Released,
 ANY_Event				,Print_Error_Ip_Aquired				,Ip_Released,
};
static const State Ip_Acquired[] =
{
 Wlan_Disconnected_Event		,Ip_Release					,Ip_Released,
 Print_State_Event			,Print_Ip_Acquired				,Ip_Acquired,
 ANY_Event				,Ip_Release					,Ip_Released,
};
//----------------------------------------------------------------------------------------------------
