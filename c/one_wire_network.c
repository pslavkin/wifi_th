#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "state_machine.h"
#include "events.h"
#include <one_wire_phisical.h>
#include <type_conversion.h>
#include <one_wire_network.h>
#include <string.h>
#include <debug.h>
#include <checksum.h>
#include <one_wire_transport.h>

static const State
	Idle[],
	Detecting[],
	Writing_Reading[],
	Detecting4Searching_Rom[],
	Searching_Rom[],
	Colisioning[],
	Parsing_Last_Code_Bit[],
	Checking_Rom_Codes_Crc[];

struct Rom_Codes_Struct Rom_Codes[MAX_ROM_CODES];
static unsigned char Actual_Bit,Actual_Code,Actual_Marker,Last_Marker;
static unsigned char Rx_Buffer[ONE_WIRE_RX_BUFFER],Bytes2Read,*Point2Write,*Point2Read;
State *One_Wire_Network_Sm;
//------------------------------------------------------------------
State** One_Wire_Network		(void)			{return &One_Wire_Network_Sm;}
//------------------------------------------------------------------
unsigned char  	One_Wire_Rx_As_Char	(unsigned char Pos)	{return *(unsigned char*)(Rx_Buffer+Pos);}
unsigned char* 	One_Wire_Rx_As_PChar	(unsigned char Pos)	{return (unsigned char*) (Rx_Buffer+Pos);}
unsigned int 	One_Wire_Rx_As_Int	(unsigned char Pos)	{return *(unsigned int*) (Rx_Buffer+Pos);}
//---------------------------------
void Execute_Cmd(unsigned char Length,unsigned char* Cmd)
{
 Bytes2Read=Length;
 Point2Write=Cmd;
 Point2Read=One_Wire_Rx_As_PChar(0);
 Atomic_Send_Event(One_Wire_Cmd_Event,One_Wire_Network());
}
//----------------------------
void Write_Read_Next_Byte	(void)
{
 Point2Read[--Bytes2Read]=Write_Read_Byte(Point2Write[Bytes2Read]);
 Atomic_Send_Event(Bytes2Read?One_Wire_Read_Next_Byte_Event:One_Wire_End_Of_Read_Event,One_Wire_Network());
}
//-------------------------------------------------
void 		Read_Presence		(void)				{Atomic_Send_Event(Presence()?One_Wire_Not_Detected_Event:One_Wire_Detected_Event,One_Wire_Network());}
void 		Search_Codes		(void)				{Atomic_Send_Event(Search_Codes_Event,One_Wire_Network());}
void 		Search_Rom		(void)				{Write_Read_Byte(SEARCH_ROM);}
void 		Read_Rom		(void)				{Execute_Cmd(9,READ_ROM_STRING);}
void 		Match_Rom		(unsigned char* Rom_Code)	{Execute_Cmd(9,Rom_Code);}
void 		Skip_Rom		(void)				{Execute_Cmd(1,SKIP_ROM_STRING);}
void 		Broadcast_T		(void)				{Execute_Cmd(2,"\x44\xCC");}
void 		Broadcast_V		(void)				{Execute_Cmd(2,"\xB4\xCC");}
void 		Broadcast_Recall_Page0	(void)				{Execute_Cmd(3,"\x00\xB8\xCC");}
unsigned char 	Read_One_Wire_Crc	(unsigned char Node)		{return Rom_Codes[Node].Crc;}
unsigned int 	Read_One_Wire_T		(unsigned char Node)		{return Rom_Codes[Node].T;}
void 		Check_Rom_Codes_Crc	(void)				
{
 unsigned char Codes;
 for(Codes=Actual_Code;Codes && One_Wire_Crc(Rom_Codes[Codes-1].Code,sizeof(Rom_Codes[0].Code));Codes--);	//revisa el CRC de toda la lista. Si todos estan ok, Codes sale encero...
 Atomic_Send_Event(Codes?Crc_Fail_Event:Crc_Ok_Event,One_Wire_Network());
}
//------------------------- DS18S20 ----------------------------------
void Read_DS18S20_Scratchpad	(unsigned char Node)			
{
 static const unsigned char Scratchpad_Template[]="\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xBE\x00\x00\x00\x00\x00\x00\x00\x00\x55";
 String_Copy(Scratchpad_Template,One_Wire_Rx_As_PChar(0),sizeof(Scratchpad_Template)-1);
 String_Copy(Rom_Codes[Node].Code,One_Wire_Rx_As_PChar(10),sizeof(Rom_Codes[0].Code));
 Execute_Cmd(sizeof(Scratchpad_Template)-1,One_Wire_Rx_As_PChar(0));
}
void Calculate_DS18S20_12Bit_T	(unsigned char Node)	
{
 if(One_Wire_Crc(One_Wire_Rx_As_PChar(0),9))
 {
  Rom_Codes[Node].T=(*(signed int*)(One_Wire_Rx_As_PChar(7)))*50 - 25 + (((One_Wire_Rx_As_Char(1)-One_Wire_Rx_As_Char(2))*100)/One_Wire_Rx_As_Char(1));			//ver documento DS18S20 pag. 5
  Rom_Codes[Node].Crc=CRC_FILTER;
 }
 else if(Rom_Codes[Node].Crc) Rom_Codes[Node].Crc--;
}
unsigned char DS18S20_Convert_Bin2Ascci_T		(unsigned char* Destiny,unsigned char Code)
{
 const unsigned char Code_T_Template[]="0123456789012345=+327.67 C Crc=Ok \r\n";
 String_Copy(Code_T_Template,Destiny,sizeof(Code_T_Template)-1);
 String2Hex_Bcd(Destiny,Rom_Codes[Code].Code,sizeof(Rom_Codes[0].Code));
 Signed_Int2_2Dec_Fix_Point_Bcd(Destiny+17,Rom_Codes[Code].T);
 if(!Rom_Codes[Code].Crc) String_Copy("Err",Destiny+sizeof(Code_T_Template)-6,3);
 return sizeof(Code_T_Template)-1;
}
//------------------------- DS18B20 ----------------------------------
void Read_DS18B20_Scratchpad	(unsigned char Node)			
{
 static const unsigned char Scratchpad_Template[]="\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xBE\x00\x00\x00\x00\x00\x00\x00\x00\x55";
 String_Copy(Scratchpad_Template,One_Wire_Rx_As_PChar(0),sizeof(Scratchpad_Template)-1);
 String_Copy(Rom_Codes[Node].Code,One_Wire_Rx_As_PChar(10),sizeof(Rom_Codes[0].Code));
 Execute_Cmd(sizeof(Scratchpad_Template)-1,One_Wire_Rx_As_PChar(0));
}
void Calculate_DS18B20_12Bit_T	(unsigned char Node)	
{
 if(One_Wire_Crc(One_Wire_Rx_As_PChar(0),9)) 
  {
    Rom_Codes[Node].T=(unsigned long)One_Wire_Rx_As_Int(7)*100/16;			//ver documento DS18B20 pag.4, se multiplica por 100 para tener 2 decimales y se divide por 16 porque la parte decimal del sensor son 4 bits...
    Rom_Codes[Node].Crc=CRC_FILTER;
  }
 else if(Rom_Codes[Node].Crc) Rom_Codes[Node].Crc--;
}
unsigned char DS18B20_Convert_Bin2Ascci_T		(unsigned char* Destiny,unsigned char Code)
{
 const unsigned char Code_T_Template[]="0123456789012345=+327.67 C Crc=Ok \r\n";
 String_Copy(Code_T_Template,Destiny,sizeof(Code_T_Template)-1);
 String2Hex_Bcd(Destiny,Rom_Codes[Code].Code,sizeof(Rom_Codes[0].Code));
 Signed_Int2_2Dec_Fix_Point_Bcd(Destiny+17,Rom_Codes[Code].T);
 if(!Rom_Codes[Code].Crc) String_Copy("Err",Destiny+sizeof(Code_T_Template)-6,3);
 return sizeof(Code_T_Template)-1;
}
//------------------------- DS2834 ----------------------------------
void Read_DS2834_Scratchpad_Page	(unsigned char Node,unsigned char Page)	
{
 static const unsigned char Scratchpad_Template[]="\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\xBE\x00\x00\x00\x00\x00\x00\x00\x00\x55";
 String_Copy(Scratchpad_Template,One_Wire_Rx_As_PChar(0),sizeof(Scratchpad_Template)-1);
 String_Copy(Rom_Codes[Node].Code,One_Wire_Rx_As_PChar(11),sizeof(Rom_Codes[0].Code));
 *One_Wire_Rx_As_PChar(9)=Page;
 Execute_Cmd(sizeof(Scratchpad_Template)-1,One_Wire_Rx_As_PChar(0));
}
void Calculate_DS2834_T_V		(unsigned char Node)			
{
 if(One_Wire_Crc(One_Wire_Rx_As_PChar(0),9))
 {
  Rom_Codes[Node].T=One_Wire_Rx_As_Char(6)*100+(One_Wire_Rx_As_Char(7)*100)/256;	//ver datasheet de DS2438 pagina 6
  Rom_Codes[Node].V=One_Wire_Rx_As_Int(4);
  Rom_Codes[Node].Crc=CRC_FILTER;
 }
 else if(Rom_Codes[Node].Crc) Rom_Codes[Node].Crc--;
}
unsigned char DS2438_Convert_Bin2Ascci_T_V		(unsigned char* Destiny,unsigned char Code)
{
 const unsigned char Code_T_V_Template[]="0123456789012345=+327.67 C +327.67 V Crc=Ok \r\n";
 String_Copy(Code_T_V_Template,Destiny,sizeof(Code_T_V_Template)-1);
 String2Hex_Bcd(Destiny,Rom_Codes[Code].Code,sizeof(Rom_Codes[0].Code));
 Signed_Int2_2Dec_Fix_Point_Bcd(Destiny+17,Rom_Codes[Code].T);
 Signed_Int2_2Dec_Fix_Point_Bcd(Destiny+27,Rom_Codes[Code].V);
 if(!Rom_Codes[Code].Crc) String_Copy("Err",Destiny+sizeof(Code_T_V_Template)-6,3);
 return sizeof(Code_T_V_Template)-1;
}
//-----------------SEARCH ROM FUNC-----------------------------------
void Reset_Actual_Bit		(void)			{Actual_Bit=64;}
void Reset_Actual_Marker	(void)			{Actual_Marker=64;}
void Reset_Last_Marker		(void)			{Last_Marker=64;}
void Reset_Actual_Code		(void)			{Actual_Code=0;}
void Inc_Actual_Code		(void)			{Actual_Code++;}
void Init_Markers		(void)			{Reset_Actual_Bit();Reset_Actual_Marker();Reset_Last_Marker();Reset_Actual_Code();}
void Actual_Marker2Last		(void)			{Last_Marker=Actual_Marker;}
void Actual_Bit2Marker		(void)			{Actual_Marker=Actual_Bit;}
void Mark_All_Crc_Fail		(void)			{unsigned char i=Actual_Code;while(i--) Rom_Codes[i].Crc=0;}
//-------------------------------------------------
void Bit_Colision		(void)			{Atomic_Send_Event(Actual_Bit<Last_Marker?Smaller_Discrepance_Event:(Actual_Bit==Last_Marker)?Equal_Discrepance_Event:Bigger_Discrepance_Event,One_Wire_Network());Send_NVDebug_One_Wire_Network_Data2Serial(9,"Colision\n");}
void Select_Bit_One		(void)			{Write_Bit_One_And_Read();Set_Bit_On_String(Rom_Codes[Actual_Code].Code,Actual_Bit);}
void Select_Bit_Zero		(void)			{Write_Bit_Zero();Clear_Bit_On_String(Rom_Codes[Actual_Code].Code,Actual_Bit);}
void Search_Next_Bit		(void)			{Atomic_Send_Event(Actual_Bit--?Read2Bits():Actual_Code_End_Event,One_Wire_Network());}
void Search_Next_Code		(void)			
{
 DBG_ONE_WIRE_NETWORK_PRINT("Next Code\n");
 Actual_Marker2Last();
 Reset_Actual_Bit();
 Reset_Actual_Marker();
 Inc_Actual_Code();
 (Last_Marker<64 && Actual_Code<MAX_ROM_CODES)?Read_Presence():Atomic_Send_Event(Search_Codes_End_Event,One_Wire_Network());	//si el marcador es menor que 64 quiere decir que hay que resolver una colision, y eso implica que hay mas dispositivos que detetar. Por otro lado no podemos detectar mas nodos que el lugar disponible....
}
void Send_Last_Code_Bit		(void)			{Atomic_Send_Event(Read_Bit4String(Rom_Codes[Actual_Code-1].Code,Actual_Bit),One_Wire_Network());}
//-------------------------------------------------
unsigned char 	Family_Code		(unsigned char Node)	{return Rom_Codes[Node].Code[7];}
unsigned char 	On_Line_Nodes		(void)			{return Actual_Code;}
void 		Ans_Anybody2App		(void)			{Atomic_Send_Event(Anybody_On_Bus_Event,One_Wire_Transport());}
void 		Ans_Nobody2App		(void)			{Atomic_Send_Event(Nobody_On_Bus_Event,One_Wire_Transport());}
void 		Ans_End_Of_Msg2App	(void)			{Atomic_Send_Event(End_Of_One_Wire_Msg_Event,One_Wire_Transport());}
//-------------------------------------------------
void Print_Rom_Codes		(void)			{Send_NVData2Serial(Convert_Nodes_Bin2Ascci(Serial_Tx_As_PChar(0)),Serial_Tx_As_PChar(0));}
void Print_Detected		(void)			{DBG_ONE_WIRE_NETWORK_PRINT("Detected\n");}
void Print_Not_Detected		(void)			{DBG_ONE_WIRE_NETWORK_PRINT("Not Detected\n");}
void Print_Bit_Error		(void)			{DBG_ONE_WIRE_NETWORK_PRINT("Bit Error\n");}
void Print_Crc_Ok		(void)			{DBG_ONE_WIRE_NETWORK_PRINT("Crc Ok\n");}
void Print_Crc_Fail		(void)			{DBG_ONE_WIRE_NETWORK_PRINT("Crc Fail\n");}
//-------------------------------------------------
void Init_One_Wire_Network	(void)			{Init_One_Wire_Link();Set_State(Idle,One_Wire_Network());Reset_Actual_Code();}
//-------------------------------------------------
void Print_Detected_And_Write_Read_Next_Byte			(void)	{Print_Detected();Write_Read_Next_Byte();}
void Read_Presence_And_Init_Markers				(void)	{Read_Presence();Init_Markers();}
void Print_Detected_And_Search_Rom_And_Search_Next_Bit		(void)	{Print_Detected();Search_Rom();Search_Next_Bit();}
void Select_Bit_One_And_Search_Next_Bit				(void)	{Select_Bit_One();Search_Next_Bit();}
void Select_Bit_Zero_And_Search_Next_Bit			(void)	{Select_Bit_Zero();Search_Next_Bit();}
void Select_Bit_Zero_And_Actual_Bit2Marker_And_Search_Next_Bit	(void)	{Select_Bit_Zero();Actual_Bit2Marker();Search_Next_Bit();}
void Print_Crc_Fail_And_Reset_Actual_Code			(void)	{Print_Crc_Fail();Reset_Actual_Code();}
void Print_Crc_Ok_And_Ans_Anybody2App				(void)	{Print_Crc_Ok();Ans_Anybody2App();}
void Print_Not_Detected_And_Ans_Nobody2App			(void)	{Print_Not_Detected();Ans_Nobody2App();}
void Print_Not_Detected_And_Ans_Nobody2App_And_Reset_Actual_Code(void)	{Print_Not_Detected_And_Ans_Nobody2App();Reset_Actual_Code();}
void Print_Crc_Ok_And_Ans_Anybody2App_And_Mark_All_Crc_Fail	(void)	{Print_Crc_Ok();Ans_Anybody2App();Mark_All_Crc_Fail();}
void Print_Bit_Error_And_Ans_Nobody2App_And_Mark_All_Crc_Fail	(void)	{Print_Bit_Error();Ans_Nobody2App();Mark_All_Crc_Fail();}
//-------------------------------------------------
static const State Idle[] =
{
 One_Wire_Cmd_Event		,Read_Presence					,Detecting,
 Search_Codes_Event		,Read_Presence_And_Init_Markers			,Detecting4Searching_Rom,
 ANY_Event			,Rien						,Idle,
};

