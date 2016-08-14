#include "hw_types.h"/*{{{*/
#include "hw_memmap.h"
#include "spi_phisical.h"
#include "spi_session.h"
#include "../driverlib/spi.h"
#include "hw_ints.h"
#include "hw_gpio.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "gpio_if.h"
#include "events.h"
#include "state_machine.h"
#include "buttons.h"
#include "common.h"
#include "schedule.h"
#include "wifi_phisical.h"
#include "leds_session.h"
#include "hibernate.h"
#include "parameters.h"
#include "bat.h"
#include "clip.h"
#include "debug.h"
#include "uart_if.h"/*}}}*/
//------------------------------------------------------------------
static const State   
        Wifi_Selection[],
        Wifi_Control[],
        Manual[],
        Charging[],
        Off[];

const State* Spi_Session_Sm;
//----------------------------------------------------------------------------------------------------
struct Struct_Params Params= {/*{{{*/
	25,	//A1
	200,	//B1
	0,	//C1	en cero va for ever...
	1,	//D1
	1,	//E1
	1,	//F1
	0,	//G1
	20,	//A2	//dado que C es cero y G1 tambien.. todo el resto no sirve para nada...
	500,	//B2
	5,	//C2
	2,	//D2
	5,	//E2
	5,	//F2
	2,	//G2
	0,	//Bank
};/*}}}*/
unsigned char Wished_Power[8]= {0,0,0,0,0,0,0,0};
unsigned char Matrix_Code[8*9*2]=/*{{{*/
{
//             4          3           2          1            8          7           6          5   
//  	    NEG2       NEG1      DUMY2       DUMY1         POS3       POS2     POS1/PWR    NEG 3
//	  11111100   22221111                        	  00000000   11111100   22221111   00000000  
//	  54321098   32109876                        	  76543210   54321098   32109876   76543210 
	0b01000000,0b00000000,0b00000000,0b00000000, 	0b00000001,0b00000000,0b00000000,0b00000000,   //SW0+
	0b10000000,0b00000000,0b00000000,0b00000000, 	0b00000010,0b00000000,0b00000000,0b00000000,   //SW0-

	0b00010000,0b00000000,0b00000000,0b00000000, 	0b00000100,0b00000000,0b00000000,0b00000000,   //SW1+
	0b00100000,0b00000000,0b00000000,0b00000000, 	0b00001000,0b00000000,0b00000000,0b00000000,   //SW1-

	0b00000100,0b00000000,0b00000000,0b00000000, 	0b00010000,0b00000000,0b00000000,0b00000000,   //SW2+
	0b00001000,0b00000000,0b00000000,0b00000000, 	0b00100000,0b00000000,0b00000000,0b00000000,   //SW2-

	0b00000001,0b00000000,0b00000000,0b00000000, 	0b01000000,0b00000000,0b00000000,0b00000000,   //SW3+
	0b00000010,0b00000000,0b00000000,0b00000000, 	0b10000000,0b00000000,0b00000000,0b00000000,   //SW3-

	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00000001,0b00000000,0b01000000,   //SW4+
	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00000010,0b00000000,0b10000000,   //SW4-

	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00000100,0b00000000,0b00010000,   //SW5+
	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00001000,0b00000000,0b00100000,   //SW5-

	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00010000,0b00000000,0b00000100,   //SW6+
	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00100000,0b00000000,0b00001000,   //SW6-

	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b01000000,0b00000000,0b00000001,   //SW7+
	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b10000000,0b00000000,0b00000010,   //SW7-

	0b00000000,0b00000000,0b00000000,0b00000000, 	0b00000000,0b00000000,0b00000000,0b00000000,  //Clear all
};/*}}}*/
//----------------------------------------------------------------------------------------------------
void 		Init_Spi_Session	(void) 			
{ 
	Spi_Session_Sm=Wifi_Selection;
	Init_100v();
	Init_Spi_Phisical();
	Wifi_Selection_Effect();
	New_None_Periodic_Schedule(10,Button3_Released_Event,Spi_Session()); 		//debug para que me conecte a dci y poder debuguear
}	
const State** 	Spi_Session		(void) {return &Spi_Session_Sm;}
void 		Spi_Session_Rti		(void) {Atomic_Send_Event(ANY_Event,Spi_Session());}
//----------------------------------------------------------------------------------------------------
unsigned int* Read_Matrix_Code(void) {return (unsigned int*)Matrix_Code;}
//----------------------------------------------------------------------------------------------------
void Init_100v(void)
{
	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA3, 	PRCM_RUN_MODE_CLK);
	MAP_PinTypeGPIO			(PIN_53,	PIN_MODE_0, 	false);
	MAP_GPIODirModeSet		(GPIOA3_BASE, 	0x40, 		GPIO_DIR_MODE_OUT);
}
void Enable_100v 	(void)	{MAP_GPIOPinWrite(GPIOA3_BASE,0x40,0x40);}
void Disable_100v 	(void)	{MAP_GPIOPinWrite(GPIOA3_BASE,0x40,0x00);}
//----------POWER---------------------
unsigned char 	Read_Wished_Power	(unsigned char Channel)				{return Wished_Power[Channel];}
unsigned char 	Read_Actual_Power	(unsigned char Channel)				{return Matrix_Code[Channel*16+6];}
void 		Set_Wished_Power	(unsigned char Channel,unsigned char Power)	
{
 	Wished_Power[Channel]=Power;
	if(Is_All_Wished_Power_Zero()) Atomic_Send_Event(Power_Disable_Event,Spi_Phisical());
 	DBG_SPI_SESSION_PRINT("Wished Power= %d\r\n",Power);
}
void 		Set_Actual_Power	(unsigned char Channel,unsigned char Power)	
{
 	Matrix_Code[Channel*16+6]=Matrix_Code[Channel*16+6+8]=Power;
 	DBG_SPI_SESSION_PRINT("Actual Power= %d Channel=%d\r\n",Power,Channel);
}
void 		Inc_Wished_Power 	(unsigned char Channel,unsigned char Increment)	
{
	unsigned int Wished_Power=Read_Wished_Power(Channel);
	if((Wished_Power+Increment)<=255) 	Set_Wished_Power(Channel,Wished_Power+Increment);
	else 					Set_Wished_Power(Channel,255);
	
}
void 		Dec_Wished_Power 	(unsigned char Channel,unsigned char Decrement)	
{
	unsigned char Wished_Power=Read_Wished_Power(Channel);
	if(Wished_Power>=Decrement)      Set_Wished_Power(Channel,Wished_Power-Decrement);
	else  				 Set_Wished_Power(Channel,0);
}
void 		Inc_Actual_Power 	(unsigned char Channel,unsigned char Increment)	
{
	unsigned int Actual_Power=Read_Actual_Power(Channel); 		//aca se puede guardar directamente la suma para no sumar 2 veces...aca y en el paso de parametros...
	if((Actual_Power+Increment)<=255) 	Set_Actual_Power(Channel,Actual_Power+Increment);
	else				 	Set_Actual_Power(Channel,255); 
}
void 		Inc_Actual_Power_Until_Wished 	(unsigned char Channel,unsigned char Increment,unsigned char Wished_Power)	
{
	unsigned int 	Actual_Power	=Read_Actual_Power(Channel);
	if((Actual_Power+Increment)<=Wished_Power) 	Set_Actual_Power(Channel,Actual_Power+Increment);
	else				 		Set_Actual_Power(Channel,Wished_Power); 
}
void 		Dec_Actual_Power 	(unsigned char Channel,unsigned char Decrement)	
{
	unsigned char Actual_Power=Read_Actual_Power(Channel);
	if(Actual_Power>=Decrement)      Set_Actual_Power(Channel,Actual_Power-Decrement);
	else				 Set_Actual_Power(Channel,0); 
}
void 		Ramp_Up_Power_All	(void)
{
	unsigned char i,Ramp=Read_E();
	DBG_SPI_SESSION_PRINT("Ramp up all\r\n");
	for(i=0;i<8;i++) {
		unsigned char Wished=Read_Wished_Power(i);
		if(Wished>0) 	Inc_Actual_Power_Until_Wished(i,Wished<Ramp?1:(Wished/Ramp),Wished);
	}
}
void 		Ramp_Down_Power_All 	(void)
{
	unsigned char i,Ramp=Read_F();
	DBG_SPI_SESSION_PRINT("Ramp Down all\r\n");
	for(i=0;i<8;i++) {
		unsigned char Wished=Read_Wished_Power(i);
		Dec_Actual_Power(i,Wished<Ramp?1:Wished/Ramp);
	}
}
unsigned char Is_All_Wished_Power_Zero	(void)		//devuelve 1 si estan todas las potencias en cero
{
	unsigned char i; 
	for (i=0;i<8 && Wished_Power[i]==0;i++) 
		;
	return i==8;
}
void Inc_Wished_Power_All	(void)			
{
	unsigned char i; 
	for (i= 0;i<8;i++) Inc_Wished_Power(i,1);
	Set_Toogle_Led_Effect(Led1);
}
void Inc_Wished_Power_AllxX	(unsigned char X)			
{
	unsigned char i; 
	for (i=0;i<8;i++) Inc_Wished_Power(i,X);
}
void Inc_Wished_Power_Allx5	(void)			
{
	Inc_Wished_Power_AllxX(5);
	Set_Toogle_Led_Effect(Led1);
}
void Dec_Wished_Power_AllxX	(unsigned char X)			
{
	unsigned char i; 
	for (i=0;i<8;i++) Dec_Wished_Power(i,X);
}
void Dec_Wished_Power_Allx5	(void)			
{
	Dec_Wished_Power_AllxX(5);
	Set_Toogle_Led_Effect(Led2);
}
void Set_Wished_Power_All	(unsigned char Power)	
{
	unsigned char i;
	DBG_SPI_SESSION_PRINT("Set Wished Power all= %d\r\n",Power);
	for (i=0;i<8;i++) Set_Wished_Power(i,Power);
}
void Set_Actual_Power_All	(unsigned char Power)	
{
	unsigned char i;
	DBG_SPI_SESSION_PRINT("Set Actual Power all= %d\r\n",Power);
	for (i=0;i<8;i++) Set_Actual_Power(i,Power);
}
void Set_Wished2Actual_Power_All	(void)	
{
	unsigned char i;
	DBG_SPI_SESSION_PRINT("Set Wished->Actual Power all\r\n");
	for (i=0;i<8;i++) Set_Actual_Power(i,Read_Wished_Power(i));
}

