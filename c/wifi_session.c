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
#include "spi_session.h"
#include "adc.h"
#include "bat.h"
#include "clip.h"
#include "buttons.h"
#include "spi_phisical.h"
#include "parameters.h"
#include "../driverlib/adc.h"

static const State   
	Welcome[],
	Time1[],
	Time2[],
	Channel_N[],
	Control[],
	Sync[],
	Info[],
	Network[],
	Bat[],
	Clip[],
	Virtual_Buttons[];

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
"www.justbody.com.ar\r\n"
"v3.1.2\r\n"
"A Time1\r\n"
"B Time2\r\n"
"C Channels\r\n"
"D Control\r\n"
"E Info\r\n"
"F Network\r\n"
"G Bat\r\n"
"H Buttons\r\n"
"I Clip\r\n"
"J Sync\r\n"
"? Help\r\n"
};
unsigned char Time1_Help_Data[]=
{
"<-------------------------------- G repeticiones de C + D + E + F --------------------------------------->\r\n"
"                              <----------C repeticiones de B---------->         <------------D----------->\r\n"
"-<----B---><----B---><----B---><----B---><----B---><----B---><----B---><----B--->\r\n"
" >A<\r\n"
"                     |-|       |-|       |-|       |-|\r\n"
"           |--       | |       | |       | |       | |        |--\r\n"
" |-|       | |       | |       | |       | |       | |        | |       |-|\r\n"
"-| | ------| | ------| | ------| | ------| | ------| | -------| | ------| | ------------------------------\r\n"
"   |_|       | |       | |       | |       | |       | |        | |       |_|\r\n"
"             |--       | |       | |       | |       | |        |--\r\n" 
"                       |_|       |_|       |_|       |_|\r\n"
"-<--------E---------->                                          <----------F------>\r\n"
"A Set  Time A\r\n"
"B Set  Time B\r\n"
"C Set  Time C\r\n"
"D Set  Time D\r\n"
"E Set  Time E\r\n"
"F Set  Time F\r\n"
"G Set  Time G\r\n"
"H Read Time A\r\n"
"I Read Time B\r\n"
"J Read Time C\r\n"
"K Read Time D\r\n"
"L Read Time E\r\n"
"M Read Time F\r\n"
"N Read Time G\r\n"
"O Choose me\r\n"
"< Back\r\n"
"? Help\r\n"
"\r\n"
};
unsigned char Control_Help_Data[]=
{
"A Start\r\n"
"B Stop\r\n"
"C Pause\r\n"
"D Set  Time\r\n"
"E Read Time\r\n"
"F Switch Off\r\n"
"G Is working?\r\n"
"H Begin Secuence Channels\r\n"
"I End Secuence Channels\r\n"
"< Back\r\n"
"? Help\r\n"
};

unsigned char Sync_Help_Data[]=
{
"A Wait D\r\n"
"< Back\r\n"
"? Help\r\n"
};

