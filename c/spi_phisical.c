#include "hw_types.h"/*{{{*/
#include "hw_memmap.h"
#include "leds_session.h"
#include "spi_phisical.h"
#include "spi_session.h"
#include "wifi_socket.h"
#include "spi.h"
#include "hw_ints.h"
#include "hw_gpio.h"
#include "hw_mcspi.h"
#include "hw_timer.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "gpio_if.h"
#include "timer_if.h"
#include "timer.h"
#include "common.h"
#include "events.h"
#include "uart_if.h"
#include "bat.h"
#include "debug.h"
#include "udma.h"/*}}}*/
//-------------------------------------------------------------
static const State   
		Charging[],
		Clip_Off[],
		Off[],
		Testing_Power[],
		E1[], 
		C1[], 
		F1[], 
		D1[];
//-------------------------------------------------------------
unsigned int*  Matrix_Codes;
unsigned char  Actual_Channel,Turn_Off_Request,Running;
unsigned short Fire_Counter;
unsigned short Secuence_Counter;
const State*   Spi_Phisical_Sm;
//-------------------------------------------------------------
void 	Init_Spi_Phisical	(void)	/*{{{*/
{
 	Spi_Phisical_Sm=Off;
	Turn_Off_Request=0;
	Running=0;
	MAP_PRCMPeripheralClkEnable(PRCM_GSPI,PRCM_RUN_MODE_CLK);
	MAP_PinTypeSPI(PIN_05, PIN_MODE_7);							//CLK
	MAP_PinTypeSPI(PIN_06, PIN_MODE_7);							//MISO
	MAP_PinTypeSPI(PIN_07, PIN_MODE_7);							//MOSI

	MAP_PRCMPeripheralReset(PRCM_GSPI);
	MAP_SPIReset(GSPI_BASE);
	MAP_SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,(SPI_3PIN_MODE|SPI_TURBO_ON|SPI_WL_32));
	MAP_SPIFIFOEnable(GSPI_BASE, SPI_RX_FIFO);
	MAP_SPIFIFOEnable(GSPI_BASE, SPI_TX_FIFO); 
	MAP_SPIWordCountSet(GSPI_BASE, 2);
	MAP_SPIEnable(GSPI_BASE);

	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA2, 	PRCM_RUN_MODE_CLK);
    	MAP_PinTypeGPIO	(PIN_08,	PIN_MODE_0, 	false);					//CLR
	MAP_GPIODirModeSet		(GPIOA2_BASE, 	0x02, 		GPIO_DIR_MODE_OUT);

	MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA1, 	PRCM_RUN_MODE_CLK);
    	MAP_PinTypeGPIO	(PIN_03,	PIN_MODE_0, 	false);					//LEB
	MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x10, 		GPIO_DIR_MODE_OUT);
    	MAP_PinTypeGPIO	(PIN_04,	PIN_MODE_0, 	false);					//LEA
	MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x20, 		GPIO_DIR_MODE_OUT);

	Spi_Le_A_Off	();
	Spi_Le_B_Off	();
	Spi_Clr_Off	();
	Init_Timers();
}/*}}}*/
//-------------------------------------------------------------
void Spi_Le_A_On	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x20,0x20);}
void Spi_Le_A_Off	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x20,0x00);}
void Spi_Le_B_On	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x10,0x10);}
void Spi_Le_B_Off	(void)	{MAP_GPIOPinWrite(GPIOA1_BASE,0x10,0x00);}
void Spi_Clr_On		(void)	{MAP_GPIOPinWrite(GPIOA2_BASE,0x02,0x02);}
void Spi_Clr_Off	(void)	{MAP_GPIOPinWrite(GPIOA2_BASE,0x02,0x00);}
//-------------------------------------------------------------
void Init_Timers(void)
{
	Actual_Channel=0;
	Matrix_Codes=Read_Matrix_Code();

	//timer A en usecs para los tiempos de pulso.
	Timer_IF_Init 		(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);
	Timer_IF_IntSetup 	(TIMERA0_BASE, TIMER_A, TimerA_Handler);
   	Timer_IF_Start_uS 	(TIMERA0_BASE, TIMER_A, 100);
	TIMERA0_A_INT_DISABLE();

	//timer B en msegs palo para setear los tiempos muertos.
	Timer_IF_Init 		(PRCM_TIMERA1, TIMERA1_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);
	Timer_IF_IntSetup 	(TIMERA1_BASE, TIMER_A, TimerB_Handler);
   	Timer_IF_Start 		(TIMERA1_BASE, TIMER_A, 10000);
	TIMERA1_A_INT_DISABLE();
}
void TimerA_Handler(void)
{
    	HWREG(TIMERA0_BASE + TIMER_O_ICR) = HWREG(TIMERA0_BASE + TIMER_O_MIS);	//limpia flag
	SPI_LE_A_OFF(); 
	SPI_LE_B_OFF();
	HWREG(GSPI_BASE + MCSPI_O_RX0);				//lee el bendito receptor que tiene lo que se lleno por la transaccion anterior.. como mando de a 2, leo 2 veces... sino leo, el tx se traba podes creer!... tengo que seguir investigando.. o hacer la lectura y tirarla por DMA>..
	HWREG(GSPI_BASE + MCSPI_O_RX0);
	SPI_LE_A_ON();
	SPI_LE_B_ON();
	if(++Actual_Channel>17) {
		TIMERA0_A_INT_DISABLE();
		DBG_SPI_PHISICAL_PRINT("\nFire=%d\r\n",Fire_Counter);					//si estoy en la entrada 17, liso... mision cumplida, quiere decir que mande los 8 canales de 2 entradas, mas la de limpiar todo... me voy
		return;	
	}			
	if( ((unsigned char*)Matrix_Codes)[6]!=0) {				//manda el dato SOLO si la potencia es mayor a cero, cosa de que si la pot es cero ni siquiera prenda el switch... para evitar el spureo de los resistores de bleed
		HWREG(GSPI_BASE+MCSPI_O_TX0) = Matrix_Codes[0]; 		//manda 2 int de 32, pero como tengo un buffer de 2 en el spi, escribe los 2 en la fifo y sale de la funcion.. no se queda esperando nada...y tampoco se pisan.. esa es la magia que buscaba
		HWREG(GSPI_BASE+MCSPI_O_TX0) = Matrix_Codes[1]; 
	}
	else {
		HWREG(GSPI_BASE+MCSPI_O_TX0) = 0;				//como la pot de este canal se solicito en cero, ni siquiera se prende el switch... se manda el dato solo para que al recibir se reciba algo y para apagar algun canal prendido ya que reoordad que la pot real se puede cambiar on real time....
		HWREG(GSPI_BASE+MCSPI_O_TX0) = 0; 
	}
	Matrix_Codes+=2;
}
void TimerB_Handler(void)
{
    	HWREG(TIMERA1_BASE + TIMER_O_ICR) = HWREG(TIMERA1_BASE + TIMER_O_MIS); //clear flag irq
	Send_Event(Turn_Off_Request?Off_Event:Time_B_Event,Spi_Phisical()); 	//si me piden apagar, aca lo chequeo contra una variable de estado, de maner que me aseguro de que en este momento que voy a apagar, la salida de todos los canales estan en cero porque justamente sino, aca empezaria a mnadar pulsos... y luego al prender, arranco siempore por rampa.. cosa de que por ejmplo si me descnoectan el clip. luego arranca por rampa...
}
//----------------------------------------------------------------------------------------------------
unsigned char Is_Running(void) 	
{
	return Running;
}
void Update_Effect_Charger(void)
{
	unsigned short int Level=Read_Bat_Level();
	DBG_SPI_PHISICAL_PRINT("Update Effect Charger With bat\r\n"); 
	if(Is_Bat_Charging()) {
		if(Level<80)	{
			Set_Fixed_Led_Effect(Led1,0x0000); 
			Set_Fixed_Led_Effect(Led2,0xF0F0); 
		}
		else {
			Set_Fixed_Led_Effect(Led1,0xF0F0); 
			Set_Fixed_Led_Effect(Led2,0xFFFF); 
		}
	}
	else {
		Set_Fixed_Led_Effect(Led1,0xFFFF); 
		Set_Fixed_Led_Effect(Led2,0xFFFF);
		Set_Fixed_Led_Effect(LedR,0x0000);
		Set_Fixed_Led_Effect(LedG,0xFFFF);
	}
}
void Off_Begin		(void)
{
	DBG_SPI_PHISICAL_PRINT("Off Begin\r\n");
	SET_TIMERA1_mSEC(200); 		//aprovecho este timer que ademas el handler consulta por algun requisito de off, para tesetear si hay alguna potencia mayor a cero... si no la hay... se queda frenado.. y si la hay me aseguro de arrancar siempre en rampa
    	HWREG(TIMERA1_BASE + TIMER_O_ICR) = HWREG(TIMERA1_BASE + TIMER_O_MIS); //clear flag irq
	TIMERA1_A_INT_ENABLE();
	Set_Led_Effect(LedR,0x0000);
	Set_Led_Effect(LedG,0x8888);
}
void Testing_Power_B 			(void)
{
	DBG_SPI_PHISICAL_PRINT("Testing_Power_B\r\n");
	if(!Is_All_Wished_Power_Zero())
		Atomic_Send_Event(Power_Enable_Event,Spi_Phisical());
}
void Testing_Power_Power_Enable		(void)
{
	DBG_SPI_PHISICAL_PRINT("Testing Power Begin\r\n");
	Enable_100v();
	Set_Led_Effect(LedR,0x8888);
	Set_Led_Effect(LedG,0x0000);
	Actual_Channel=0;	
	Matrix_Codes=Read_Matrix_Code();
	SET_TIMERA0_uSECx10	(Read_A());	
	SET_TIMERA1_mSEC	(Read_B());
	Set_Actual_Power_All(0);
	Fire_Counter=0;				//cuenta desde cero hasta Read_E()
	Secuence_Counter=0;			//cuenta desde cero hasta Read_G() con lo cual si cambia G en tiempo de ejecucion, automaticamente se entera,,,
	Ramp_Up_Power_All();
    	HWREG(TIMERA0_BASE + TIMER_O_ICR) = HWREG(TIMERA0_BASE + TIMER_O_MIS);	//limpia flag
    	HWREG(TIMERA1_BASE + TIMER_O_ICR) = HWREG(TIMERA1_BASE + TIMER_O_MIS); //clear flag irq
	TIMERA0_A_INT_ENABLE();
//	TIMERA1_A_INT_ENABLE();
	Running=1;
}
void E1_B	(void)
{
	DBG_SPI_PHISICAL_PRINT("Ramping up E1_B\r\n");
	Actual_Channel=0;	
	Matrix_Codes=Read_Matrix_Code();
	SET_TIMERA0_uSECx10(Read_A());	
	SET_TIMERA1_mSEC(Read_B());
	if(++Fire_Counter>=Read_E()) {
	 	Atomic_Send_Event(Time_E_Event,Spi_Phisical());
		Set_Wished2Actual_Power_All();			//por las dudas me aseguro de que hayan llegado los valores de potencia a lo solicitado despues de la rampa porque quien sabe y me cambiaron en la mitad de la rampa las cosas....
	}
	else 	Ramp_Up_Power_All();
	TIMERA0_A_INT_ENABLE();
}
void E1_E	(void)	
{
	Fire_Counter=0;					//cuenta desde cero hasta Read_C()
 	DBG_SPI_PHISICAL_PRINT("Fin ramping up E1_E\r\n");
}
void C1_B	(void)
{
	unsigned short C=Read_C();
	DBG_SPI_PHISICAL_PRINT("Contraction C1_B\r\n");
	Actual_Channel=0;	
	Matrix_Codes=Read_Matrix_Code();
	SET_TIMERA0_uSECx10(Read_A());	
	SET_TIMERA1_mSEC(Read_B());
	if(C && ++Fire_Counter>=C) 	
		Atomic_Send_Event(Time_C_Event,Spi_Phisical());
	else
		Set_Wished2Actual_Power_All();				//por las dudas me aseguro de sincronizar el volumen mientras estoy en contraccino porque si me cambian el volumen, no me entero hasta la siguiente rampa...
	TIMERA0_A_INT_ENABLE();
}
void C1_C	(void)
{
 	DBG_SPI_PHISICAL_PRINT("Fin working C1_C\r\n");
	Fire_Counter=0;				//cuenta desde cero hasta Read_F()
}
void F1_B	(void)
{
	DBG_SPI_PHISICAL_PRINT("Ramping down F1_B\r\n");
	Actual_Channel=0;	
	Matrix_Codes=Read_Matrix_Code();
	SET_TIMERA0_uSECx10(Read_A());	
	SET_TIMERA1_mSEC(Read_B());
	if(++Fire_Counter>=Read_F()) 
		Atomic_Send_Event(Time_F_Event,Spi_Phisical());
	else {			
		Ramp_Down_Power_All();
		TIMERA0_A_INT_ENABLE();
	}
}
void F1_F	(void)
{
 	DBG_SPI_PHISICAL_PRINT("Fin ramping down F1_F\r\n");
	SET_TIMERA1_mSEC(Read_D()?(Read_D()*1000):100); 				//si me piden 0 segundos le doy un puchito de 100 mseg...
	Set_Actual_Power_All(0);							//como pueden haber cambiado la potencia en tiempo de ejecucion podria darse el caso de que no haya llegado a cero cuando hace la rampa.. porque la rama la va bajando en funcion del valor de wished power y la cantidad de disparos... si eso me lo cambian mientrass lo estoy moviendo puede que quede con algo de power...con esto me aseguro de que no...
	Send_Event2Event_Listener(Spi_Phisical_State_D_Event,'B'); 			//si alguien se enlisto para ser informado que aca empieza el descnso...recibira el mensaje.... util para sincronizar software....
}
void D1_B	(void)
{
 	DBG_SPI_PHISICAL_PRINT("Fin relajacion D1_B Secuencia=%d\r\n",Secuence_Counter);
	if(Read_G()!=0 && ++Secuence_Counter>=Read_G())	{
 		DBG_SPI_PHISICAL_PRINT("Cambio de banco\r\n");
		Toogle_Bank();			//cambia de parametros al otro grupo. a partir de ahora todo sigue igual solo que con otro numeros...
		Secuence_Counter=0;		//arranca en cero como siempre hasta Read_G
	}
	Actual_Channel=0;	
	Matrix_Codes=Read_Matrix_Code();
	SET_TIMERA0_uSECx10(Read_A());	
	SET_TIMERA1_mSEC(Read_B());
	Fire_Counter=0;
	Ramp_Up_Power_All();
	TIMERA0_A_INT_ENABLE();
}
void Anywere_Off	(void) 
{
	DBG_SPI_PHISICAL_PRINT("Turn Off\r\n");
 	Disable_100v();
	Set_Led_Effect(LedR,0x0000);
	Set_Led_Effect(LedG,0x8888);
	Turn_Off_Request=0;
	Running=0;
	TIMERA1_A_INT_DISABLE(); 		//apago tambien la irq b, que seria como una rti...ya que ahor no sirve mas...
}
void Anyware_Clip_Off(void)
{
	DBG_SPI_PHISICAL_PRINT("Clip Off\r\n");
	Set_Led_Effect(LedR,0x0000);
	Set_Led_Effect(LedG,0xAAAA);
 	Disable_100v();
	Turn_Off_Request=0;
	Running=0;
	TIMERA1_A_INT_DISABLE(); 		//apago tambien la irq b, que seria como una rti...ya que ahor no sirve mas...
}
void Anyware_Charger_Connected(void)
{
	DBG_SPI_PHISICAL_PRINT("Chager connected\r\n");
 	Disable_100v();
	Set_Led_Effect(Led2,0xF0F0); 	//no dio para efectos de carga.. si carga titila, sino se preupone cargado. 
	Set_Led_Effect(Led1,0x0000); 
	Set_Led_Effect(LedR,0xF0F0);
	Set_Led_Effect(LedG,0x0000);
	Turn_Off_Request=0;
	Running=0;
	TIMERA1_A_INT_DISABLE(); 		//apago tambien la irq b, que seria como una rti...ya que ahor no sirve mas...
}
static void Charger_Disconnected(void)
{
	DBG_SPI_PHISICAL_PRINT("Chager disconnected\r\n");
	Set_Led_Effect(LedR,0x0000);
	Set_Led_Effect(LedG,0x8888);
	Set_Led_Effect(Led1,0x0000);
	Set_Led_Effect(Led2,0x0000);
}
//----------------------------------------------------------------------------------------------------
void			Turn_Off_Spi_Phisical 	(void) 	{Turn_Off_Request=1;}
void			Turn_On_Spi_Phisical 	(void)  {Atomic_Send_Event(Begin_Event,Spi_Phisical());}
//----------------------------------------------------------------------------------------------------
const State**		Spi_Phisical		(void) {return &Spi_Phisical_Sm;}
void			Spi_Phisical_Rti	(void) {Atomic_Send_Event(ANY_Event,Spi_Phisical());}
//----------------------------------------------------------------------------------------------------/*{{{*/
//unsigned int	Read_Spi_Phisical_State		(void)	
//{
//	if(Spi_Phisical_Sm==Charging)		return Spi_Phisical_State_Event+0;
//	if(Spi_Phisical_Sm==Clip_Off)		return Spi_Phisical_State_Event+1;
//	if(Spi_Phisical_Sm==Off)		return Spi_Phisical_State_Event+2;
//	if(Spi_Phisical_Sm==Testing_Power)	return Spi_Phisical_State_Event+3;
//	if(Spi_Phisical_Sm==E1)			return Spi_Phisical_State_Event+4;
//	if(Spi_Phisical_Sm==C1)			return Spi_Phisical_State_Event+5;
//	if(Spi_Phisical_Sm==F1)			return Spi_Phisical_State_Event+6;
//	if(Spi_Phisical_Sm==D1)			return Spi_Phisical_State_Event+7;
//						return Spi_Phisical_State_Event+8;
//}	/*}}}*/
//----------------------------------------------------------------------------------------------------
static const State Charging[] =
{
 Charger_Connected_Event	,Update_Effect_Charger		,Charging,
 Charger_Disconnected_Event	,Charger_Disconnected		,Off,
 ANY_Event                      ,Rien			 	,Charging,
};
static const State Clip_Off[] =
{
 Clip_On_Event			,Off_Begin			,Testing_Power,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 ANY_Event                      ,Rien			 	,Clip_Off,
};
static const State Off[] =
{
 Begin_Event			,Off_Begin			,Testing_Power,
// Clip_Off_Event			,Anyware_Clip_Off		,Clip_Off,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 ANY_Event                      ,Rien			 	,Off,
};
static const State Testing_Power[] =
{
 Time_B_Event			,Testing_Power_B		,Testing_Power,
 Power_Enable_Event 		,Testing_Power_Power_Enable	,E1,
 Off_Event			,Anywere_Off			,Off,
 Clip_Off_Event			,Anyware_Clip_Off		,Clip_Off,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 ANY_Event                      ,Rien			 	,Testing_Power,
};
static const State E1[] = 
{
 Time_B_Event			,E1_B				,E1,
 Time_E_Event			,E1_E				,C1,
 Off_Event			,Anywere_Off			,Off,
 Clip_Off_Event			,Anyware_Clip_Off		,Clip_Off,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 Power_Disable_Event 		,Off_Begin			,Testing_Power,
 ANY_Event			,Rien				,E1,
};
static const State C1[] = 
{
 Time_B_Event			,C1_B				,C1,
 Time_C_Event			,C1_C				,F1,
 Off_Event			,Anywere_Off			,Off,
 Clip_Off_Event			,Anyware_Clip_Off		,Clip_Off,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 Power_Disable_Event 		,Off_Begin			,Testing_Power,
 ANY_Event			,Rien				,C1,
};
static const State F1[] = 
{
 Time_B_Event			,F1_B				,F1,
 Time_F_Event			,F1_F				,D1,
 Off_Event			,Anywere_Off			,Off,
 Clip_Off_Event			,Anyware_Clip_Off		,Clip_Off,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 Power_Disable_Event 		,Off_Begin			,Testing_Power,
 ANY_Event			,Rien				,F1,
};
static const State D1[] = 
{
 Time_B_Event			,D1_B				,E1,
 Off_Event			,Anywere_Off			,Off,
 Clip_Off_Event			,Anyware_Clip_Off		,Clip_Off,
 Charger_Connected_Event	,Anyware_Charger_Connected	,Charging,
 Power_Disable_Event 		,Off_Begin			,Testing_Power,
 ANY_Event			,Rien				,D1,
};
//----------------------------------------------------------------------------------------------------

