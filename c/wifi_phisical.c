#include "wlan.h"
#include "hw_types.h"
#include "uart_if.h"
#include "common.h"
#include "wifi_phisical.h"
#include "wifi_network.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "prcm.h"
#include "debug.h"
#include "leds_session.h"

static const State   
        Start_State[],
        Starting[],
	Net_Config_State[],
	Re_Start_State[],
	Waiting_Connection[],
	Connected[];

const State* 		Wifi_Phisical_Sm;
unsigned char  		g_ucConnectionSSID[SSID_LEN_MAX+1]; //Connection SSID
unsigned char  		g_ucConnectionBSSID[BSSID_LEN_MAX]; //Connection BSSID
static SlWlanEvent_t 	*g_pWlanEvent;
unsigned char 		Ssid_Name[16];
unsigned char 		Security_Key[20];
//------------------------------------------------------------------
void Set_Ssid_Name(unsigned char *Value,unsigned char Length) //recibe el nombre de la red a conectarse
{
 String_Copy_Backward(Value,Ssid_Name,Length);
}
void Set_Security_Key(unsigned char *Value,unsigned char Length) //recibe el nombre de la red a conectarse
{
 String_Copy_Backward(Value,Security_Key,Length);
}
//-----------------------------------------------
void Connect2Pyj(void)
{
	 Set_Ssid_Name		("pyj",3);
	 Set_Security_Key	("piedras689",10);
	 DBG_WIFI_PHISICAL_PRINT("Wifi_pyj\r\n"); 
	 //Set_Led_Effect		(Led1,0x0000);
	 Turn_On_Wifi		(); 
}
void Connect2Dci(void)
{
	 Set_Ssid_Name		("DCI",3);
	 Set_Security_Key	("DisenioconingeniO",17);
	 DBG_WIFI_PHISICAL_PRINT("Wifi_dci\r\n"); 
	 //Set_Led_Effect		(Led1,0x0000);
	 Turn_On_Wifi		(); 
}
//------------------------------------------------------------------
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
	g_pWlanEvent=pWlanEvent;
	Atomic_Send_Event(g_pWlanEvent->Event,Wifi_Phisical());
}
//----------------------------------------------------------------------------------------------------
void Turn_On_Wifi	(void) {Atomic_Send_Event(Turn_Wifi_On_Event,Wifi_Phisical()); }
void Turn_Off_Wifi	(void) {Atomic_Send_Event(Turn_Wifi_Off_Event,Wifi_Phisical()); }
//----------------------------------------------------------------------------------------------------
void Start_Complete(void)
{
	DBG_WIFI_PHISICAL_PRINT("---->Start Complete\r\n");
	Atomic_Send_Event(0,Wifi_Phisical());
}
void Start(void)
{
	DBG_WIFI_PHISICAL_PRINT("---->Start\r\n");
	sl_Start(0, 0,(P_INIT_CALLBACK)Start_Complete);
}
void Re_Start(void)
{
	DBG_WIFI_PHISICAL_PRINT("---->Re Start\r\n");
	sl_Start(0, 0,(P_INIT_CALLBACK)Start_Complete);
}
void Net_Config2ST(void)
{
	unsigned char Val=1,Power=0;
    	_WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};
    	memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
	DBG_WIFI_PHISICAL_PRINT("---->Net_Config2ST\r\n");
	Atomic_Send_Event(
		sl_WlanPolicySet		(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0)				+
		sl_WlanProfileDel		(0xFF)												+
		sl_WlanDisconnect		()?0:0												+ //suma siempre cero porque si ya estaba desconectado devuelve -1.. raro...
		sl_NetCfgSet			(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&Val) 							+
		sl_WlanPolicySet		(SL_POLICY_SCAN, 0, NULL, 0)									+
    		sl_WlanSet			(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1,&Power)			+
    		sl_WlanPolicySet		(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0)							+
    		sl_NetAppMDNSUnRegisterService	(0, 0) 												+
    		sl_WlanRxFilterSet		(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask, sizeof(_WlanRxFilterOperationCommandBuff_t))	+
    		sl_Stop				(0)										,
		Actual_Sm());
}
void Try_Connect(void)
{
	SlSecParams_t secParams = {0};
	long lRetVal = 0;
	
	secParams.Key 		= Security_Key;
	secParams.KeyLen 	= strlen(Security_Key);
	secParams.Type 		= SECURITY_TYPE;
	DBG_WIFI_PHISICAL_PRINT("---->Try Connect\r\n");
	Clear_SSID_BSSID();
 	lRetVal=sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID); 			//para asegurar el encendido hay que apagarlo y luego prenderlo.. 
