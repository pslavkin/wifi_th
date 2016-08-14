#ifndef BAT
#define BAT

#define RAW_BAT_MAX 		2200	
#define RAW_BAT_MIN 		1850
#define BAT_MIN 		1  	//porciento
#define CHARGER_THRESHOLD 	1300	//arriba de esto considero cargador conectado
//------------------------------------------------------
extern void 			Init_Charger		(void);	
extern void 			Test_Charger		(void); 	
extern unsigned char 		Is_Charger_Connected	(void);
extern void 			Dont_Care_Charger	(void);
extern void 			Do_Care_Charger		(void);
//-------------------------------------------------------------
extern void 			Init_Charger_Status	(void);	
extern void 			Test_Charger_Status	(void); 	
extern unsigned char		Is_Bat_Charging		(void);
//-------------------------------------------------------------
extern void 			Init_Bat		(void);	
extern void 			Test_Bat		(void); 	
extern unsigned char 		Read_Bat_Level		(void); 	
//------------------------------------------------------
#endif