void Secuence_Wished_Power	(void)		//...0-1-2-3-4-5-6-7-all-0-1...	
{
	static unsigned char i; 
		if(i==0) {
			unsigned char j;
			Set_Wished_Power(0,100);
			for(j=1;j<8;j++) Set_Wished_Power(j,0);
			i++;
		}
	else	if(i>0 && i<8) { 				//si la static entro con basura.. seguramente caiga por aca la primera entrada...
			Set_Wished_Power(i,100);		//primero pongo el siguiente canal en potencia mayor a cero y LUEGO apago el anterior... sino en el momento que quedan todos los canales sin power se reinicia el ciclo de operacion de spi_phisical
			Set_Wished_Power(i-1,0);
			i++;
		}
	else 	if(i>=8) {
			Set_Wished_Power_All(100);
			i=0;
		}
}
void Begin_Power_Secuence(void)
{
	Dont_Care_Clip();
	Dont_Care_Charger();
	Turn_On_Spi_Phisical();
	DBG_SPI_SESSION_PRINT("Begin power secuence\r\n"); 
	Free_Func_Schedule(Secuence_Wished_Power);
	New_Periodic_Func_Schedule(30,Secuence_Wished_Power);
	Set_Led_Effect(Led1,0xA000);
}
void End_Power_Secuence(void)
{
	DBG_SPI_SESSION_PRINT("End power secuence\r\n"); 
	Free_Func_Schedule(Secuence_Wished_Power);
	Set_Fixed_Led_Effect(Led1,0x0000);
}
//----------Lectura de parametros---------------------
unsigned char 	Read_A		(void)	{return Params.Bank?Params.A2:Params.A1;}
unsigned short	Read_B		(void)	{return Params.Bank?Params.B2:Params.B1;}
unsigned short 	Read_C		(void)	{return Params.Bank?Params.C2:Params.C1;}
unsigned char 	Read_D		(void)	{return Params.Bank?Params.D2:Params.D1;}
unsigned char 	Read_E		(void)	{return Params.Bank?Params.E2:Params.E1;}
unsigned char 	Read_F		(void)	{return Params.Bank?Params.F2:Params.F1;}
unsigned char 	Read_G		(void)	{return Params.Bank?Params.G2:Params.G1;}
unsigned char 	Read_A1 	(void)	{return Params.A1;}
unsigned short	Read_B1 	(void)	{return Params.B1;}
unsigned short 	Read_C1 	(void)	{return Params.C1;}
unsigned char 	Read_D1 	(void)	{return Params.D1;}
unsigned char 	Read_E1 	(void)	{return Params.E1;}
unsigned char 	Read_F1 	(void)	{return Params.F1;}
unsigned char 	Read_G1 	(void)	{return Params.G1;}
unsigned char 	Read_A2 	(void)	{return Params.A2;}
unsigned short	Read_B2 	(void)	{return Params.B2;}
unsigned short 	Read_C2 	(void)	{return Params.C2;}
unsigned char 	Read_D2 	(void)	{return Params.D2;}
unsigned char 	Read_E2 	(void)	{return Params.E2;}
unsigned char 	Read_F2 	(void)	{return Params.F2;}
unsigned char 	Read_G2 	(void)	{return Params.G2;}
void 		Toogle_Bank	(void)	{Params.Bank^=0x01;}
void 		Set_Bank1	(void)	{Params.Bank=0x00;}
void 		Set_Bank2	(void)	{Params.Bank=0x01;}
void  		Set_A1		(unsigned char  A)	{Params.A1=A;}
void  		Set_B1		(unsigned short B)	{Params.B1=B;}
void  		Set_C1		(unsigned short C)	{Params.C1=C;}
void  		Set_D1		(unsigned char  D)	{Params.D1=D;}
void  		Set_E1		(unsigned char  E)	{Params.E1=E;}
void  		Set_F1		(unsigned char  F)	{Params.F1=F;}
void  		Set_G1		(unsigned char  G)	{Params.G1=G;}
void  		Set_A2		(unsigned char  A)	{Params.A2=A;}
void  		Set_B2		(unsigned short B)	{Params.B2=B;}
void  		Set_C2		(unsigned short C)	{Params.C2=C;}
void  		Set_D2		(unsigned char  D)	{Params.D2=D;}
void  		Set_E2		(unsigned char  E)	{Params.E2=E;}
void  		Set_F2		(unsigned char  F)	{Params.F2=F;}
void  		Set_G2		(unsigned char  G)	{Params.G2=G;}
//----------EFFECTS---------------------
void Effect_Ready2Hibernate(void)
{
	 Turn_Off_Spi_Phisical	(); 		//levanta un flag para que apague la secuencia spi en orden
	 DBG_SPI_SESSION_PRINT	("Ready2Hibernate \r\n"); 
	 Set_Led_Effect		(Led1,0x8888);
	 Set_Led_Effect		(Led2,0x8888);
	 Set_Led_Effect		(Led3,0x8888);
}
void Wifi_Selection_Effect(void)
{
	 DBG_SPI_SESSION_PRINT	("Wifi selection\r\n"); 
	 Set_Led_Effect		(Led1,0xFFFF);
	 Set_Led_Effect		(Led2,0xFFFF);
	 Set_Led_Effect		(Led3,0xFFFF);
}

