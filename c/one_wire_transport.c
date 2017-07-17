#include "hw_types.h"
#include "hw_memmap.h"
#include "uart_if.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include <schedule.h>
#include "state_machine.h"
#include "events.h"
#include <one_wire_phisical.h>
#include <one_wire_network.h>
#include <one_wire_transport.h>
#include <type_conversion.h>
#include <string.h>
#include <checksum.h>
#include <debug.h>
#include <string.h>
#include <wifi_socket.h>

static const State
	Searching_Rom_Codes[],
	Broadcasting_T[],
	Broadcasting_V[],
	Broadcasting_Recalling_Page0[],
	Unicasting[],
	Parsing_Family_Code[],
	Reading_DS18S20_Scratchpad[],
	Reading_DS18B20_Scratchpad[],
	Reading_DS2438_Scratchpad[],
	Checking_Crcs[];

//#pragma section {Flash_UData}	
unsigned short int Reload_One_Wire_Nodes_TOut	=0;	//1 vez por minuto
//#pragma section ()

const State *One_Wire_Transport_Sm;
unsigned char Actual_Node,Snapshot_On_Line_Nodes,Actual_Sending_Node;
//------------------------------------------------------------------
const State** One_Wire_Transport(void)	{return &One_Wire_Transport_Sm;}
void Init_One_Wire_Transport(void)	
{
 Init_One_Wire_Network();
 Set_State(Searching_Rom_Codes,One_Wire_Transport());
 New_None_Periodic_Schedule(50,Timeout_1Sec_Event,One_Wire_Transport());	//2 segs
// if(Reload_One_Wire_Nodes_TOut) New_Periodic_Func_Schedule(10*Reload_One_Wire_Nodes_TOut,Reload_One_Wire_Codes);
 Init_Tmp_Control();
}
//------------------------------------------------------------------
void Refresh_Reload_One_Wire_Nodes_TOut		(unsigned short int Time)			{if(Time) Update_Func_Schedule(10*Time,Reload_One_Wire_Codes); else Free_Func_Schedule(Reload_One_Wire_Codes);}
void Save_Reload_One_Wire_Nodes_TOut		(unsigned short int TOut)	
{
 //unsigned short int Time=TOut>MIN_RELOAD_ONE_WIRE_NODES_TOUT?TOut:0;
 //Save_Int2Flash(&Reload_One_Wire_Nodes_TOut,Time);
 //Refresh_Reload_One_Wire_Nodes_TOut(Time);									//habra que ejecutar 2 veces esto para que lo tome porque sino tomara el valor anterior...
}
void Save_Reload_One_Wire_Nodes_TOut4Loaded_Int		(void)			{1;}//Save_Reload_One_Wire_Nodes_TOut(Dec_Bcd2Int(Socket_Rx_Buffer()));}
void Send_Reload_One_Wire_Nodes_TOut2Tcp		(void)			{1;}//Send_NLine_Int_NLine2Socket(Reload_One_Wire_Nodes_TOut);}
//------------------------------------------------------------------
void Reset_Actual_Node			(void)	{Actual_Node=0;}
void Inc_Actual_Node			(void)	{Actual_Node++;}
void Parse_Next_Family_Code		(void)	{Atomic_Send_Event(Actual_Node<On_Line_Nodes()?Family_Code(Actual_Node):End_Of_Nodes_Event,One_Wire_Transport());}
void Read_Actual_DS18S20_Scratchpad	(void)	{Read_DS18S20_Scratchpad(Actual_Node);DBG_ONE_WIRE_TRANSPORT_PRINT("DS18S20 scratchpad read\r\n");}
void Read_Actual_DS18B20_Scratchpad	(void)	{Read_DS18B20_Scratchpad(Actual_Node);DBG_ONE_WIRE_TRANSPORT_PRINT("DS18B20 scratchpad read\r\n");}
void Read_Actual_DS2438_Scratchpad0	(void)	{Read_DS2834_Scratchpad_Page(Actual_Node,0);DBG_ONE_WIRE_TRANSPORT_PRINT("DS2438 scratchpad read\r\n");}
void Reload_One_Wire_Codes		(void)	{Atomic_Send_Event(Reload_Codes_Event,One_Wire_Transport());}
//------------------------------------------------------------------
void Print_Nobody_On_Bus		(void)	{DBG_ONE_WIRE_TRANSPORT_PRINT("Nobody on Bus\r\n");}
void Print_All_Measured			(void)	{DBG_ONE_WIRE_TRANSPORT_PRINT("All Measured\r\n");}
//-------------------------------------------------
void Check_Crcs(void)
{
 unsigned char i=On_Line_Nodes();
 while(i && Read_One_Wire_Crc(i-1)>1) i--;						//con que uno este fallado, pagan todos...
 Atomic_Send_Event(i?Crc_Fail_Event:Crc_Ok_Event,One_Wire_Transport());		//sino llego a recorrer todos... falla.
}
//-------------------------------------------------
unsigned char Convert_Node_Bin2Ascci(unsigned char Node,unsigned char* Buf)
{
  if(Family_Code(Node)==DS18B20) return DS18S20_Convert_Bin2Ascci_T(Buf,Node);
  if(Family_Code(Node)==DS18S20) return DS18B20_Convert_Bin2Ascci_T(Buf,Node);
  if(Family_Code(Node)==DS2438)	 return DS2438_Convert_Bin2Ascci_T_V(Buf,Node);
 return 0;
}
void Send_One_Wire_Info2Tcp(void)
{
 static const unsigned char Header[]="\r\n000 Nodes\r\n";
 unsigned char Buf[46],Snapshot_On_Line_Nodes=On_Line_Nodes(),Actual_Sending_Node;
 String_Copy(Header,Buf,sizeof(Header)-1);
 Char2Bcd(Buf+2,Snapshot_On_Line_Nodes);
 Send_Data2Socket(Buf,sizeof(Header)-1);
 
 for(Actual_Sending_Node=0;Actual_Sending_Node<Snapshot_On_Line_Nodes;Actual_Sending_Node++)
  Send_Data2Socket(Buf,Convert_Node_Bin2Ascci(Actual_Sending_Node,Buf));
 Send_Data2Socket("EOF\r\n",5);
}

