#include "hw_ints.h"
#include "hw_types.h"
#include "uart.h"
#include "utils.h"
#include "wlan.h"
#include "uart_if.h"
#include "common.h"
#include "wifi_network.h"
#include "wifi_session.h"
#include "wifi_socket.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "type_conversion.h"
#include "leds_session.h"
#include "debug.h"
#include "ubidots.h"


static const State   
	//Clientes
		C_Closed[],
		C_Opening[],
		C_Opened[],
		C_Connecting[],
		C_Connected[],
		C_Connected_Saving[],
	//Bind
		B_Closed[],
		B_Opening[],
		B_Opened[],
		B_Binding[],
		B_Binded[],
	//Servers
		S_Closed[],
		S_Listening[],
		S_Connected[],
		S_Connected_Saving[];

struct Socket_Struct
{
	const State* 		Sm;
	const State* 		App;
	int			Sd;
	unsigned char 		Rx_Buff[SOCKET_RX_BUF_SIZE];		//lo usa cada sockete para guardar data entry del usuario y poder parsearlo a gusto..
	unsigned int 		Rx_Pos;			//para ir contando mientras recibe datos
	unsigned int 		Rx_Length;		//numero de bytes a recibir
	unsigned char 		Rx_Delimiter;		//lo uso para terminar el ingreso de datos
	void 			(*Rx_Func)(void);	//se ejecuta cuando termina de recibir el dato pedido
	unsigned short		Port;			//almaceno el puerto de conexion.. por lo pronto fijo...
}Socket_List[MAX_SOCKETS];

//----------------------------------------------------------------------------------------------------
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse) { }
//----------------------------------------------------------------------------------------------------
const State* Find_Sm4Sd(unsigned char Sd)
{
	unsigned char i;
	for(i=0;i<MAX_SOCKETS && Sd!=Socket_List[i].Sd;i++) ;
	if(i<MAX_SOCKETS) return Socket_List[i].Sm;
	return Empty_App();
}
const State* Find_App4Sd(unsigned char Sd)
{
	unsigned char i;
	for(i=0;i<MAX_SOCKETS && Sd!=Socket_List[i].Sd;i++) ;
	if(i<MAX_SOCKETS) return Socket_List[i].App;
	return Empty_App();
}
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
	DBG_WIFI_SOCKET_PRINT	("---->Socket Event: %d socket: %d Sm: %d\r\n",pSock->Event,pSock->socketAsyncEvent.SockTxFailData.sd,Find_Sm4Sd(pSock->socketAsyncEvent.SockTxFailData.sd));
