#ifndef  WIFI_SOCKET
#define  WIFI_SOCKET

#include "state_machine.h"
//----------------------------------------------------------------------------------------------------

//extern int BsdTcpClient();
//extern int BsdTcpServer(unsigned short usPort);
//extern long WlanConnect();              
//extern void DisplayBanner();            
//extern void BoardInit();                
//extern void InitializeAppVariables();   


//en realidad se podrias hacer todo con un mismo puerto.. tabto el cliente hablar al puerto X como el servidor escuchar solo en el puerto X.. y las conexiones se ultiplexan por el puerto de salida...tampoco esta mal dejar un par de puertos abiertos por si esta opcipado uno... que se yo.. o aclaro porque creo que lo hice pensando que no se podia...
#define MAX_SOCKETS 	(MAX_CLIENTS+MAX_BIND+MAX_SERVERS)
#define MAX_CLIENTS 	1
#define MAX_BIND 	1		
#define MAX_SERVERS 	1
#define PORT_BASE       49152 
//------------------------------------------------------------------------------------                         
#define SOCKET_RX_BUF_SIZE 1000
// Application specific status/error codes
//------------------------------------------------------------------------------------                         
typedef enum{
	// Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
	SOCKET_CREATE_ERROR 		= 			-0x7D0,
	BIND_ERROR 			= SOCKET_CREATE_ERROR 	-1,
	LISTEN_ERROR 			= BIND_ERROR 		-1,
	SOCKET_OPT_ERROR 		= LISTEN_ERROR 		-1,
	CONNECT_ERROR 			= SOCKET_OPT_ERROR 	-1,
	ACCEPT_ERROR 			= CONNECT_ERROR 	-1,
	SEND_ERROR 			= ACCEPT_ERROR 		-1,
	RECV_ERROR 			= SEND_ERROR 		-1,
	SOCKET_CLOSE_ERROR 		= RECV_ERROR 		-1,
	DEVICE_NOT_IN_STATION_MODE 	= SOCKET_CLOSE_ERROR 	-1,
	STATUS_CODE_MAX 		= 			-0xBB8
}e_AppStatusCodes;

//-----------------------------------------------------------
enum Wifi_Socket_Event_Code	{
					Ip_Acquired_Event		= 0x0D00,
					Ip_Released_Event		= 0x0D01,
					Socket_Fail_Event		= 0x0D02,
					Socket_Created_Event		= 0x0D03,
					Server_Fail_Event		= 0x0D04,
					Server_Connected_Event		= 0x0D05,
					Data_Sended_Event		= 0x0D06, 
					Sending_Data_Error_Event 	= 0x0D07,
					Receiving_Data_Error_Event	= 0x0D08, 
					Data_Received_Event		= 0x0D09,
					Client_Event			= 0x0D0A, 
					Bind_Event			= 0x0D0B, 
					Server_Event			= 0x0D0C, 
				       	Server_Binded_Event		= 0x0D0D, 
				       	Begin_Save_Event		= 0x0D0E,
				       	Save_Ready_Event		= 0x0D0F,
				       	Close_Socket_Event		= 0x0D10
	    	};
//-----------------------------------------------------------
extern void 		Wifi_Socket_Rti			(void);
extern void 		Init_Wifi_Socket		(void);
extern const State** 	Wifi_Socket			(unsigned char sd);
extern void 		Atomic_Send_Event2All_Sockets	(unsigned int Event);
//----------------------------------------------------
extern void 		Send_NLine2Socket		(void);
extern void 		Send_Char2Socket		(unsigned char Data);	
extern void 		Send_Int2Socket			(unsigned int  Data);	
extern void 		Send_Char_NLine2Socket		(unsigned char Data);	
extern void 		Send_Int_NLine2Socket		(unsigned int  Data);	
extern void 		Send_Ip2Socket			(unsigned int  Ip);
//----------
extern void 		Send_NLine2Socket4Sm		(void);
extern void 		Send_Char2Socket4Sm		(unsigned char Data);	
extern void 		Send_Int2Socket4Sm		(unsigned int  Data);	
extern void 		Send_Char_NLine2Socket4Sm	(unsigned char Data);	
extern void 		Send_Int_NLine2Socket4Sm	(unsigned int  Data);
//----------------------------------------------------
extern int  		Actual_Sd4Sm		 	(void);	
extern unsigned int 	Actual_CBS4Sm		 	(void);
extern const State**	Actual_App4Sm		 	(void);	
extern const State**	Actual_Sm4App		 	(void);
extern int  		Actual_Sd4App		 	(void);	
extern int  		Actual_Port4App			(void);
extern unsigned short	Actual_Port4Sm			(void);	
extern unsigned char*	Actual_Rx_Buff4Sm		(void);
extern unsigned int	Actual_Rx_Pos4Sm		(void);
extern unsigned char 	Actual_Rx_Delimiter4Sm		(void);
extern void		Set_Actual_Port4Sm		(unsigned short Port);
extern unsigned int	Actual_Rx_Length4Sm		(void);
extern unsigned int*	Actual_Rx_PPos4Sm		(void);
extern void 		(*Actual_Rx_Func4Sm		(void))	(void);	
extern unsigned char*	Actual_Rx_Buff4App		(void);
extern void 		Set_Rx_Pos4App			(unsigned int Pos);
extern void 		Set_Rx_Delimiter4App		(unsigned char Del);
extern void 		Set_Actual_Rx_Func4App		(void (*Func)(void));
extern void 		Set_Actual_Rx_Length4App	(unsigned int Length);
extern int  		Set_Actual_Sd4Sm		(int Sd);
extern const State* 	Set_Actual_App4Sm		(const State* App);
extern void 		Set_Schedule4Sm			(unsigned int TOut);	
extern void 		Send_Hex_Data2Socket		(unsigned char* Buf,unsigned int Length);
extern void 		Send_Data2Socket4Sm		(unsigned char* Buf,unsigned int Length);
extern void 		Send_Data2Socket		(unsigned char* Buf,unsigned int Length);
extern void 		Resend_Byte2Session		(void);
//----------------------------------------------------
extern void 		Config2Save_Til_Delimiter	(unsigned int Length,void (*Func) (void),unsigned char Offset,unsigned char Del);
extern void 		Config2Save_Til_Enter		(unsigned int Length,void (*Func) (void),unsigned char Offset);
extern void 		Config2Save			(unsigned int Length,void (*Func) (void));
//----------------------------------------------------
extern unsigned char 	Number_Of_Socket_Opened		(void);
//----------------------------------------------------

#endif

