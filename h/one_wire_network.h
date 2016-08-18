#ifndef ONE_WIRE_NETWORK
#define ONE_WIRE_NETWORK

#define ONE_WIRE_RX_BUFFER	20//20//1	//es tambien el tamaño maximo de algun comando...generalmente no pasa de 20, que es un match ROM + read Page
#define MAX_ROM_CODES		1//20//20//1	//cantidad de nodos permitidos en la red..
#define CRC_FILTER		4	//mantiene por 4 lecturas el valor anterior antes de dar error.en el alteultimo resetea el bus...

enum One_Wire_Network_Events 
       	{
	 End_Of_One_Wire_Msg_Event	=0x1000,
	 One_Wire_Detected_Event	=0x1001,
	 One_Wire_Not_Detected_Event	=0x1002,
	 One_Wire_Cmd_Event		=0x1003,
	 One_Wire_End_Of_Read_Event	=0x1004,
	 One_Wire_Read_Next_Byte_Event	=0x1005,
	 Search_Codes_Event		=0x1006,
	 Actual_Code_End_Event		=0x1007,
	 Search_Codes_End_Event		=0x1008,
	 Smaller_Discrepance_Event	=0x1009,
	 Equal_Discrepance_Event	=0x100A,
	 Bigger_Discrepance_Event	=0x100B,
	 Crc_Fail_Event			=0x100C,
	 Crc_Ok_Event			=0x100D,
	 Nobody_On_Bus_Event		=0x100E,
	 Anybody_On_Bus_Event		=0x100F
	};
//------------------------------------------------------ 
typedef struct Rom_Codes_Struct
{
 unsigned char Code[8];		//guarda el cogido del nodo...
 unsigned int T;		//guarda la temperatura actual del nodo, 
 unsigned int V;		//guarda la tension medida en el modelo DS2438
 unsigned char Crc;		//guarda la tension medida en el modelo DS2438
};
//------------------------------------------------------ 
extern void 		Init_One_Wire_Network	(void);
extern const State** 	One_Wire_Network	(void);
//-----------
extern unsigned char  	One_Wire_Rx_As_Char	(unsigned char Pos);
extern unsigned char* 	One_Wire_Rx_As_PChar	(unsigned char Pos);
//------------------Comandos ----------------------------------
extern void 		Execute_Cmd		(unsigned char Length,unsigned char* Cmd);
extern void 		Read_Rom		(void);
extern void 		Match_Rom		(unsigned char* Rom_Code);
extern void 		Skip_Rom		(void);
extern void 		Broadcast_V		(void);
extern void 		Broadcast_T		(void);	
extern void 		Broadcast_Recall_Page0	(void);
//----------------- ALL -------------------------------
extern unsigned char 	On_Line_Nodes		(void);
extern unsigned char 	Family_Code		(unsigned char Node);
extern void 		Print_Rom_Codes		(void);
extern void 		Search_Codes		(void);
extern unsigned int 	Convert_Nodes_Bin2Ascci	(unsigned char* Buf);
extern unsigned char 	Read_One_Wire_Crc	(unsigned char Node);
extern unsigned int 	Read_One_Wire_T		(unsigned char Node);
//------------------DS18S20------------------------------------ 
extern void 		Read_DS18S20_Scratchpad		(unsigned char Node);
extern void 		Calculate_DS18S20_12Bit_T	(unsigned char Node);
extern unsigned char 	DS18S20_Convert_Bin2Ascci_T	(unsigned char* Destiny,unsigned char Code);
//------------------DS18B20------------------------------------ 
extern void 		Read_DS18B20_Scratchpad		(unsigned char Node);
extern void 		Calculate_DS18B20_12Bit_T	(unsigned char Node);
extern unsigned char 	DS18B20_Convert_Bin2Ascci_T	(unsigned char* Destiny,unsigned char Code);
//------------------DS2438------------------------------------ 
extern void 		Read_DS2834_Scratchpad_Page	(unsigned char Node,unsigned char Page);
extern void 		Calculate_DS2834_T_V		(unsigned char Node);
extern unsigned char 	DS2438_Convert_Bin2Ascci_T_V	(unsigned char* Destiny,unsigned char Code);
//------------------------------------------------------ 

#endif