//	DBG_WIFI_PHISICAL_PRINT("---->Off Http %d\r\n",lRetVal);
	
 	lRetVal=sl_NetAppStart(SL_NET_APP_HTTP_SERVER_ID); 			//prendo
//	DBG_WIFI_PHISICAL_PRINT("---->On Http %d\r\n",lRetVal);
	sl_WlanConnect(Ssid_Name, strlen(Ssid_Name), 0, &secParams, 0);
	Set_Led_Effect(Led_Run,0x8000); 			//1 pulsos
}
void Wlan_Connect(void)
{
	slWlanConnectAsyncResponse_t *pEventData 	= NULL;
	pEventData 					= & g_pWlanEvent->EventData.STAandP2PModeWlanConnected;
	memcpy						(g_ucConnectionSSID,	g_pWlanEvent->EventData.  STAandP2PModeWlanConnected.ssid_name, 	g_pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
	memcpy						(g_ucConnectionBSSID, 	g_pWlanEvent->EventData.	STAandP2PModeWlanConnected.bssid, 	SL_BSSID_LENGTH);
	DBG_WIFI_PHISICAL_PRINT				("[WLAN EVENT] STA Connected to the AP: %s ," " BSSID: %x:%x:%x:%x:%x:%x\n\r", g_ucConnectionSSID,g_ucConnectionBSSID[0], g_ucConnectionBSSID[1],g_ucConnectionBSSID[2], g_ucConnectionBSSID[3],g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
	DBG_WIFI_PHISICAL_PRINT 			("---->Wlan Conectado....\r\n");
	Set_Led_Effect(Led_Run,0xA000); 			//2 pulsos
}
void Wlan_Disconnect(void)
{
	DBG_WIFI_PHISICAL_PRINT("---->Desconecto Wlan....\r\n");
	sl_WlanDisconnect();
	Atomic_Send_Event(Wlan_Disconnected_Event,Wifi_Network());
}
void Wlan_Unwanted_Disconnect(void)
{
	slWlanConnectAsyncResponse_t*  pEventData = NULL;
	pEventData = &g_pWlanEvent->EventData.STAandP2PModeDisconnected;

	// If the user has initiated 'Disconnect' request,'reason_code' is SL_USER_INITIATED_DISCONNECTION
	if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
	    DBG_WIFI_PHISICAL_PRINT("[WLAN EVENT]Device disconnected from the AP: %s," "BSSID: %x:%x:%x:%x:%x:%x on application's request \n\r", g_ucConnectionSSID,g_ucConnectionBSSID[0], g_ucConnectionBSSID[1],g_ucConnectionBSSID[2], g_ucConnectionBSSID[3],g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
	else
	    DBG_WIFI_PHISICAL_PRINT("[WLAN ERROR]Device disconnected from the AP: %s," "BSSID: %x:%x:%x:%x:%x:%x on an ERROR..!! \n\r", g_ucConnectionSSID,g_ucConnectionBSSID[0], g_ucConnectionBSSID[1],g_ucConnectionBSSID[2], g_ucConnectionBSSID[3],g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
	memset(g_ucConnectionSSID,	0,sizeof(g_ucConnectionSSID));
	memset(g_ucConnectionBSSID,	0,sizeof(g_ucConnectionBSSID));
	DBG_WIFI_PHISICAL_PRINT("---->Wlan Desconectado x Error....\r\n");
	Atomic_Send_Event(Wlan_Disconnected_Event,Wifi_Network());
	Atomic_Send_Event(Wlan_Disconnected_Event,Wifi_Phisical());
}
void Stop(void)
{
	DBG_WIFI_PHISICAL_PRINT("---->Wifi apagado\r\n");
//	sl_Stop				(1000);
	PRCMMCUReset(1);
}
//----------------------------------------------------------------------------------------------------
static void Print_Error_Start			(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Error Start\n\r");}
static void Print_Error_Starting		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Error Starting\n\r");}
static void Print_Error_Net_Config		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Error Net_Config\n\r");}
static void Print_Error_Re_Start		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Error Re Start\n\r");}
static void Print_Error_Waiting_Connection	(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Error Waiting Connection\n\r");}
static void Print_Error_Connected		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Error Connected\n\r");}
static void Print_Empty_Event			(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Empty Event\n\r");}
static void Print_Start_State			(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Start State\n\r");}
static void Print_Starting_State		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Starting State\n\r");}
static void Print_Net_Config_State		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Net_Config State\n\r");}
static void Print_Re_Start_State		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Re Start State\n\r");}
static void Print_Waiting_Connection_State	(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Waiting Connection State\n\r");}
static void Print_Connected_State		(void)	{DBG_WIFI_PHISICAL_PRINT("---->Wlan Connected State\n\r");}
//------------------------------------------------------------------
void 		Init_Wifi_Phisical	(void) 
{ 
 Wifi_Phisical_Sm=Start_State;
 New_Periodic_Schedule(2,Rti_Event,		Wifi_Phisical());
 Connect2Pyj();
// Connect2Dci();
}	
void Clear_SSID_BSSID(void)
{
 memset(g_ucConnectionSSID,0,sizeof(g_ucConnectionSSID));
 memset(g_ucConnectionBSSID,0,sizeof(g_ucConnectionBSSID));
}
const State** 	Wifi_Phisical		(void) 			{return &Wifi_Phisical_Sm;} 
void 		Wifi_Phisical_Rti	(void)			{Atomic_Send_Event(Rti_Event,Wifi_Phisical());}
//----------------------------------------------------------------------------------------------------
static const State Start_State[] =
{
 Turn_Wifi_On_Event		,Start						,Starting,
 Turn_Wifi_Off_Event		,Stop						,Start_State,
 Print_State_Event		,Print_Start_State				,Start_State,
 ANY_Event			,Print_Error_Start				,Start_State,
};

static const State Starting[] =
{
 0				,Net_Config2ST					,Net_Config_State,
 Rti_Event			,Rien						,Starting,
 Turn_Wifi_Off_Event		,Stop						,Start_State,
 Print_State_Event		,Print_Starting_State				,Starting,
 ANY_Event			,Print_Error_Starting				,Start_State,
};

static const State Net_Config_State[] =
{
 0				,Re_Start					,Re_Start_State,
 Rti_Event			,Rien						,Net_Config_State,
 Turn_Wifi_Off_Event		,Stop						,Start_State,
 Print_State_Event		,Print_Net_Config_State				,Net_Config_State,
 ANY_Event			,Print_Error_Net_Config				,Start_State,
};
static const State Re_Start_State[] =
{
 0				,Try_Connect					,Waiting_Connection,
 Rti_Event			,Rien						,Re_Start_State,
 Turn_Wifi_Off_Event		,Stop						,Start_State,
 Print_State_Event		,Print_Re_Start_State				,Re_Start_State,
 ANY_Event			,Print_Error_Re_Start				,Start_State,
};
static const State Waiting_Connection[] =
{
 SL_WLAN_CONNECT_EVENT		,Wlan_Connect 					,Connected,
 SL_WLAN_DISCONNECT_EVENT	,Wlan_Unwanted_Disconnect			,Waiting_Connection,
 Wlan_Disconnected_Event	,Try_Connect					,Waiting_Connection,
 0				,Print_Empty_Event				,Waiting_Connection,
 Rti_Event			,Rien						,Waiting_Connection,
 Turn_Wifi_Off_Event		,Stop						,Start_State,
 Print_State_Event		,Print_Waiting_Connection_State			,Waiting_Connection,
 ANY_Event			,Print_Error_Waiting_Connection			,Waiting_Connection,
};
static const State Connected[] =
{
 SL_WLAN_DISCONNECT_EVENT	,Wlan_Unwanted_Disconnect			,Waiting_Connection,
 Rti_Event			,Rien						,Connected,
 Turn_Wifi_Off_Event		,Stop						,Start_State,
 0				,Print_Empty_Event				,Connected,
 Print_State_Event		,Print_Connected_State				,Connected,
 ANY_Event			,Print_Error_Connected				,Connected,
};
//----------------------------------------------------------------------------------------------------
