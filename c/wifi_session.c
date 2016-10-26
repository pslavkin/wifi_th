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

static const State   
	Welcome[],
	Info[],
	Network[],
	Virtual_Buttons[],
	File_System[];

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
"< Back\r\n"
"? Help\r\n"
};
//------------WELCOME----------------------------------------------------------------------------------------
void  Welcome_A	(void)	{}
void  Welcome_B	(void)	{}
void  Welcome_C	(void)	{}
void  Welcome_D	(void)	{}
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
	String_Copy(Actual_Rx_Buff4Sm(),Actual_Rx_Buff4Sm()+10,3);
	Actual_Rx_Buff4Sm()[13]=0; 					//fuerzo fin del nombre..	
	DBG_WIFI_SESSION_PRINT("Nombre %s\r\n",Actual_Rx_Buff4Sm()+10);
}
void	File_System_B4Buff	(void)	{
	Create_File(Actual_Rx_Buff4Sm()+10,Dec_Bcd2Char(Actual_Rx_Buff4Sm()));
	DBG_WIFI_SESSION_PRINT("Archivo %s creado. Tamanio %d \r\n",Actual_Rx_Buff4Sm()+10,Dec_Bcd2Char(Actual_Rx_Buff4Sm()));
}
void	File_System_C4Buff	(void)	{
	unsigned char Buf[20];
	unsigned short int Ret;
	Ret=Read_File(Actual_Rx_Buff4Sm()+10,Buf,Dec_Bcd2Char(Actual_Rx_Buff4Sm()));
	Send_Data2Socket4Sm(Buf,Ret);
	DBG_WIFI_SESSION_PRINT("Leyendo desde %s tamanio %d data %s \r\n",Actual_Rx_Buff4Sm()+10,Dec_Bcd2Char(Actual_Rx_Buff4Sm()),Buf);
}
void	File_System_D4Buff	(void)	{
	Write_File(Actual_Rx_Buff4Sm()+10,Actual_Rx_Buff4Sm(),10);
	DBG_WIFI_SESSION_PRINT("grabando en %s datos %s tamanio fijo 10 \r\n",Actual_Rx_Buff4Sm()+10,Actual_Rx_Buff4Sm());
}
void	File_System_E4Buff	(void)	{
	SlFsFileInfo_t Fileinfo;
	Info_File(Actual_Rx_Buff4App()+10,&Fileinfo);
	Send_Data2Socket("Fill=",5);
	Send_Int_NLine2Socket(Fileinfo.FileLen);
	Send_Data2Socket("Size=",5);
	Send_Int_NLine2Socket(Fileinfo.AllocatedLen);
	DBG_WIFI_SESSION_PRINT("Info de archivo %s len= %d usado = %d\r\n",Actual_Rx_Buff4App()+10,Fileinfo.FileLen,Fileinfo.AllocatedLen);
}
void	File_System_F4Buff	(void)	{
	Del_File(Actual_Rx_Buff4App()+10);
	Send_Data2Socket("deleted\r\n ",9);
	DBG_WIFI_SESSION_PRINT("archivo %s borrado\r\n",Actual_Rx_Buff4App()+10);
}
//------------------
void	File_System_A		(void)	{Config2Save( 3,File_System_A4Buff);}
void	File_System_B		(void)	{Config2Save( 3,File_System_B4Buff);}
void	File_System_C		(void)	{Config2Save( 3,File_System_C4Buff);}
void	File_System_D		(void)	{Config2Save(10,File_System_D4Buff);}
void	File_System_E		(void)	{File_System_E4Buff();}
void	File_System_F		(void)	{File_System_F4Buff();}
//----------------------------------------------------------------------------------------------------
void  Welcome_Help		(void)	{Send_Data2Socket(Welcome_Help_Data,		sizeof(Welcome_Help_Data)-1);}
void  Info_Help			(void)	{Send_Data2Socket(Info_Help_Data,		sizeof(Info_Help_Data)-1);}
void  Network_Help		(void)	{Send_Data2Socket(Network_Help_Data,		sizeof(Network_Help_Data)-1);}
void  Virtual_Buttons_Help	(void)	{Send_Data2Socket(Virtual_Buttons_Help_Data,	sizeof(Virtual_Buttons_Help_Data)-1);}
void  File_System_Help		(void)	{Send_Data2Socket(File_System_Help_Data,	sizeof(File_System_Help_Data)-1);}
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 'A'				,Welcome_A					,Info,
 'B'				,Welcome_B					,Network,
 'C'				,Welcome_C					,Virtual_Buttons,
 'D'				,Welcome_D					,File_System,
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
 '<'				,Rien 						,Welcome,
 '?'				,File_System_Help				,File_System,
 ANY_Event			,Rien						,File_System,
};
//----------------------------------------------------------------------------------------------------