unsigned char Channel_N_Help_Data[]=
{
"A Inc Power\r\n"
"B Dec Power\r\n"
"C Set Power\r\n"
"D Read Power\r\n"
"E Read Power All\r\n"
"F Inc Power All\r\n"
"G Dec Power All\r\n"
"< Back\r\n"
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
unsigned char Bat_Help_Data[]=
{
"A Bat Level\r\n"
"B Charger Connected?\r\n"
"C Charging?\r\n"
"D Do Care Charger\r\n"
"E Don't Care Charger\r\n"
//"F Adc1\r\n"
//"G Adc2\r\n"
//"H Adc3\r\n"
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
"E 2 Pressed\r\n"
"F 2 Holded\r\n"
"G 2 Holding\r\n"
"H 2 Released\r\n"
"I 3 Pressed\r\n"
"J 3 Holded\r\n"
"K 3 Holding\r\n"
"L 3 Released\r\n"
"M 3 Holded Released\r\n"
"< Back\r\n"
"? Help\r\n"
};
unsigned char Clip_Help_Data[]=
{
"A Clip State\r\n"
"B Do Care Clip\r\n"
"C Don't Care Clip\r\n"
"< Back\r\n"
"? Help\r\n"
};
//------------WELCOME----------------------------------------------------------------------------------------
void  Welcome_A	(void)	{}
void  Welcome_B	(void)	{}
void  Welcome_C	(void)	{}
void  Welcome_D	(void)	{}
void  Welcome_E	(void)	{}
void  Welcome_F	(void)	{}
void  Welcome_G	(void)	{}
void  Welcome_H	(void)	{}
void  Welcome_I	(void)	{}
void  Welcome_J	(void)	{}
//------------TIME1----------------------------------------------------------------------------------------
void  Time1_Set_A4Buff 	(void)	{unsigned char      A=Dec_Bcd2Char(Actual_Rx_Buff4Sm());if(A>=5  && A<=50)    Set_A1(A);}
void  Time1_Set_B4Buff 	(void)	{unsigned short int B=Dec_Bcd2Int(Actual_Rx_Buff4Sm()); if(B>=10 && B<=5000)  Set_B1(B);}
void  Time1_Set_C4Buff 	(void)	{unsigned short int C=Dec_Bcd2Int(Actual_Rx_Buff4Sm()); if(C>=0  && C<=60000) Set_C1(C);}
void  Time1_Set_D4Buff 	(void)	{Set_D1(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Time1_Set_E4Buff 	(void)	{Set_E1(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Time1_Set_F4Buff 	(void)	{Set_F1(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Time1_Set_G4Buff 	(void)	{Set_G1(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
//----------
void  Time1_Set_A	(void)	{Config2Save(3,Time1_Set_A4Buff); }
void  Time1_Set_B	(void)	{Config2Save(5,Time1_Set_B4Buff); }
void  Time1_Set_C	(void)	{Config2Save(5,Time1_Set_C4Buff); }
void  Time1_Set_D	(void)	{Config2Save(3,Time1_Set_D4Buff); }
void  Time1_Set_E	(void)	{Config2Save(3,Time1_Set_E4Buff); }
void  Time1_Set_F	(void)	{Config2Save(3,Time1_Set_F4Buff); }
void  Time1_Set_G	(void)	{Config2Save(3,Time1_Set_G4Buff); }
//----------
void  Time1_Read_A	(void)	{Send_Char_NLine2Socket(Read_A1());}
void  Time1_Read_B	(void)	{Send_Int_NLine2Socket (Read_B1());}
void  Time1_Read_C	(void)	{Send_Int_NLine2Socket (Read_C1());}
void  Time1_Read_D	(void)	{Send_Char_NLine2Socket(Read_D1());}
void  Time1_Read_E	(void)	{Send_Char_NLine2Socket(Read_E1());}
void  Time1_Read_F	(void)	{Send_Char_NLine2Socket(Read_F1());}
void  Time1_Read_G	(void)	{Send_Char_NLine2Socket(Read_G1());}

//------------TIME2----------------------------------------------------------------------------------------
void  Time2_Set_A4Buff 	(void)	{unsigned char      A=Dec_Bcd2Char(Actual_Rx_Buff4Sm());if(A>=5  && A<=50)    Set_A2(A);}
void  Time2_Set_B4Buff 	(void)	{unsigned short int B=Dec_Bcd2Int(Actual_Rx_Buff4Sm()); if(B>=10 && B<=5000)  Set_B2(B);}
void  Time2_Set_C4Buff 	(void)	{unsigned short int C=Dec_Bcd2Int(Actual_Rx_Buff4Sm()); if(C>=1  && C<=60000) Set_C2(C);}
void  Time2_Set_D4Buff 	(void)	{Set_D2(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Time2_Set_E4Buff 	(void)	{Set_E2(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Time2_Set_F4Buff 	(void)	{Set_F2(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Time2_Set_G4Buff 	(void)	{Set_G2(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
//----------
void  Time2_Set_A	(void)	{Config2Save(3,Time2_Set_A4Buff); }
void  Time2_Set_B	(void)	{Config2Save(5,Time2_Set_B4Buff); }
void  Time2_Set_C	(void)	{Config2Save(5,Time2_Set_C4Buff); }
void  Time2_Set_D	(void)	{Config2Save(3,Time2_Set_D4Buff); }
void  Time2_Set_E	(void)	{Config2Save(3,Time2_Set_E4Buff); }
void  Time2_Set_F	(void)	{Config2Save(3,Time2_Set_F4Buff); }
void  Time2_Set_G	(void)	{Config2Save(3,Time2_Set_G4Buff); }
//----------
void  Time2_Read_A	(void)	{Send_Char_NLine2Socket(Read_A2());}
void  Time2_Read_B	(void)	{Send_Int_NLine2Socket (Read_B2());}
void  Time2_Read_C	(void)	{Send_Int_NLine2Socket (Read_C2());}
void  Time2_Read_D	(void)	{Send_Char_NLine2Socket(Read_D2());}
void  Time2_Read_E	(void)	{Send_Char_NLine2Socket(Read_E2());}
void  Time2_Read_F	(void)	{Send_Char_NLine2Socket(Read_F2());}
void  Time2_Read_G	(void)	{Send_Char_NLine2Socket(Read_G2());}
//------------ CHANNEL N----------------------------------------------------------------------------------------
void  Channel_N_A4Buff 	(void)	{Inc_Wished_Power(Dec_Bcd2Char(Actual_Rx_Buff4Sm()),Dec_Bcd2Char(Actual_Rx_Buff4Sm()+3));}
void  Channel_N_B4Buff 	(void)	{Dec_Wished_Power(Dec_Bcd2Char(Actual_Rx_Buff4Sm()),Dec_Bcd2Char(Actual_Rx_Buff4Sm()+3));}
void  Channel_N_C4Buff 	(void)	{Set_Wished_Power(Dec_Bcd2Char(Actual_Rx_Buff4Sm()),Dec_Bcd2Char(Actual_Rx_Buff4Sm()+3));}
void  Channel_N_D4Buff 	(void)	{Send_Char_NLine2Socket4Sm(Read_Wished_Power(Dec_Bcd2Char(Actual_Rx_Buff4Sm())));}
void  Channel_N_F4Buff 	(void)	{Inc_Wished_Power_AllxX(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}
void  Channel_N_G4Buff 	(void)	{Dec_Wished_Power_AllxX(Dec_Bcd2Char(Actual_Rx_Buff4Sm()));}

void  Channel_N_A	(void)	{Config2Save(6,Channel_N_A4Buff);}
void  Channel_N_B	(void)	{Config2Save(6,Channel_N_B4Buff);}
void  Channel_N_C	(void)	{Config2Save(6,Channel_N_C4Buff);}
void  Channel_N_D	(void)	{Config2Save(3,Channel_N_D4Buff);}
void  Channel_N_E	(void)	{
	unsigned char i,Buf[9*8];
	for(i=0;i<8;i++) {
		Char2Bcd(Buf+0+i*9,i);
		Buf         [3+i*9]=' ';
		Char2Bcd(Buf+4+i*9,Read_Wished_Power(i));
		Buf         [7+i*9]='\r';
		Buf         [8+i*9]='\n';
	}
	Send_Data2Socket(Buf,9*8);
}
void  Channel_N_F	(void)	{Config2Save(3,Channel_N_F4Buff);}
void  Channel_N_G	(void)	{Config2Save(3,Channel_N_G4Buff);}
//------------ CONTROL----------------------------------------------------------------------------------------
void  Control_A		(void)	{Turn_On_Spi_Phisical();}
void  Control_B		(void)	{Turn_Off_Spi_Phisical();}
void  Control_C		(void)	{}
void  Control_D		(void)	{}
void  Control_E		(void)	{}
void  Control_F		(void)	{Atomic_Send_Event(Button3_Holded_Released_Event,Spi_Session());}
void  Control_G		(void)	{Send_Char_NLine2Socket(Is_Running());}
void  Control_H		(void)	{Begin_Power_Secuence();}
void  Control_I		(void)	{End_Power_Secuence();}
//------------ SYNC----------------------------------------------------------------------------------------
void  Sync_A		(void)	{Add_Event_Listener(Spi_Phisical_State_D_Event,Actual_Sm());}
void  Sync_B		(void)	{Send_Char_NLine2Socket(Read_D());}
void  Sync_Back		(void)	{Free_Event_Listener(Spi_Phisical_State_D_Event,Actual_Sm());}
//------------ INFO----------------------------------------------------------------------------------------
void  Info_A		(void)	{Send_Data2Socket(SERIAL_ID,SERIAL_ID_LENGTH);}
//------------ NETWORK----------------------------------------------------------------------------------------
void  Network_A		(void)	{Send_Ip2Socket(Read_Actual_Ip());}
void  Network_B		(void)	{}
void  Network_C		(void)	{Send_Int_NLine2Socket(Actual_Port4App());}
void  Network_D		(void)	{}
void  Network_E		(void)	{Send_Char_NLine2Socket(Number_Of_Socket_Opened());}
//------------ Bat----------------------------------------------------------------------------------------
void  Bat_A		(void)	{Send_Char_NLine2Socket(Read_Bat_Level());}
void  Bat_B		(void)	{Send_Char_NLine2Socket(Is_Charger_Connected());}
void  Bat_C		(void)	{Send_Char_NLine2Socket(Is_Bat_Charging());}
void  Bat_D		(void)	{Do_Care_Charger();}
void  Bat_E		(void)	{Dont_Care_Charger();}
void  Bat_F		(void)	{Send_Int_NLine2Socket(Read_Adc(ADC_CH_1));}
void  Bat_G		(void)	{Send_Int_NLine2Socket(Read_Adc(ADC_CH_2));}
void  Bat_H		(void)	{Send_Int_NLine2Socket(Read_Adc(ADC_CH_3));}
//------------ Virtual Buttons----------------------------------------------------------------------------------------
void  Virtual_Buttons_A		(void)	{Atomic_Send_Event(Button1_Pressed_Event,		Spi_Session());}
void  Virtual_Buttons_B		(void)	{Atomic_Send_Event(Button1_Holded_Event,		Spi_Session());}
void  Virtual_Buttons_C		(void)	{Atomic_Send_Event(Button1_Holding_Event,		Spi_Session());}
void  Virtual_Buttons_D		(void)	{Atomic_Send_Event(Button1_Released_Event,		Spi_Session());}
void  Virtual_Buttons_E		(void)	{Atomic_Send_Event(Button2_Pressed_Event,		Spi_Session());}
void  Virtual_Buttons_F		(void)	{Atomic_Send_Event(Button2_Holded_Event,		Spi_Session());}
void  Virtual_Buttons_G		(void)	{Atomic_Send_Event(Button2_Holding_Event,		Spi_Session());}
void  Virtual_Buttons_H		(void)	{Atomic_Send_Event(Button2_Released_Event,		Spi_Session());}
void  Virtual_Buttons_I		(void)	{Atomic_Send_Event(Button3_Pressed_Event,		Spi_Session());}
void  Virtual_Buttons_J		(void)	{Atomic_Send_Event(Button3_Holded_Event,		Spi_Session());}
void  Virtual_Buttons_K		(void)	{Atomic_Send_Event(Button3_Holding_Event,		Spi_Session());}
void  Virtual_Buttons_L		(void)	{Atomic_Send_Event(Button3_Released_Event,		Spi_Session());}
void  Virtual_Buttons_M		(void)	{Atomic_Send_Event(Button3_Holded_Released_Event,	Spi_Session());}
void  Virtual_Buttons_N		(void)	{Send_Char_NLine2Socket(Read_Clip_State());}
//------------ Virtual Buttons----------------------------------------------------------------------------------------
void  Clip_A			(void)	{Send_Char_NLine2Socket(Read_Clip_State());}
void  Clip_B			(void)	{Do_Care_Clip();}
void  Clip_C			(void)	{Dont_Care_Clip();}
//----------------------------------------------------------------------------------------------------
void  Welcome_Help		(void)	{Send_Data2Socket(Welcome_Help_Data,		sizeof(Welcome_Help_Data)-1);}
void  Time1_Help		(void)	{Send_Data2Socket(Time1_Help_Data,		sizeof(Time1_Help_Data)-1);}
void  Time2_Help		(void)	{Send_Data2Socket(Time1_Help_Data,		sizeof(Time1_Help_Data)-1);}
void  Channel_N_Help		(void)	{Send_Data2Socket(Channel_N_Help_Data,		sizeof(Channel_N_Help_Data)-1);}
void  Control_Help		(void)	{Send_Data2Socket(Control_Help_Data,		sizeof(Control_Help_Data)-1);}
void  Sync_Help			(void)	{Send_Data2Socket(Sync_Help_Data,		sizeof(Sync_Help_Data)-1);}
void  Info_Help			(void)	{Send_Data2Socket(Info_Help_Data,		sizeof(Info_Help_Data)-1);}
void  Network_Help		(void)	{Send_Data2Socket(Network_Help_Data,		sizeof(Network_Help_Data)-1);}
void  Bat_Help			(void)	{Send_Data2Socket(Bat_Help_Data,		sizeof(Bat_Help_Data)-1);}
void  Virtual_Buttons_Help	(void)	{Send_Data2Socket(Virtual_Buttons_Help_Data,	sizeof(Virtual_Buttons_Help_Data)-1);}
void  Clip_Help			(void)	{Send_Data2Socket(Clip_Help_Data,		sizeof(Clip_Help_Data)-1);}
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
 'A'				,Welcome_A					,Time1,
 'B'				,Welcome_B					,Time2,
 'C'				,Welcome_C					,Channel_N,
 'D'				,Welcome_D					,Control,
 'E'				,Welcome_E					,Info,
 'F'				,Welcome_F					,Network,
 'G'				,Welcome_G					,Bat,
 'H'				,Welcome_H					,Virtual_Buttons,
 'I'				,Welcome_I					,Clip,
 'J'				,Welcome_J					,Sync,
 '?'				,Welcome_Help					,Welcome,
 ANY_Event			,Rien						,Welcome,
};
static const State Time1[] =
{
 'A'				,Time1_Set_A					,Time1,	//Set T1A
 'B'				,Time1_Set_B					,Time1, //Set T1B
 'C'				,Time1_Set_C					,Time1, //Set T1C
 'D'				,Time1_Set_D					,Time1, //Set T1D
 'E'				,Time1_Set_E					,Time1, //Set T1E
 'F'				,Time1_Set_F					,Time1, //Set T1F
 'G'				,Time1_Set_G					,Time1, //Set T1G
 'H'				,Time1_Read_A					,Time1,	//Read T1A
 'I'				,Time1_Read_B					,Time1, //Read T1B
 'J'				,Time1_Read_C					,Time1, //Read T1C
 'K'				,Time1_Read_D					,Time1, //Read T1D
 'L'				,Time1_Read_E					,Time1, //Read T1E
 'M'				,Time1_Read_F					,Time1, //Read T1F
 'N'				,Time1_Read_G					,Time1, //Read T1G
 'O'				,Set_Bank1					,Time1, //Fuerza a usar el banko 1
 '<'				,Rien 						,Welcome,
 '?'				,Time1_Help					,Time1,
 ANY_Event			,Rien						,Time1,
};
static const State Time2[] =
{
 'A'				,Time2_Set_A					,Time2,	//Set  T2A
 'B'				,Time2_Set_B					,Time2, //Set  T2B
 'C'				,Time2_Set_C					,Time2, //Set  T2C
 'D'				,Time2_Set_D					,Time2, //Set  T2D
 'E'				,Time2_Set_E					,Time2, //Set  T2E
 'F'				,Time2_Set_F					,Time2, //Set  T2F
 'G'				,Time2_Set_G					,Time2, //Set  T2G
 'H'				,Time2_Read_A					,Time2,	//Read T2A
 'I'				,Time2_Read_B					,Time2, //Read T2B
 'J'				,Time2_Read_C					,Time2, //Read T2C
 'K'				,Time2_Read_D					,Time2, //Read T2D
 'L'				,Time2_Read_E					,Time2, //Read T2E
 'M'				,Time2_Read_F					,Time2, //Read T2F
 'N'				,Time2_Read_G					,Time2, //Read T2G
 'O'				,Set_Bank2					,Time2, //Fuerza a usar el banko 2
 '<'				,Rien 						,Welcome,
 '?'				,Time2_Help					,Time2,
 ANY_Event			,Rien						,Time2,
};
static const State Channel_N[] =
{
 'A'				,Channel_N_A					,Channel_N, 	//Inc power
 'B'				,Channel_N_B					,Channel_N, 	//Dec power	
 'C'				,Channel_N_C					,Channel_N, 	//Set Power
 'D'				,Channel_N_D					,Channel_N, 	//Read Power
 'E'				,Channel_N_E					,Channel_N, 	//Read Power All
 'F'				,Channel_N_F					,Channel_N, 	//Inc_Ch_AllxX
 'G'				,Channel_N_G					,Channel_N, 	//Inc_Ch_AllxX
 '<'				,Rien 						,Welcome,
 '?'				,Channel_N_Help					,Channel_N,
 ANY_Event			,Rien						,Channel_N,
};
static const State Control[] =
{
 'A'				,Control_A					,Control, 	//Start
 'B'				,Control_B					,Control, 	//Stop
 'C'				,Control_C					,Control, 	//Pause
 'D'				,Control_D					,Control, 	//Set Time
 'E'				,Control_E					,Control, 	//Read Time
 'F'				,Control_F					,Control, 	//Power Off
 'G'				,Control_G					,Control, 	//Is running?
 'H'				,Control_H					,Control, 	//Power Seecuence
 'I'				,Control_I					,Control, 	//Power Seecuence
 '<'				,Rien 						,Welcome,
 '?'				,Control_Help					,Control,
 ANY_Event			,Rien						,Control,
};
static const State Sync[] =
{
 'A'				,Sync_A						,Sync,
 'B'				,Sync_B						,Sync, 
 '<'				,Sync_Back 					,Welcome,
 '?'				,Sync_Help					,Sync,
 ANY_Event			,Rien						,Sync,
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
static const State Bat[] =
{
 'A'				,Bat_A						,Bat,
 'B'				,Bat_B						,Bat,
 'C'				,Bat_C						,Bat,
 'D'				,Bat_D						,Bat,
 'E'				,Bat_E						,Bat,
 'F'				,Bat_F						,Bat,
 'G'				,Bat_G						,Bat,
 'H'				,Bat_H						,Bat,
 '<'				,Rien 						,Welcome,
 '?'				,Bat_Help					,Bat,
 ANY_Event			,Rien						,Bat,
};
static const State Virtual_Buttons[] =
{
 'A'				,Virtual_Buttons_A					,Virtual_Buttons,
 'B'				,Virtual_Buttons_B					,Virtual_Buttons,
 'C'				,Virtual_Buttons_C					,Virtual_Buttons,
 'D'				,Virtual_Buttons_D					,Virtual_Buttons,
 'E'				,Virtual_Buttons_E					,Virtual_Buttons,
 'F'				,Virtual_Buttons_F					,Virtual_Buttons,
 'G'				,Virtual_Buttons_G					,Virtual_Buttons,
 'H'				,Virtual_Buttons_H					,Virtual_Buttons,
 'I'				,Virtual_Buttons_I					,Virtual_Buttons,
 'J'				,Virtual_Buttons_J					,Virtual_Buttons,
 'K'				,Virtual_Buttons_K					,Virtual_Buttons,
 'L'				,Virtual_Buttons_L					,Virtual_Buttons,
 'M'				,Virtual_Buttons_M					,Virtual_Buttons,
 'N'				,Virtual_Buttons_N					,Virtual_Buttons,
 '<'				,Rien 							,Welcome,
 '?'				,Virtual_Buttons_Help					,Virtual_Buttons,
 ANY_Event			,Rien							,Virtual_Buttons,
};
static const State Clip[] =
{
 'A'				,Clip_A					,Clip,
 'B'				,Clip_B					,Clip,
 'C'				,Clip_C					,Clip,
 '<'				,Rien 					,Welcome,
 '?'				,Clip_Help				,Clip,
 ANY_Event			,Rien					,Clip,
};
//----------------------------------------------------------------------------------------------------

