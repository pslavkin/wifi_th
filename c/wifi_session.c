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
#include "flash.h"
#include "debug.h"
#include "string.h"
#include "one_wire_transport.h"

static const State   
	Welcome[],
	Info[],
	Network[],
	Virtual_Buttons[],
	File_System[],
	Temp[];

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
"D File System\r\n"
"E Temperature\r\n"
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
unsigned char File_System_Help_Data[]=
{
"File Sustem\r\n"
"A Load Name\r\n"
"B Create File\r\n"
"C Read File\r\n"
"D Write File\r\n"
"E Info File\r\n"
"F Del File\r\n"
"G Copy File\r\n"
"H Append File\r\n"
"< Back\r\n"
"? Help\r\n"
};
unsigned char Temp_Help_Data[]=
{
"Temperature\r\n"
"A View Temperature\r\n"
"B Refresh Nodes\r\n"
"C-Load Refresh Nodes Period\r\n"
"D-View\r\n"
"E-Save\r\n"
"F-Actual\r\n"
"G-Inc Delay\r\n"
"H-Dec Delay\r\n"
"I-Actual Delay\r\n"
"? Help\r\n"
"< Return\r\n"
};
//------------WELCOME----------------------------------------------------------------------------------------
void  Welcome_A	(void)	{}
void  Welcome_B	(void)	{}
void  Welcome_C	(void)	{}
void  Welcome_D	(void)	{}
void  Welcome_E	(void)	{}
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
void	File_System_A4Buff	(void)	{
	Actual_Rx_Buff4Sm()[Actual_Rx_Pos4Sm()]=0; 					//fuerzo fin del nombre justo al final del largo del nombre que ingreso el usuario..	
	DBG_WIFI_SESSION_PRINT("Nombre %s\r\n",Actual_Rx_Buff4Sm());
}
void	File_System_B4Buff	(void)	{
	Send_Data2Socket(Actual_Rx_Buff4App(),Search_Null_On_String(Actual_Rx_Buff4App(),FILE_NAME_SIZE));
	if(File_Create(Actual_Rx_Buff4App(),100)==0) {					//lo creo con un tamanio minimo y luego cuando se escribe lo agrando de ser neceario (creando otro fila claro...)
		Send_Data2Socket(" created\r\n",10);
	}
	else	Send_Data2Socket("not created\r\n",13);
	DBG_WIFI_SESSION_PRINT("Archivo %s creado\r\n",Actual_Rx_Buff4App());
}
void	File_System_C4Buff	(void)	{
	signed long Ret;
	unsigned char Buf[MAX_FILE_LENGTH];
	Ret=File_Read(Actual_Rx_Buff4App(),Buf,0,MAX_FILE_LENGTH); 		//pido para leer todo lo que pueda.. pero en realidad el fs.h es inteligente y me devuelve todo lo que tiene..
	if(Ret>0) Send_Data2Socket(Buf,Ret);					//File_Read me dice cuando leyo exactamente..
	Send_Data2Socket("EOF\r\n",5);
	DBG_WIFI_SESSION_PRINT("Leyendo desde %s tamanio %d data %s \r\n",Actual_Rx_Buff4App(),Ret,Buf);
}
void	File_System_D4Buff	(void)	{
	signed long Ret;
	Ret=File_Write(Actual_Rx_Buff4Sm(),Actual_Rx_Buff4Sm()+FILE_NAME_SIZE,Actual_Rx_Pos4Sm()-FILE_NAME_SIZE); 	//grabo en el file desde la FILE_NAME_SIZE, que ahi termina el nombre y hasta lo que indica la pos, que es la cantidad de datos que llegaron antes del delimiter
	if(Ret>0) {
		Send_Int2Socket4Sm(Ret);
		Send_Data2Socket4Sm(" bytes written\r\n",16);
	}
	else 	Send_Data2Socket4Sm("error\r\n",7);
	DBG_WIFI_SESSION_PRINT("grabando en %s datos %s tamanio %d \r\n",Actual_Rx_Buff4Sm(),Actual_Rx_Buff4Sm()+FILE_NAME_SIZE,Ret);
}
void	File_System_E4Buff	(void)	{
	SlFsFileInfo_t Fileinfo;
	File_Info(Actual_Rx_Buff4App(),&Fileinfo);
	Send_Data2Socket(Actual_Rx_Buff4App(),Search_Null_On_String(Actual_Rx_Buff4App(),FILE_NAME_SIZE));
	Send_NLine2Socket();
	if(Fileinfo.AllocatedLen>0) {
		Send_Data2Socket("fill=",5);
		Send_Int_NLine2Socket(Fileinfo.FileLen);
		Send_Data2Socket("size=",5);
		Send_Int_NLine2Socket(Fileinfo.AllocatedLen);
	} 
	else	Send_Data2Socket("not exist\r\n",11);
	DBG_WIFI_SESSION_PRINT("Info de archivo %s len= %d usado = %d\r\n",Actual_Rx_Buff4App(),Fileinfo.FileLen,Fileinfo.AllocatedLen);
}
void	File_System_F4Buff	(void)	{
	signed long Ret;
	Ret=File_Del(Actual_Rx_Buff4App());
	Send_Data2Socket(Actual_Rx_Buff4App(),Search_Null_On_String(Actual_Rx_Buff4App(),FILE_NAME_SIZE));
	Send_Data2Socket(Ret==0?" deleted  \r\n ":" not exist\r\n",12);
	DBG_WIFI_SESSION_PRINT("archivo %s borrado\r\n",Actual_Rx_Buff4App());
} 
void	File_System_G4Buff	(void)	{
	signed long Ret;
	Ret=File_Copy(Actual_Rx_Buff4App(),"tmp");
	Send_Data2Socket(Actual_Rx_Buff4App(),Search_Null_On_String(Actual_Rx_Buff4App(),FILE_NAME_SIZE));
	Send_Data2Socket(Ret>0?" copy \r\n ":" error\r\n",8);
	DBG_WIFI_SESSION_PRINT("archivo %s copiado a tmp\r\n",Actual_Rx_Buff4App());
} 
void	File_System_H4Buff	(void)	{
	signed long Ret;
	Ret=File_Append(Actual_Rx_Buff4Sm(),Actual_Rx_Buff4Sm()+FILE_NAME_SIZE,Actual_Rx_Pos4Sm()-FILE_NAME_SIZE); 		//grabo en el file desde la FILE_NAME_SIZE, que ahi termina el nombre y hasta lo que indica la pos, que es la cantidad de datos que llegaron antes del delimiter
	if(Ret>0) {
		Send_Int2Socket4Sm(Ret);
		Send_Data2Socket4Sm(" bytes total\r\n",14);
	}
	else 	Send_Data2Socket4Sm("error\r\n",7);
	DBG_WIFI_SESSION_PRINT("datos agregados en %s datos %s \r\n",Actual_Rx_Buff4Sm(),Actual_Rx_Buff4Sm()+FILE_NAME_SIZE);
}
//--------------FILE SYSTEM----
void	File_System_A		(void)	{Config2Save_Til_Enter(19,File_System_A4Buff,0);}	//19 maximo largo del nombre, uno mas para el '\0'
void	File_System_B		(void)	{File_System_B4Buff();}
void	File_System_C		(void)	{File_System_C4Buff();}
void	File_System_D		(void)	{Config2Save_Til_Delimiter(SOCKET_RX_BUF_SIZE,File_System_D4Buff,FILE_NAME_SIZE,'!');}	//grabo entrada de usuario hasta el enter y lo dejo en el buffer del socket a partir de la posicion FILE_NAME_SIZE.. antes de eso esta el nombre del file...
void	File_System_E		(void)	{File_System_E4Buff();}
void	File_System_F		(void)	{File_System_F4Buff();}
void	File_System_G		(void)	{File_System_G4Buff();}
void	File_System_H		(void)	{Config2Save_Til_Delimiter(SOCKET_RX_BUF_SIZE,File_System_H4Buff,FILE_NAME_SIZE,'!');}	//grabo entrada de usuario hasta el enter y lo dejo en el buffer del socket a partir de la posicion FILE_NAME_SIZE.. antes de eso esta el nombre del file...
//--------------TEMPERATURE----
void  	Temp_A4Buff  		(void)  {unsigned int  A=Dec_Bcd2Int(Actual_Rx_Buff4Sm());Send_Int_NLine2Socket(A);}
//----------
void	Temp_A		(void)	{Send_One_Wire_Info2Tcp();}
void	Temp_B		(void)	{Reload_One_Wire_Codes();}
void	Temp_C		(void)	{Config2Save(5,Temp_A4Buff);}
void	Temp_D		(void)	{}
void	Temp_E		(void)	{}
void	Temp_F		(void)	{}
void	Temp_G		(void)	{}
void	Temp_H		(void)	{}
void	Temp_I		(void)	{}
//----------------------------------------------------------------------------------------------------
void  Welcome_Help		(void)	{Send_Data2Socket(Welcome_Help_Data,		sizeof(Welcome_Help_Data)-1);}
void  Info_Help			(void)	{Send_Data2Socket(Info_Help_Data,		sizeof(Info_Help_Data)-1);}
void  Network_Help		(void)	{Send_Data2Socket(Network_Help_Data,		sizeof(Network_Help_Data)-1);}
void  Virtual_Buttons_Help	(void)	{Send_Data2Socket(Virtual_Buttons_Help_Data,	sizeof(Virtual_Buttons_Help_Data)-1);}
void  File_System_Help		(void)	{Send_Data2Socket(File_System_Help_Data,	sizeof(File_System_Help_Data)-1);}
void  Temp_Help			(void)	{Send_Data2Socket(Temp_Help_Data,		sizeof(Temp_Help_Data)-1);}
void  Send_Comm_Error		(void)  {Send_Data2Socket("Can't send data\r\n",17);}
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 'A'				,Welcome_A					,Info,
 'B'				,Welcome_B					,Network,
 'C'				,Welcome_C					,Virtual_Buttons,
 'D'				,Welcome_D					,File_System,
 'E'				,Welcome_E					,Temp,
 '?'				,Welcome_Help					,Welcome,
 Data_Sended_Event		,Send_Comm_Error				,Welcome,
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
 'A'				,Virtual_Buttons_A				,Virtual_Buttons,
 'B'				,Virtual_Buttons_B				,Virtual_Buttons,
 'C'				,Virtual_Buttons_C				,Virtual_Buttons,
 'D'				,Virtual_Buttons_D				,Virtual_Buttons,
 '<'				,Rien 						,Welcome,
 '?'				,Virtual_Buttons_Help				,Virtual_Buttons,
 ANY_Event			,Rien						,Virtual_Buttons,
};
static const State File_System[] =
{
 'A'				,File_System_A					,File_System, 	// setea el nombre
 'B'				,File_System_B					,File_System, 	// crea
 'C'				,File_System_C					,File_System, 	// lee
 'D'				,File_System_D					,File_System, 	// escribe
 'E'				,File_System_E					,File_System, 	// info
 'F'				,File_System_F					,File_System, 	// borrar
 'G'				,File_System_G					,File_System, 	// copia a tmp
 'H'				,File_System_H					,File_System, 	// append
 '<'				,Rien 						,Welcome,
 '?'				,File_System_Help				,File_System,
 ANY_Event			,Rien						,File_System,
};
static const State Temp[] =
{
 'A'				,Temp_A					,Temp, 
 'B'				,Temp_B					,Temp, 
 'C'				,Temp_C					,Temp, 
 'D'				,Temp_D					,Temp, 
 'E'				,Temp_E					,Temp, 
 'F'				,Temp_F					,Temp, 
 'G'				,Temp_G					,Temp, 
 'H'				,Temp_H					,Temp, 
 'I'				,Temp_I					,Temp, 
 '<'				,Rien					,Welcome,
 '?'				,Temp_Help				,Temp,
 ANY_Event			,Rien					,Temp,

// Data_Ready_Event		,Parse_Tcp_Packet_And_Restore_Emac		,Temp,
// 'A'				,Begin_Send_One_Wire_Info2Tcp			,Sending_Tmp,
// 'B'				,Reload_One_Wire_Codes				,Temp,
// 'C'				,Load_Int4Tcp					,Temp,
// 'D'				,Send_Loaded_Int2Tcp				,Temp,
// 'E'				,Save_Reload_One_Wire_Nodes_TOut4Loaded_Int	,Temp,
// 'F'				,Send_Reload_One_Wire_Nodes_TOut2Tcp		,Temp,
// 'G'				,Inc_One_Wire_Read_Delay			,Temp,
// 'H'				,Dec_One_Wire_Read_Delay			,Temp,
// 'I'				,Send_One_Wire_Read_Delay2Tcp			,Temp,
};
//----------------------------------------------------------------------------------------------------