void Wifi_Control1_Effect(void)
{
	 Set_Ssid_Name		(SERIAL_ID,SERIAL_ID_LENGTH);
	 Set_Security_Key	(SECURITY_KEY,SECURITY_KEY_LENGTH);
	 DBG_SPI_SESSION_PRINT	("Wifi_Control1\r\n"); 
	 Set_Led_Effect		(Led1,0x0000);
	 Set_Led_Effect		(Led2,0x0000);
	 New_None_Periodic_Func_Schedule(20,Wifi_Control1_Delayed_Led_Effect);
	 Turn_On_Wifi		(); 
}
void Wifi_Control1_Delayed_Led_Effect(void)
{
	 Set_Temp_Led_Effect 	(Led2,0x0001);
}

void Wifi_Control2_Effect(void)
{
	 Set_Ssid_Name		("justbody",8);
	 Set_Security_Key	(SECURITY_KEY,SECURITY_KEY_LENGTH);
	 DBG_SPI_SESSION_PRINT	("Wifi_Control2\r\n"); 
	 Set_Led_Effect		(Led1,0x0000);
	 Set_Led_Effect		(Led2,0x0000);
	 New_None_Periodic_Func_Schedule(20,Wifi_Control2_Delayed_Led_Effect);
	 Turn_On_Wifi		(); 
}
void Wifi_Control2_Delayed_Led_Effect(void)
{
	 Set_Temp_Led_Effect 	(Led2,0x0005);
}