//	if(pSock->Event==SL_SOCKET_TX_FAILED_EVENT)	
//		Atomic_Send_Event(pSock->Event,Find_Sm4Sd(pSock->socketAsyncEvent.SockTxFailData.sd));
}
//----------------------------------------------------------------------------------------------------
void Open_Socket			(void)
{
	Set_Actual_Sd4Sm(sl_Socket(SL_AF_INET,SL_SOCK_STREAM,IPPROTO_TCP));
	Atomic_Send_Event(Actual_Sd4Sm()<0?Socket_Fail_Event:Socket_Created_Event,Actual_Sm());
	DBG_WIFI_SOCKET_PRINT		("---->Open sd: %d port: %d\r\n",Actual_Sd4Sm(),Actual_Port4Sm());
}
void Connect2Server		(void)
{
	SlSockAddrIn_t  	sAddr;
	int             	iAddrSize,iStatus;
	long lNonBlocking 	= 1; 
	sAddr.sin_family 	= SL_AF_INET;
	sAddr.sin_port 		= sl_Htons(Actual_Port4Sm());
	sAddr.sin_addr.s_addr 	= sl_Htonl(0x32177C44); //ubidots
//	sAddr.sin_addr.s_addr 	= sl_Htonl((Read_Actual_Ip()&0xFFFFFF00)+100);
	iAddrSize 		= sizeof(SlSockAddrIn_t);
	DBG_WIFI_SOCKET_PRINT	("---->Try connect server ip: ubidots sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());
	iStatus 		= sl_SetSockOpt(Actual_Sd4Sm(), SL_SOL_SOCKET, SL_SO_NONBLOCKING, &lNonBlocking, sizeof(lNonBlocking)); //configura el socket como no bloqueante, clave!
	iStatus 		= sl_Connect(Actual_Sd4Sm(), ( SlSockAddr_t *)&sAddr, iAddrSize);
	Atomic_Send_Event(iStatus<0?Server_Fail_Event:Server_Connected_Event,Actual_Sm());
}
void Bind_Port			(void)
{
	SlSockAddrIn_t  	sAddr;
	int             	iAddrSize,iStatus;
	long lNonBlocking 	= 1; 
	sAddr.sin_family 	= SL_AF_INET;
	sAddr.sin_port 		= sl_Htons(Actual_Port4Sm());
	sAddr.sin_addr.s_addr 	= 0;
	iAddrSize 		= sizeof(SlSockAddrIn_t);
	DBG_WIFI_SOCKET_PRINT	("---->Try Bind server sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());
	iStatus 		=  sl_SetSockOpt(Actual_Sd4Sm(), 	SL_SOL_SOCKET, 			SL_SO_NONBLOCKING, &lNonBlocking, sizeof(lNonBlocking)); //configura el socket como no bloqueante, clave!
 	iStatus 		+= sl_Bind	(Actual_Sd4Sm(),	(SlSockAddr_t *)&sAddr, 	iAddrSize);
	iStatus 		+= sl_Listen	(Actual_Sd4Sm(), 	0);
	Atomic_Send_Event(iStatus<0?Server_Fail_Event:Server_Binded_Event,Actual_Sm());
}
void Listen_Bind		(void)
{
   	unsigned char 	i;
   	SlSockAddrIn_t  sAddr;
 	int             iAddrSize,iNewSockID;

	iAddrSize = sizeof(SlSockAddrIn_t);
	for(i=MAX_CLIENTS;i<(MAX_CLIENTS+MAX_BIND);i++)
		if(Socket_List[i].Sd!=0)	{
			iNewSockID = sl_Accept(Socket_List[i].Sd, ( struct SlSockAddr_t *)&sAddr, (SlSocklen_t*)&iAddrSize);
			DBG_WIFI_SOCKET_PRINT("---->Searching Bind sd: %d ans: %d\r\n",Socket_List[i].Sd,iNewSockID);
			if(iNewSockID>=0) {
				Set_Actual_Sd4Sm(iNewSockID);
				Set_Actual_Port4Sm(Socket_List[i].Port);
//				Set_Actual_App4Sm(Wifi_Session_App());
				DBG_WIFI_SOCKET_PRINT("---->Accepted Bind sd: %d <-> sd: %d port: %d\r\n",Socket_List[i].Sd,Actual_Sd4Sm(),Actual_Port4Sm());
				Atomic_Send_Event(Server_Connected_Event,Actual_Sm());
				return;
			}
		}
}
void Client_Connected	(void) 	
{
	DBG_WIFI_SOCKET_PRINT("---->Client Connected sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());
//	Set_Actual_App4Sm(Wifi_Session_App());									//una vez conectado le asigno la app.. lo hago aca para no hcerlo cada vez que intento conectarme al server... 
	Set_Led_Effect(Led_Run,0xFFFF);
	Set_Temp_Led_Effect(Buzzer,0x0001);
	Atomic_Send_Event(Send_Temp_Hum_Event,Actual_App4Sm()); //le mando una 'A' para que haga algo../ubidots
	Set_Schedule4Sm(2);
}
void Server_Connected	(void) 	
{
	DBG_WIFI_SOCKET_PRINT("---->Server Connected sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());
	Set_Led_Effect(Led_Run,0xFFFF);
	Set_Temp_Led_Effect(Buzzer,0x0001);
	Set_Schedule4Sm(2);
}
void Server_Binded	(void)	{DBG_WIFI_SOCKET_PRINT("---->Server Binded sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());}

unsigned char Number_Of_Socket_Opened(void)
{
	unsigned char i,Ans=0;
	for(i=0;i<MAX_SOCKETS;i++)
		if(Socket_List[i].App!=Empty_App()) Ans++;
	DBG_WIFI_SOCKET_PRINT("---->Open Sockets: %d\n\r",Ans);
	return Ans;
}

void Close_Socket	(void)
{
	DBG_WIFI_SOCKET_PRINT("---->Socket Closed sd: %d\n\r",Actual_Sd4Sm());
	Free_All_Event_Listener(Actual_App4Sm());	//elimino cualquier lista de eventos pendientes...
	Free_All_Schedule(Actual_App4Sm());		//tambien elimino las schedules pendientes que haya tgenerado la app...
	//Set_Actual_App4Sm(Empty_App());			//por mensajes pendientes.. a cantarle a gardell creo que nunca puede pasar porque una vez que cierro el socket cambio de estado.. y en el estado nuevo no reenvio a app...
        sl_Close(Actual_Sd4Sm());
	Set_Schedule4Sm(600);
	Set_Temp_Led_Effect(Buzzer,0x0005);
	if(Number_Of_Socket_Opened()==0) Set_Led_Effect(Led_Run,0xA800);	//si no quedo ningn socket abierto, 3 pulsos..
}
void Unwanted_Close_Socket(void)
{
	DBG_WIFI_SOCKET_PRINT("---->Socket Closed Unwanted sd: %d\n\r",Actual_Sd4Sm());
	Free_All_Event_Listener(Actual_App4Sm());	//elimino cualquier lista de eventos pendientes...
	Free_All_Schedule(Actual_App4Sm());		//tambien elimino las schedules pendientes que haya tgenerado la app...
	Set_Actual_App4Sm(Empty_App());			//por mensajes pendientes.. a cantarle a gardell
        sl_Close(Actual_Sd4Sm());       
	Set_Schedule4Sm(100);
	//el estado de los leds en este caso lo maneja wifi_phisical...
}
void Reset_Sd				(void)
{
	Set_Actual_Sd4Sm(0);
}
void Socket_Opened			(void) 	{DBG_WIFI_SOCKET_PRINT("---->Socket Opened sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());}
void Server_Fail			(void)	{DBG_WIFI_SOCKET_PRINT("---->Server Fail sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());}
void Error_Sending_Data			(void)	{DBG_WIFI_SOCKET_PRINT("---->Error Sending Data\n\r");}
void Error_Receiving_Data		(void)	{DBG_WIFI_SOCKET_PRINT("---->Error Receiving Data\n\r");}
void Begin_Saving			(void)	{DBG_WIFI_SOCKET_PRINT("---->Begin Saving\n\r");}
//----------------------------------------------------------------------------------------------------
void Print_Error_Opening		(void)	{DBG_WIFI_SOCKET_PRINT("---->Error Socket Opening\n\r");}
void Print_Error_Opened			(void)	{DBG_WIFI_SOCKET_PRINT("---->Error Socket Opened\n\r");}
void Print_Error_Connected		(void)	{DBG_WIFI_SOCKET_PRINT("---->Error Socket Connected\n\r");}
void Print_Error_Disconnected		(void)	{DBG_WIFI_SOCKET_PRINT("---->Error Socket Disconnected\n\r");}
void Print_Closed			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Closed\n\r");}
void Print_Opening			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Opening\n\r");}
void Print_Opened			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Opened\n\r");}
void Print_Binding			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Binding\n\r");}
void Print_Connecting			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Connecting\n\r");}
void Print_Connected			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Connected\n\r");}
void Print_Connected_Saving		(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Connectedi Saving\n\r");}
void Print_Binded			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Binded\n\r");}
void Print_Sending			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Sending\n\r");}
void Print_Receiving			(void)	{DBG_WIFI_SOCKET_PRINT("---->Socket Receiving\n\r");}
void Print_Data_Sended			(void)	{DBG_WIFI_SOCKET_PRINT("---->Data Sended\n\r");}
void Print_Data_Received		(void)	{DBG_WIFI_SOCKET_PRINT("---->Data Received\n\r");}
void Print_Listening			(void)	{DBG_WIFI_SOCKET_PRINT("---->Listening sd: %d port: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm());}
//----------------------------------------------------------------------------------------------------
void Send_NLine2Socket		(void)			{Send_Data2Socket("\r\n",2);}
void Send_Char2Socket		(unsigned char Data) 	{unsigned char Buf[3]; Send_Data2Socket(Char2Bcd	(Buf,Data),3);}
void Send_Int2Socket		(unsigned int  Data) 	{unsigned char Buf[5]; Send_Data2Socket(Int2Bcd		(Buf,Data),5);}
void Send_Char_NLine2Socket	(unsigned char Data) 	{unsigned char Buf[5]; Send_Data2Socket(Char2Bcd_NLine	(Buf,Data),5);}	
void Send_Int_NLine2Socket	(unsigned int  Data) 	{unsigned char Buf[7]; Send_Data2Socket(Int2Bcd_NLine	(Buf,Data),7);}
void Send_Ip2Socket		(unsigned int  Ip) 	
{
	unsigned char Buf[]="192.198.002.002\r\n"; 
	Char2Bcd(Buf+ 0,((unsigned char*)&Ip)[3]);	//van al verre porque es indio chiquito...
	Char2Bcd(Buf+ 4,((unsigned char*)&Ip)[2]);
	Char2Bcd(Buf+ 8,((unsigned char*)&Ip)[1]);
	Char2Bcd(Buf+12,((unsigned char*)&Ip)[0]);
	Send_Data2Socket(Buf,sizeof(Buf));
}
//----------
void Send_NLine2Socket4Sm	(void)						{Send_Data2Socket4Sm("\r\n",2);}
void Send_Char2Socket4Sm	(unsigned char Data)				{unsigned char Buf[3]; Send_Data2Socket4Sm(Char2Bcd		(Buf,Data),3);}
void Send_Int2Socket4Sm		(unsigned int  Data)				{unsigned char Buf[5]; Send_Data2Socket4Sm(Int2Bcd		(Buf,Data),5);}
void Send_Char_NLine2Socket4Sm	(unsigned char Data)				{unsigned char Buf[5]; Send_Data2Socket4Sm(Char2Bcd_NLine	(Buf,Data),5);}
void Send_Int_NLine2Socket4Sm	(unsigned int  Data)				{unsigned char Buf[7]; Send_Data2Socket4Sm(Int2Bcd_NLine	(Buf,Data),7);}
//----------------------------------------------------------------------------------------------------
void Send_Data2Socket(unsigned char* Buf,unsigned int Length)
{
	DBG_WIFI_SOCKET_PRINT("---->Data sended 4App2Sd sd: %d port: %d Length: %d\n\r",Actual_Sd4App(),Actual_Port4App(),Length);
	Atomic_Send_Event(sl_Send(Actual_Sd4App(), Buf, Length, 0 )<0?Sending_Data_Error_Event:Data_Sended_Event,Actual_Sm4App());
}
void Send_Data2Socket4Sm(unsigned char* Buf,unsigned int Length)
{
	DBG_WIFI_SOCKET_PRINT("---->Data sended 4App2Sd sd: %d port: %d Length: %d\n\r",Actual_Sd4Sm(),Actual_Port4Sm(),Length);
	Atomic_Send_Event(sl_Send(Actual_Sd4Sm(), Buf, Length, 0 )<0?Sending_Data_Error_Event:Data_Sended_Event,Actual_Sm());
}
void Resend_Byte2Session(void)
{
	int             	iStatus;
	unsigned char 		Data;
	iStatus = sl_Recv(Actual_Sd4Sm(), &Data, 1, 0);
	if(iStatus==1) 	{
		DBG_WIFI_SOCKET_PRINT("---->Data Received from Server= %c status= %d sd: %d port: %d\n\r",Data,iStatus,Actual_Sd4Sm(),Actual_Port4Sm());
		Atomic_Send_Event(Data,Actual_App4Sm());
		Atomic_Send_Event(Rti_Event,Actual_Sm()); 				//para acelerar la lectura del supuesto proximo byte...
		Set_Toogle_Led_Effect(Led_Run);
	}
	else 	if(iStatus>=-1) Atomic_Send_Event(Receiving_Data_Error_Event,Actual_Sm()); 	//ojo que si pasa esto hay que reventar el socket... poque quiere decir que lo cerraron..o sea si intento leer el socket y me da cero bytes o -1 significa que revento el sockete.. si en cambio esta todo ok pero no hay nada para leer, entonces devuelve una cosas como -11 o una pabada asi...horrible...pero es asi....
}
void Save_Byte2Buffer(void)
{
	int             	iStatus;
	unsigned char 		Data;
	iStatus = sl_Recv(Actual_Sd4Sm(), &Data, 1, 0);
	if(iStatus==1) 	{
		DBG_WIFI_SOCKET_PRINT("---->Data Received from Server2Save2Buffer: %c Status: %d Sd= %d\n\r",Data,iStatus,Actual_Sd4Sm());
		if(Actual_Rx_Pos4Sm()<Actual_Rx_Length4Sm() && Data!=Actual_Rx_Delimiter4Sm()) {
			*(Actual_Rx_Buff4Sm()+Actual_Rx_Pos4Sm())=Data;
			(*Actual_Rx_PPos4Sm())++;
			DBG_WIFI_SOCKET_PRINT("1 byte recibido y van= %d\r\n",Actual_Rx_Pos4Sm());
			Atomic_Send_Event(Rti_Event,Actual_Sm()); 		//para acelerar la lectura del supuesto proximo byte...
		}
		else 	Atomic_Send_Event(Save_Ready_Event,Actual_Sm());
	}
	else 	if(iStatus>=-1) Atomic_Send_Event(Receiving_Data_Error_Event,Actual_Sm()); 	//ojo que si pasa esto hay que reventar el socket... poque quiere decir que lo cerraron..o sea si intento leer el socket y me da cero bytes o -1 significa que revento el sockete.. si en cambio esta todo ok pero no hay nada para leer, entonces devuelve una cosas como -11 o una pabada asi...horrible...pero es asi....
}
void Config2Save_Til_Delimiter(unsigned int Length,void (*Func) (void),unsigned char Offset,unsigned char Del)
{
	Set_Actual_Rx_Length4App(Length); 
	Set_Actual_Rx_Func4App(Func);  
	Set_Rx_Pos4App(Offset);
	Set_Rx_Delimiter4App(Del);
	Atomic_Insert_Event(Begin_Save_Event,Actual_Sm4App());
	DBG_WIFI_SOCKET_PRINT("Ready2Save bytes: %d\r\n",Length);
}
void Config2Save_Til_Enter(unsigned int Length,void (*Func) (void),unsigned char Offset)
{
	Config2Save_Til_Delimiter(Length,Func,Offset,'\n');
}
void Config2Save(unsigned int Length,void (*Func) (void))
{
	Config2Save_Til_Delimiter(Length,Func,0,0);
}
void Execute_Rx_Func(void)
{
	Actual_Rx_Func4Sm()();
	DBG_WIFI_SOCKET_PRINT("Executed Function\r\n");
	Atomic_Send_Event(Rti_Event,Actual_Sm()); 			//y que pasa si viene mas de un comando por paquete?? no quiero esperar... acelero por si me queda algo por leer...
}
//----------------------------------------------------------------------------------------------------
int  		Actual_Sd4Sm			(void)			{return  ((struct Socket_Struct*)Actual_Sm())->Sd;}
unsigned char*	Actual_Rx_Buff4Sm		(void)			{return  ((struct Socket_Struct*)Actual_Sm())->Rx_Buff;}
unsigned int	Actual_Rx_Pos4Sm		(void)			{return  ((struct Socket_Struct*)Actual_Sm())->Rx_Pos;}
unsigned char 	Actual_Rx_Delimiter4Sm		(void)			{return  ((struct Socket_Struct*)Actual_Sm())->Rx_Delimiter;}
unsigned short	Actual_Port4Sm			(void)			{return  ((struct Socket_Struct*)Actual_Sm())->Port;}
void		Set_Actual_Port4Sm		(unsigned short Port)	{        ((struct Socket_Struct*)Actual_Sm())->Port=Port;}
unsigned int	Actual_Rx_Length4Sm		(void)			{return  ((struct Socket_Struct*)Actual_Sm())->Rx_Length;}
unsigned int*	Actual_Rx_PPos4Sm		(void)			{return &((struct Socket_Struct*)Actual_Sm())->Rx_Pos;}
unsigned char*	Actual_Rx_Buff4App		(void)			{return  ((struct Socket_Struct*)Actual_Sm4App())->Rx_Buff;}
void 		(*Actual_Rx_Func4Sm		(void))	(void)		{return  ((struct Socket_Struct*)Actual_Sm())->Rx_Func;}
void 		Set_Schedule4Sm			(unsigned int TOut)	{Update_Schedule(TOut,Rti_Event,Actual_Sm());}
void 		Set_Actual_Rx_Func4App		(void(*Func)(void))	{        ((struct Socket_Struct*)Actual_Sm4App())->Rx_Func=Func;}
void 		Set_Actual_Rx_Length4App	(unsigned int Length)	{        ((struct Socket_Struct*)Actual_Sm4App())->Rx_Length=Length;}
void 		Set_Rx_Pos4App			(unsigned int Pos)	{        ((struct Socket_Struct*)Actual_Sm4App())->Rx_Pos=Pos;}
void 		Set_Rx_Delimiter4App		(unsigned char Del)	{        ((struct Socket_Struct*)Actual_Sm4App())->Rx_Delimiter=Del;}
const State**	Actual_App4Sm			(void)			{return Actual_Sm()+1;}
const State**	Actual_Sm4App			(void)			{return Actual_Sm()-1;}
int  		Actual_Sd4App			(void)			{return  ((struct Socket_Struct*)(Actual_Sm()-1))->Sd;}
int  		Actual_Port4App			(void)			{return  ((struct Socket_Struct*)(Actual_Sm()-1))->Port;}
int  		Set_Actual_Sd4Sm		(int Sd)		{return  ((struct Socket_Struct*)Actual_Sm())->Sd=Sd;}
const State* 	Set_Actual_App4Sm		(const State* App)	{return  ((struct Socket_Struct*)Actual_Sm())->App=App;}
void 		Init_Wifi_Socket		(void) 
{ 
	unsigned char i,j=0;
	for(i=0;i<MAX_CLIENTS;i++,j++){
		Socket_List[j].Sm	=C_Closed;
		Socket_List[j].App	=Ubidots_App();
		Socket_List[j].Sd	=0;
		Socket_List[j].Port	=80;//PORT_BASE+i; //test ubidots
		New_Periodic_Schedule	(100,Rti_Event,		Wifi_Socket(j));
	//	New_Periodic_Schedule	(2,Print_State_Event,	Wifi_Socket(j));	//debug
	}
	for(i=0;i<MAX_BIND;i++,j++){
		Socket_List[j].Sm	=B_Closed;
		Socket_List[j].App	=Empty_App();
		Socket_List[j].Sd	=0;
		Socket_List[j].Port	=PORT_BASE+i;
		New_Periodic_Schedule	(20,Rti_Event,		Wifi_Socket(j));
	//	New_Periodic_Schedule	(2,Print_State_Event,	Wifi_Socket(j));	//debug
	}
	for(i=0;i<MAX_SERVERS;i++,j++){
		Socket_List[j].Sm	=S_Closed;
		Socket_List[j].App	=Wifi_Session_App();
		Socket_List[j].Sd	=0;
		New_Periodic_Schedule	(20,Rti_Event,		Wifi_Socket(j));
	//	New_Periodic_Schedule	(2,Print_State_Event,	Wifi_Socket(j));	//debug
	}
}	
void Atomic_Send_Event2All_Sockets(unsigned int Event)
{
 unsigned char i;
 for(i=0;i<MAX_SOCKETS;i++) Atomic_Send_Event(Event,Wifi_Socket(i));
}
const State** 	Wifi_Socket		(unsigned char Sm) 	{return &Socket_List[Sm].Sm;} 
void 		Wifi_Socket_Rti		(void) 			{Atomic_Send_Event2All_Sockets(Rti_Event);}
//------------------CLIENT----------------------------------------------------------------------------------
static const State C_Closed[] =
{
 Ip_Acquired_Event		,Open_Socket			,C_Opening,
 SL_SOCKET_TX_FAILED_EVENT	,Reset_Sd			,C_Closed,
 Print_State_Event		,Print_Closed			,C_Closed,
 ANY_Event			,Rien				,C_Closed,
};
static const State C_Opening[] =
{
 Socket_Created_Event		,Socket_Opened			,C_Opened,
 Socket_Fail_Event		,Print_Error_Opening		,C_Opening,
 Ip_Released_Event		,Unwanted_Close_Socket		,C_Closed,
 Rti_Event			,Open_Socket			,C_Opening,
 Print_State_Event		,Print_Opening			,C_Opening,
 ANY_Event			,Rien				,C_Opening,
};
static const State C_Opened[] =
{
 Ip_Released_Event		,Unwanted_Close_Socket		,C_Closed,
 Rti_Event 			,Connect2Server			,C_Connecting,
 Print_State_Event		,Print_Opened			,C_Opened,
 ANY_Event			,Rien				,C_Opened,
};
static const State C_Connecting[] =
{
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,C_Opening,
 Server_Connected_Event		,Client_Connected		,C_Connected,
 Server_Fail_Event		,Server_Fail			,C_Opened,
 Ip_Released_Event		,Unwanted_Close_Socket		,C_Closed,
 Print_State_Event		,Print_Connecting		,C_Connecting,
 ANY_Event			,Rien				,C_Connecting,
};
static const State C_Connected[] =
{
 Rti_Event 			,Resend_Byte2Session		,C_Connected,
 Begin_Save_Event 		,Begin_Saving			,C_Connected_Saving,
 Ip_Released_Event		,Unwanted_Close_Socket		,C_Closed,
 Sending_Data_Error_Event	,Error_Sending_Data		,C_Connected,
 Receiving_Data_Error_Event	,Close_Socket			,C_Opening,
 Data_Sended_Event		,Print_Data_Sended		,C_Connected,
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,C_Opening,
 Close_Socket_Event		,Close_Socket			,C_Opening,
 Print_State_Event		,Print_Connected		,C_Connected,
 ANY_Event			,Rien				,C_Connected,
};
static const State C_Connected_Saving[] =
{
 Rti_Event 			,Save_Byte2Buffer		,C_Connected_Saving,
 Save_Ready_Event 		,Execute_Rx_Func		,C_Connected,
 Sending_Data_Error_Event	,Error_Sending_Data		,C_Connected_Saving,
 Receiving_Data_Error_Event	,Close_Socket			,C_Opening,
 Data_Sended_Event		,Print_Data_Sended		,C_Connected_Saving,
 Ip_Released_Event		,Unwanted_Close_Socket		,C_Closed,
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,C_Opening,
 Print_State_Event		,Print_Connected		,C_Connected_Saving,
 ANY_Event			,Rien				,C_Connected_Saving,
};
//-----------BIND-----------------------------------------------------------------------------------------
static const State B_Closed[] =
{
 Ip_Acquired_Event		,Open_Socket			,B_Opening,
 SL_SOCKET_TX_FAILED_EVENT	,Reset_Sd			,B_Closed,
 Print_State_Event		,Print_Closed			,B_Closed,
 ANY_Event			,Rien				,B_Closed,
};
static const State B_Opening[] =
{
 Socket_Created_Event		,Socket_Opened			,B_Opened,
 Socket_Fail_Event		,Print_Error_Opening		,B_Opening,
 Ip_Released_Event		,Unwanted_Close_Socket		,B_Closed,
 Rti_Event			,Open_Socket			,B_Opening,
 Print_State_Event		,Print_Opening			,B_Opening,
 ANY_Event			,Rien				,B_Opening,
};
static const State B_Opened[] =
{
 Ip_Released_Event		,Unwanted_Close_Socket		,B_Closed,
 Rti_Event 			,Bind_Port			,B_Binding,
 Print_State_Event		,Print_Opened			,B_Opened,
 ANY_Event			,Rien				,B_Opened,
};
static const State B_Binding[] =
{
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,B_Opening,
 Server_Binded_Event		,Server_Binded			,B_Binded,
 Server_Fail_Event		,Server_Fail			,B_Opened,
 Ip_Released_Event		,Unwanted_Close_Socket		,B_Closed,
 Rti_Event 			,Rien				,B_Binding,
 Print_State_Event		,Print_Binding			,B_Binding,
 ANY_Event			,Rien				,B_Binding,
};
static const State B_Binded[] =
{
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,B_Opening,
 Ip_Released_Event		,Unwanted_Close_Socket		,B_Closed,
 Rti_Event 			,Rien				,B_Binded,
 Print_State_Event		,Print_Binded			,B_Binded,
 ANY_Event			,Rien				,B_Binded,
};
//-----------SERVER-----------------------------------------------------------------------------------------
static const State S_Closed[] =
{
 Ip_Acquired_Event		,Listen_Bind			,S_Listening,
 SL_SOCKET_TX_FAILED_EVENT	,Reset_Sd			,S_Closed,
 Print_State_Event		,Print_Closed			,S_Closed,
 ANY_Event			,Rien				,S_Closed,
};
static const State S_Listening[] =
{
 Ip_Released_Event		,Unwanted_Close_Socket		,S_Closed,
 Server_Connected_Event		,Server_Connected		,S_Connected,
 Rti_Event			,Listen_Bind			,S_Listening,
 Print_State_Event		,Print_Listening		,S_Listening,
 ANY_Event			,Rien				,S_Listening,
};
static const State S_Connected[] =
{
 Rti_Event 			,Resend_Byte2Session		,S_Connected,
 Begin_Save_Event 		,Begin_Saving			,S_Connected_Saving,

 Data_Sended_Event		,Print_Data_Sended		,S_Connected,
 Receiving_Data_Error_Event	,Close_Socket			,S_Listening,
 Sending_Data_Error_Event	,Error_Sending_Data		,S_Connected,
 Ip_Released_Event		,Unwanted_Close_Socket		,S_Closed,
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,S_Listening,
 Print_State_Event		,Print_Connected		,S_Connected,
 ANY_Event			,Rien				,S_Connected,
};
static const State S_Connected_Saving[] =
{
 Rti_Event 			,Save_Byte2Buffer		,S_Connected_Saving,
 Save_Ready_Event 		,Execute_Rx_Func		,S_Connected,

 Data_Sended_Event		,Print_Data_Sended		,S_Connected_Saving,
 Sending_Data_Error_Event	,Error_Sending_Data		,S_Connected_Saving,
 SL_SOCKET_TX_FAILED_EVENT	,Close_Socket			,S_Listening,
 Ip_Released_Event		,Unwanted_Close_Socket		,S_Closed,
 Receiving_Data_Error_Event	,Close_Socket			,S_Listening,
 Print_State_Event		,Print_Connected_Saving		,S_Connected_Saving,
 ANY_Event			,Rien				,S_Connected_Saving,
};
//----------------------------------------------------------------------------------------------------
