#ifndef  LEDS_SESSION
#define  LEDS_SESSION

#include "state_machine.h"
//-----------------------------------------------------------
enum Leds_Session_Event_Code{
				Leds_Session_Null_Event	= 0xD000
		           };
//-----------------------------------------------------------
extern void 	Init_Leds		(void);
extern void 	Led_Run_On		(void);
extern void 	Led_Run_Off		(void);
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
 Led_Run	=0,
 Led_TR		=1,
 Led_TG		=2,
 Led_TB		=3,
 Led_THR	=4,
 Led_THG	=5,
 Led_THB	=6,
 Buzzer		=7
};
struct Led_Effect_Struct
 {
  unsigned int Effect;
  unsigned int Temp_Effect;
  void (*On_Function)(void);
  void (*Off_Function)(void);
 };
//---------------------------------------------------------
extern void Led_Run_Off		(void);
extern void Led_Run_On		(void);
extern void Led_TR_Off		(void);
extern void Led_TR_On		(void);
extern void Led_TG_Off		(void);
extern void Led_TG_On		(void);
extern void Led_TB_Off		(void);
extern void Led_TB_On		(void);
extern void Led_THR_Off		(void);
extern void Led_THR_On		(void);
extern void Led_THG_Off		(void);
extern void Led_THG_On		(void);
extern void Led_THB_Off		(void);
extern void Led_THB_On		(void);

#endif

