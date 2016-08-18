#ifndef ONE_WIRE_TRANSPORT
#define ONE_WIRE_TRANSPORT


#define ONE_WIRE		//si esta definido se usara en los mails y el el control de temperatura

#define MIN_RELOAD_ONE_WIRE_NODES_TOUT	5

enum One_Wire_Transport_Events 
       	{
	 End_Of_Nodes_Event		=0x2000,
	 Reload_Codes_Event		=0x2001
	};
enum One_Wire_Maxim_Ic_Codes 
       	{
	 DS18S20			=0x10,
	 DS18B20			=0x28,
	 DS2438				=0x26
	};
//------------------------------------------------------ 
extern void 		Init_One_Wire_Transport		(void);
extern const State** 	One_Wire_Transport		(void);
//------------------------------------------------------ 
extern void 		Reload_One_Wire_Codes		(void);
extern void 		Send_Temp2Tcp			(void);
extern unsigned char 	One_Wire_Bin2Ascci		(unsigned char Node,unsigned char *Buf);
extern signed int 	One_Wire_Bin			(unsigned char Pos);
extern void 		Begin_Send_One_Wire_Info2Tcp	(void);
extern void 		Send_Next_One_Wire_Info2Tcp	(void);
extern unsigned char 	Convert_Node_Bin2Ascci		(unsigned char Node,unsigned char* Buf);
extern void 		One_Wire_T2Tcp			(unsigned char Node);
//------------------------------------------------------ 
extern void 		Refresh_Reload_One_Wire_Nodes_TOut		(unsigned int Time);
extern void 		Save_Reload_One_Wire_Nodes_TOut			(unsigned int TOut);
extern void 		Save_Reload_One_Wire_Nodes_TOut4Loaded_Int	(void);
extern void 		Send_Reload_One_Wire_Nodes_TOut2Tcp		(void);
//------------------------------------------------------ 
extern void 		Send_One_Wire_Transport_State2Socket_Fifo	(void);

#endif