void Wifi_Control_Dci_Effect(void)
{
	 Set_Ssid_Name		("DCI",3);
	 Set_Security_Key	("DisenioconingeniO",17);
	 DBG_SPI_SESSION_PRINT	("Wifi_Control_Dci\r\n"); 
	 Set_Led_Effect		(Led1,0x0000);
	 Set_Led_Effect		(Led2,0x0000);
	 New_None_Periodic_Func_Schedule(20,Wifi_Dci_Delayed_Led_Effect);
	 Turn_On_Wifi		(); 
}
void Wifi_Dci_Delayed_Led_Effect(void)
{
	 Set_Temp_Led_Effect 	(Led2,0x0015);
}

void Charger_Connected(void)
{
	Atomic_Send_Event	(Charger_Connected_Event,	Spi_Phisical());
}
static void Charger_Disconnected(void)
{
 	Atomic_Send_Event	(Charger_Disconnected_Event,	Spi_Phisical());
}
void Cancell_Schedule_Auto_Wifi	(void)	
{
	Free_Schedule		(Button3_Released_Event,Spi_Session());
}
//----------------------------------------------------------------------------------------------------
void Wifi_Control_Dci_Effect_And_Begin_Power_Secuence 	(void) {Wifi_Control_Dci_Effect();	Begin_Power_Secuence();}		
void Wifi_Control2_Effect_And_Begin_Power_Secuence 	(void) {Wifi_Control2_Effect();		Begin_Power_Secuence();}	
//----------------------------------------------------------------------------------------------------
static const State Wifi_Selection[] =
{
 Button1_Released_Event		,Wifi_Control2_Effect_And_Begin_Power_Secuence		,Manual,
 Button2_Released_Event		,Wifi_Control1_Effect					,Wifi_Control,
 Button3_Released_Event		,Wifi_Control2_Effect					,Wifi_Control,
 Button2_Trick_Event		,Wifi_Control_Dci_Effect_And_Begin_Power_Secuence	,Wifi_Control,
 Button3_Holded_Event		,Effect_Ready2Hibernate					,Wifi_Selection,
 Button3_Holded_Released_Event 	,Hibernate_Now 						,Off,

 Button1_Pressed_Event 		,Cancell_Schedule_Auto_Wifi				,Wifi_Selection,
 Button2_Pressed_Event 		,Cancell_Schedule_Auto_Wifi				,Wifi_Selection,
 Button3_Pressed_Event 		,Cancell_Schedule_Auto_Wifi				,Wifi_Selection,

 ANY_Event                      ,Rien							,Wifi_Selection,
};
static const State Wifi_Control[] =
{
 Button1_Holded_Event		,Inc_Wished_Power_Allx5  				,Wifi_Control,	// si tomo el pressse me disparan solos los botones x interferencia!!
 Button2_Holded_Event		,Dec_Wished_Power_Allx5  				,Wifi_Control,
 Button1_Holding_Event		,Inc_Wished_Power_Allx5  				,Wifi_Control,	
 Button2_Holding_Event		,Dec_Wished_Power_Allx5  				,Wifi_Control,
 
 Button3_Holded_Event		,Effect_Ready2Hibernate					,Wifi_Control,
 Button3_Holded_Released_Event	,Hibernate_Now 		 				,Off,
 Charger_Connected_Event	,Rien 							,Charging,
 ANY_Event			,Rien							,Wifi_Control,
};
//----------
static const State Manual[] = 
{
 Button1_Holded_Event		,Inc_Wished_Power_Allx5  				,Manual,	// si tomo el pressse me disparan solos los botones x interferencia!!
 Button2_Holded_Event		,Dec_Wished_Power_Allx5  				,Manual,
 Button1_Holding_Event		,Inc_Wished_Power_Allx5  				,Manual,	
 Button2_Holding_Event		,Dec_Wished_Power_Allx5  				,Manual,

 Button3_Holded_Event		,Effect_Ready2Hibernate					,Manual,
 Button3_Holded_Released_Event	,Hibernate_Now						,Off,
 Charger_Connected_Event	,Rien 							,Charging,
 ANY_Event			,Rien							,Manual,
};
//----------
static const State Charging[] =
{
 Charger_Connected_Event	,Charger_Connected 					,Charging,
 Charger_Disconnected_Event	,Charger_Disconnected					,Wifi_Control,
 Button3_Holded_Event		,Effect_Ready2Hibernate					,Charging,
 Button3_Holded_Released_Event 	,Hibernate_Now 						,Off,
 ANY_Event                      ,Rien			 				,Charging,
};
static const State Off[] =
{
 ANY_Event                      ,Rien			 				,Off,
};
//-------------------------------------------------------------