void Send_Temp2Tcp(void)	
{
 unsigned char Buf[46];
 if(!On_Line_Nodes())
  Send_Data2Socket("Not detected\r\n",14);
 else
 {
  Send_Data2Socket("Temp=    ",9);
  Send_Data2Socket(Buf+17,Convert_Node_Bin2Ascci(0,Buf)-29);
  Send_NLine2Socket();
 }
}
//------------------------------------------------------------------------
signed short int 	One_Wire_Bin		(unsigned char Pos)				{return (On_Line_Nodes()>Pos && Read_One_Wire_Crc(Pos))?Read_One_Wire_T(Pos):0x7FFF;} //si hay error devuelve el numero mas grande de signed int 0x7FFF
void 			One_Wire_T2Tcp		(unsigned char Node)				{unsigned char Buf[25];Send_Data2Socket(Buf,One_Wire_Bin2Ascci(Node,Buf));}
unsigned char 		One_Wire_Bin2Ascci	(unsigned char Node,unsigned char *Buf)	
{
 const unsigned char T_Template[]="00002 T =+000.00 C ST=2\r\n";
 String_Copy(T_Template,Buf,sizeof(T_Template)-1);
 Char2Bcd(Buf+2,Node+2);
 	if(!On_Line_Nodes()) String_Copy("1",Buf+22,1);
 else 	if(Read_One_Wire_Crc(Node)) {String_Copy("0",Buf+22,1);Signed_Int2_2Dec_Fix_Point_Bcd(Buf+9,Read_One_Wire_T(Node));};
 return sizeof(T_Template)-1;
}
//------------------------------------------------------------------------
unsigned short int Convert_Nodes_Bin2Ascci(unsigned char* Buf)	
{
 static const unsigned char Header[]="000 Nodes\r\n";
 unsigned char Node,Length=sizeof(Header)-1;
 String_Copy(Header,Buf,sizeof(Header)-1);
 Char2Bcd(Buf,On_Line_Nodes());
 for(Node=0;Node<On_Line_Nodes();Node++)  Length+=Convert_Node_Bin2Ascci(Node,Buf+Length);
 return Length;
}
//-------------------------------------------------
void Reset_Actual_Node_And_Parse_Next_Family_Code				(void)	{Reset_Actual_Node();Parse_Next_Family_Code();}
void Inc_Actual_Node_And_Parse_Next_Family_Code					(void)	{Inc_Actual_Node();Parse_Next_Family_Code();}
void Print_All_Measured_And_Broadcast_V						(void)	{Print_All_Measured();Broadcast_V();}
void Print_Nobody_On_Bus_And_Wait1Sec						(void)	{Print_Nobody_On_Bus();None_Periodic_1Sec();}
void Calculate_DS18S20_12Bit_T_And_Inc_Actual_Node_And_Parse_Next_Family_Code	(void)	{Calculate_DS18S20_12Bit_T(Actual_Node);Inc_Actual_Node_And_Parse_Next_Family_Code();}
void Calculate_DS18B20_12Bit_T_And_Inc_Actual_Node_And_Parse_Next_Family_Code	(void)	{Calculate_DS18B20_12Bit_T(Actual_Node);Inc_Actual_Node_And_Parse_Next_Family_Code();}
void Calculate_DS2834_T_V_Inc_Actual_Node_And_Parse_Next_Family_Code		(void)	{Calculate_DS2834_T_V(Actual_Node);	Inc_Actual_Node_And_Parse_Next_Family_Code();}
void Free_Wait1Sec_And_Search_Codes						(void)	{Free_Schedule_1Sec();Search_Codes();}
void One_Wire_Power_On_Reset_And_Wait1Sec					(void)	{One_Wire_Power_On_Reset();None_Periodic_1Sec();}
//-------------------------------------------------
static const State Searching_Rom_Codes[]=
{
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec	,Searching_Rom_Codes,
 Timeout_1Sec_Event		,Search_Codes				,Searching_Rom_Codes,
 Anybody_On_Bus_Event		,Broadcast_V				,Broadcasting_V,
 Reload_Codes_Event		,Free_Wait1Sec_And_Search_Codes		,Searching_Rom_Codes,
 ANY_Event			,Rien					,Searching_Rom_Codes,
};
//----------------------------------------------
static const State Broadcasting_V[]=
{
 End_Of_One_Wire_Msg_Event	,Broadcast_T				,Broadcasting_T,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec	,Searching_Rom_Codes,
 Reload_Codes_Event		,Search_Codes				,Searching_Rom_Codes,
 ANY_Event			,Rien					,Broadcasting_V,
};
static const State Broadcasting_T[]=
{
 End_Of_One_Wire_Msg_Event	,None_Periodic_1Sec			,Broadcasting_Recalling_Page0,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec	,Searching_Rom_Codes,
 Reload_Codes_Event		,Search_Codes				,Searching_Rom_Codes,
 ANY_Event			,Rien					,Broadcasting_T,
};
static const State Broadcasting_Recalling_Page0[]=
{
 Timeout_1Sec_Event		,Broadcast_Recall_Page0			,Unicasting,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec	,Searching_Rom_Codes,
 Reload_Codes_Event		,Free_Wait1Sec_And_Search_Codes		,Searching_Rom_Codes,
 ANY_Event			,Rien					,Broadcasting_Recalling_Page0,
};
//----------------------------------------------
static const State Unicasting[]=
{
 End_Of_One_Wire_Msg_Event	,Reset_Actual_Node_And_Parse_Next_Family_Code	,Parsing_Family_Code,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec		,Searching_Rom_Codes,
 Reload_Codes_Event		,Search_Codes					,Searching_Rom_Codes,
 ANY_Event			,Rien						,Unicasting,
};
static const State Parsing_Family_Code[]=
{
 DS18S20			,Read_Actual_DS18S20_Scratchpad				,Reading_DS18S20_Scratchpad,
 DS18B20			,Read_Actual_DS18B20_Scratchpad				,Reading_DS18B20_Scratchpad,
 DS2438				,Read_Actual_DS2438_Scratchpad0				,Reading_DS2438_Scratchpad,
 End_Of_Nodes_Event		,Check_Crcs 						,Checking_Crcs,
 Reload_Codes_Event		,Search_Codes						,Searching_Rom_Codes,
 ANY_Event			,Inc_Actual_Node_And_Parse_Next_Family_Code		,Parsing_Family_Code, 	//si viene falmilia desconocida, pasa al siguiente (si hubiera) no estaa haciendo el incremento y se cogaba...
};
//----------------------------------------------
static const State Reading_DS18S20_Scratchpad[]=
{
 End_Of_One_Wire_Msg_Event	,Calculate_DS18S20_12Bit_T_And_Inc_Actual_Node_And_Parse_Next_Family_Code	,Parsing_Family_Code,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec						,Searching_Rom_Codes,
 Reload_Codes_Event		,Search_Codes									,Searching_Rom_Codes,
 ANY_Event			,Rien										,Reading_DS18S20_Scratchpad,
};
//----------------------------------------------
static const State Reading_DS18B20_Scratchpad[]=
{
 End_Of_One_Wire_Msg_Event	,Calculate_DS18B20_12Bit_T_And_Inc_Actual_Node_And_Parse_Next_Family_Code	,Parsing_Family_Code,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec						,Searching_Rom_Codes,
 Reload_Codes_Event		,Search_Codes									,Searching_Rom_Codes,
 ANY_Event			,Rien										,Reading_DS18B20_Scratchpad,
};
//----------------------------------------------
static const State Reading_DS2438_Scratchpad[]=
{
 End_Of_One_Wire_Msg_Event	,Calculate_DS2834_T_V_Inc_Actual_Node_And_Parse_Next_Family_Code		,Parsing_Family_Code,
 Nobody_On_Bus_Event		,Print_Nobody_On_Bus_And_Wait1Sec						,Searching_Rom_Codes,
 Reload_Codes_Event		,Search_Codes									,Searching_Rom_Codes,
 ANY_Event			,Rien										,Reading_DS2438_Scratchpad,
};
//----------------------------------------------
static const State Checking_Crcs[]=
{
 Crc_Ok_Event			,Print_All_Measured_And_Broadcast_V		,Broadcasting_V,		//si esta todo ok, sigue..
 Crc_Fail_Event			,One_Wire_Power_On_Reset_And_Wait1Sec		,Searching_Rom_Codes,		//si fallo OJO que esto entra si el CRC llego a '1' desde un valor mayor..., APAGA el bus... durante 1 segundo y busca nodos nuevamente...
 Reload_Codes_Event		,Search_Codes					,Searching_Rom_Codes,		//
 ANY_Event			,Rien						,Checking_Crcs,
};
//----------------------------------------------



