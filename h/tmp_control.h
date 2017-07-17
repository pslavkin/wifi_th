#ifndef TMP_CONTROL
#define TMP_CONTROL

//---------------------------------------------------------
enum Tmp_Control_Event_Code
{
	Test_Tmp_Event		=0x6000,
	Tmp_Fail_Event		=0x6001,
	Inside_Event		=0x6002,
	Below_Event		=0x6003,
	Above_Event		=0x6004
};

#define TEST_TMP_PERIOD	2
//---------------------------------------------------------
extern void 		Init_Tmp_Control		(void);
extern const State** 	Tmp_Control			(void);
//---------------------------------------------------------------------------------
extern void 		Save_Max_Tmp4Loaded_Tmp		(void);
extern void 		Save_Min_Tmp4Loaded_Tmp		(void);
extern void 		Save_Windows_Tmp4Loaded_Tmp	(void);

extern void 		Send_Max_Tmp2Tcp		(void);
extern void 		Send_Min_Tmp2Tcp		(void);
extern void 		Send_Windows_Tmp2Tcp		(void);

extern void		Toogle_Tmp_Email		(void);
extern void 		Send_Tmp_Email2Tcp		(void);

extern void		Toogle_Tmp_Relay		(void);
extern void 		Send_Tmp_Relay2Tcp		(void);

extern void 		Toogle_Tmp_Sd			(void);
extern void 		Send_Tmp_Sd2Tcp			(void);
//---------------------------------------------------------------------------------
extern void 		Save_Tmp_Control_State		(unsigned char* Buf);
extern void 		Send_Tmp_Control_State2Tcp	(void);
//---------------------------------------------------------------------------------



#endif

