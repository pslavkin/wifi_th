#ifndef CLIPS
#define CLIPS

#define MAX_CLIP_COUNTER 2
//------------------------------------------------------
extern void 		Init_Clip		(void);
extern void 		Test_Clip		(void);
extern unsigned char 	Read_Clip_State		(void);
extern void 		Dont_Care_Clip		(void); 
extern void 		Do_Care_Clip		(void); 
//------------------------------------------------------
#endif
