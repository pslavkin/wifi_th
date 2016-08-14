#ifndef  LEDS_SESSION
#define  LEDS_SESSION

#include "state_machine.h"
//-----------------------------------------------------------
enum Leds_Session_Event_Code{
				Leds_Session_Null_Event	= 0xD000
		           };
//-----------------------------------------------------------
extern void 	Init_Leds		(void);
extern void 	Led1_On			(void);
extern void 	Led1_Off		(void);
extern void 	Led2_On			(void);
extern void 	Led2_Off		(void);
extern void 	Led3_On			(void);
extern void 	Led3_Off		(void);
extern void 	LedR_Off		(void);	
extern void 	LedR_On			(void);	
extern void 	LedG_Off		(void);	
extern void 	LedG_On			(void);	
extern void 	LedB_Off		(void);	
extern void 	LedB_On			(void);	
//-----------------------------------------------------------
extern void 		Leds_Session_Rti	(void);
extern void 		Init_Leds_Session	(void);
extern const State** 	Leds_Session		(void);
//----------------------------------------------------
extern void	Set_Led_Effect		(unsigned char Led,unsigned int Effect);
extern void	Set_Temp_Led_Effect	(unsigned char Led,unsigned int Effect);
extern void	Set_Fixed_Led_Effect	(unsigned char Led,unsigned int Effect);
extern void 	Set_Toogle_Led_Effect	(unsigned char Led);
//---------------------------------------------------------
enum Leds_Position
{
 Led1	=0,
 Led2	=1,
 Led3	=2,
 LedR	=3,
 LedG	=4,
 LedB	=5,
};
struct Led_Effect_Struct
 {
  unsigned int Effect;
  unsigned int Temp_Effect;
  void (*On_Function)(void);
  void (*Off_Function)(void);
 };
//---------------------------------------------------------

#endif