static const State Detecting[] =
{
 One_Wire_Detected_Event	,Print_Detected_And_Write_Read_Next_Byte			,Writing_Reading,
 One_Wire_Not_Detected_Event	,Print_Not_Detected_And_Ans_Nobody2App_And_Reset_Actual_Code	,Idle,	
 Search_Codes_Event		,Read_Presence_And_Init_Markers					,Detecting4Searching_Rom,
 ANY_Event			,Rien								,Detecting,
};
static const State Writing_Reading[] =
{
 One_Wire_Read_Next_Byte_Event	,Write_Read_Next_Byte				,Writing_Reading,
 One_Wire_End_Of_Read_Event	,Ans_End_Of_Msg2App				,Idle,
 Search_Codes_Event		,Read_Presence_And_Init_Markers			,Detecting4Searching_Rom,
 ANY_Event			,Rien						,Writing_Reading,
};
//-----------------------------
static const State Detecting4Searching_Rom[] =
{
 One_Wire_Detected_Event	,Print_Detected_And_Search_Rom_And_Search_Next_Bit	,Searching_Rom,
 One_Wire_Not_Detected_Event	,Print_Not_Detected_And_Ans_Nobody2App			,Idle,
 Search_Codes_End_Event		,Check_Rom_Codes_Crc					,Checking_Rom_Codes_Crc, 
 ANY_Event			,Rien							,Detecting4Searching_Rom,
};
static const State Searching_Rom[] =
{
 0x0000				,Bit_Colision							,Colisioning,
 0x0001				,Select_Bit_One_And_Search_Next_Bit				,Searching_Rom,
 0x0002				,Select_Bit_Zero_And_Search_Next_Bit				,Searching_Rom,
 0x0003				,Print_Bit_Error_And_Ans_Nobody2App_And_Mark_All_Crc_Fail	,Idle,
 Actual_Code_End_Event		,Search_Next_Code						,Detecting4Searching_Rom,
 ANY_Event			,Rien								,Searching_Rom,
};
//-----------------------------
static const State Colisioning[] =
{
 Bigger_Discrepance_Event	,Send_Last_Code_Bit						,Parsing_Last_Code_Bit,
 Equal_Discrepance_Event	,Select_Bit_One_And_Search_Next_Bit				,Searching_Rom,
 Smaller_Discrepance_Event	,Select_Bit_Zero_And_Actual_Bit2Marker_And_Search_Next_Bit	,Searching_Rom,
 ANY_Event			,Rien								,Colisioning,
};
static const State Parsing_Last_Code_Bit[] =
{
 0x0001				,Select_Bit_One_And_Search_Next_Bit				,Searching_Rom,
 0x0000				,Select_Bit_Zero_And_Actual_Bit2Marker_And_Search_Next_Bit	,Searching_Rom,
 ANY_Event			,Rien								,Parsing_Last_Code_Bit,
};
//---------------------------------------------
static const State Checking_Rom_Codes_Crc[] =
{
 Crc_Ok_Event			,Print_Crc_Ok_And_Ans_Anybody2App_And_Mark_All_Crc_Fail		,Idle,
 Crc_Fail_Event			,Print_Crc_Fail_And_Reset_Actual_Code				,Idle,
 ANY_Event			,Rien								,Checking_Rom_Codes_Crc,
};
