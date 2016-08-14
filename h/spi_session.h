#ifndef SPI_SESSION
#define SPI_SESSION

#include "state_machine.h"

enum Spi_Session_Event_Code{
 			Soft_Control_Event	= 0xE000
	};
//------------------------------------------------------
struct Struct_Params
{
 unsigned char 		A1;			//1=10uSec. vale de 1 a 40
 unsigned short		B1;			//1=1mSeg.  vale de 5 a 1000 (200hz a 1hz)
 unsigned short		C1;			//1=1 repeticion de B. vale de 1 a 4000 
 unsigned char		D1;			//1=1Seg.   vale de 1 a 50  
 unsigned char 		E1;			//1=1B  .   vale de 1 a 255. Cuantos B  se necesitan para alcanzar la potencia solicitada en cada canal. Si P1=200 y Ramp_Up=10,  en cada B1, se incrementara la potencia de ese canal en 20 puntos  cosa que en 10 B1 se llegue a 200
 unsigned char 		F1;			//1=1B  .   vale de 1 a 255. Cuantos B  se necesitan para reducir  la potencia a cero     en cada canal. Si P1=200 y Ramp_Down=2, en cada B1, se decrementara la potencia de ese canal en 100 puntos cosa que en 2  B1 se llegue a   0
 unsigned char		G1;			//1=1repeticion.   vale de 1 a 255 
 unsigned char 		A2;			//1=10uSec. vale de 1 a 40
 unsigned short		B2;			//1=1mSeg.  vale de 5 a 1000 (200hz a 1hz)
 unsigned short		C2;			//1=1 repeticion de B. vale de 1 a 4000 
 unsigned char		D2;			//1=1Seg.   vale de 1 a 50  
 unsigned char 		E2;			//1=1B  .   vale de 1 a 255. Cuantos B  se necesitan para alcanzar la potencia solicitada en cada canal. Si P1=200 y Ramp_Up=10,  en cada B1, se incrementara la potencia de ese canal en 20 puntos  cosa que en 10 B1 se llegue a 200
 unsigned char 		F2;			//1=1B  .   vale de 1 a 255. Cuantos B  se necesitan para reducir  la potencia a cero     en cada canal. Si P1=200 y Ramp_Down=2, en cada B1, se decrementara la potencia de ese canal en 100 puntos cosa que en 2  B1 se llegue a   0
 unsigned char		G2;			//1=1repeticion.   vale de 1 a 255 
 unsigned char		Bank;			//bool, dice si se trata de la serie 1 o 2
};
//------------------------------------------------------
extern void 		Init_Spi_Session			(void);
extern const State** 	Spi_Session				(void);
extern void 		Spi_Session_Rti				(void);
//------------------------------------------------------
extern unsigned int* 	Read_Matrix_Code			(void);
//------------------------------------------------------
extern void 		Init_100v 				(void);
extern void 		Enable_100v 				(void);
extern void 		Disable_100v 				(void);
extern unsigned char 	Read_Wished_Power			(unsigned char Channel);
extern unsigned char 	Read_Actual_Power			(unsigned char Channel);
extern void 		Set_Wished_Power			(unsigned char Channel,unsigned char Power);
extern void 		Set_Actual_Power			(unsigned char Channel,unsigned char Power);
extern void 		Set_Actual_Power_All			(unsigned char Power);
extern void 		Inc_Wished_Power 			(unsigned char Channel,unsigned char Increment);	
extern void 		Dec_Wished_Power 			(unsigned char Channel,unsigned char Decrement);	
extern void 		Inc_Wished_Power_AllxX			(unsigned char X);
extern void 		Inc_Wished_Power_Allx5			(void);		
extern void 		Dec_Wished_Power_AllxX			(unsigned char X);
extern void 		Dec_Wished_Power_Allx5			(void);			
extern void 		Inc_Actual_Power 			(unsigned char Channel,unsigned char Increment);	
extern void 		Dec_Actual_Power 			(unsigned char Channel,unsigned char Decrement);	
extern unsigned char 	Is_All_Wished_Power_Zero		(void);
extern void 		Ramp_Up_Power_All			(void);
extern void 		Ramp_Down_Power_All 			(void);
extern void 		Set_Wished_Power_All			(unsigned char Power);	
extern void 		Set_Actual_Power_All			(unsigned char Power);	
extern void 		Set_Wished2Actual_Power_All		(void);
extern void 		Secuence_Wished_Power			(void);
extern void 		Begin_Power_Secuence			(void);
extern void 		End_Power_Secuence			(void);
//------------------------------------------------------
extern unsigned char 	Read_A					(void);
extern unsigned short	Read_B					(void);
extern unsigned short 	Read_C					(void);
extern unsigned char 	Read_D					(void);
extern unsigned char 	Read_E					(void);
extern unsigned char 	Read_F					(void);
extern unsigned char 	Read_G					(void);
extern void 		Toogle_Bank				(void);	
extern void 		Set_Bank1				(void);	
extern void 		Set_Bank2				(void);	
extern unsigned char 	Read_A1 				(void);	
extern unsigned short	Read_B1 				(void);	
extern unsigned short 	Read_C1 				(void);	
extern unsigned char 	Read_D1 				(void);	
extern unsigned char 	Read_E1 				(void);	
extern unsigned char 	Read_F1 				(void);	
extern unsigned char 	Read_G1 				(void);	
extern unsigned char 	Read_A2 				(void);	
extern unsigned short	Read_B2 				(void);	
extern unsigned short 	Read_C2 				(void);	
extern unsigned char 	Read_D2 				(void);	
extern unsigned char 	Read_E2 				(void);	
extern unsigned char 	Read_F2 				(void);	
extern unsigned char 	Read_G2 				(void);	
//------------------------------------------------------
extern void  		Set_A1					(unsigned char  A);
extern void  		Set_B1					(unsigned short B);
extern void  		Set_C1					(unsigned short C);
extern void  		Set_D1					(unsigned char  D);
extern void  		Set_E1					(unsigned char  E);
extern void  		Set_F1					(unsigned char  F);
extern void  		Set_G1					(unsigned char  G);
extern void  		Set_A2					(unsigned char  A);
extern void  		Set_B2					(unsigned short B);
extern void  		Set_C2					(unsigned short C);
extern void  		Set_D2					(unsigned char  D);
extern void  		Set_E2					(unsigned char  E);
extern void  		Set_F2					(unsigned char  F);
extern void  		Set_G2					(unsigned char  G);
//------------------------------------------------------
extern void 		Wifi_Selection_Effect 			(void);
extern void 		Wifi_Control1_Delayed_Led_Effect	(void);
extern void 		Wifi_Control2_Delayed_Led_Effect	(void);
extern void 		Wifi_Dci_Delayed_Led_Effect		(void);
//------------------------------------------------------
#endif

