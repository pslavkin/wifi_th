#ifndef SCHEDULE
#define SCHEDULE
#include <state_machine.h>
//------------------------------------------------------------------------
#define MAX_SCHEDULE_INDEX 		25    	//define el maximo numero de solicitantes de tiem+outs concurrentes...

enum Schedule_Events 
       	{
	 Timeout_1Sec_Event	=0x5000,
	 Timeout_2Sec_Event	=0x5001,
	 Timeout_1Min_Event	=0x5002,
	 Timeout_10Sec_Event	=0x5003
	};
//------------------------------------------------------------------------
extern void 		Init_Schedule				(void);
extern void 		Schedule				(void);
extern unsigned char 	Update_Schedule				(unsigned int Time_Out,unsigned int Event,const State** Machine);
//------------------------------------------------------------------------
extern void 		Update_Func_Schedule			(unsigned int Time_Out,void (*Func)(void));
extern void 		Free_Schedule				(unsigned int Event,const State** Machine);
extern void 		Free_All_Schedule			(const State** Machine);
extern void 		Pause_Schedule				(unsigned int Event,const State** Machine);
extern unsigned char 	Resume_Schedule				(unsigned int Event,const State** Machine);
extern void 		New_Periodic_Schedule			(unsigned int Time_Out,unsigned int Event,const State** Machine);
extern void 		New_None_Periodic_Schedule		(unsigned int Time_Out,unsigned int Event,const State** Machine);
extern unsigned int 	Read_Schedule_TOut			(unsigned int Event,const State** Machine);
//------------------------------------------------------------------------
extern void 		New_None_Periodic_Func_Schedule		(unsigned int Time_Out,void (*Func)(void));
extern void 		New_Periodic_Func_Schedule		(unsigned int Time_Out,void (*Func)(void));
extern void 		Pause_Func_Schedule			(void (*Func)(void));
extern unsigned char 	Resume_Func_Schedule			(void (*Func)(void));
extern void 		Free_Func_Schedule			(void (*Func)(void));
extern void 		Resume_Or_New_Periodic_Func_Schedule	(unsigned int Time_Out,void (*Func)(void));
extern void 		Update_Or_New_None_Periodic_Schedule	(unsigned int Time_Out,unsigned int Event,const State** Machine);
//------------------------------------------------------------------------
extern void 		Periodic_1Sec4Sm			(const State** Machine);
extern void 		None_Periodic_1Sec			(void);
extern void 		Free_Schedule_1Sec			(void);
//-----------------------------------------
extern void 		None_Periodic_1Min			(void);
extern void 		Periodic_1Min				(void);
extern void 		Pause_Periodic_1Min			(void);
extern void 		Resume_Periodic_1Min			(void);
extern void 		Free_Schedule_1Min			(void);
extern void 		Resume_Or_New_Periodic_1Min		(void);
//----------------------------

#endif

