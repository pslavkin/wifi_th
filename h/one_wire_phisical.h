#ifndef ONE_WIRE_PHISICAL
#define ONE_WIRE_PHISICAL

#define READ_ROM_STRING		"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x33"
#define SKIP_ROM_STRING		"\xCC"
#define SEARCH_ROM_STRING	"\xF0"

enum One_Wire_Phisical_Commands 
{
 READ_ROM	=0x33,
 SKIP_ROM	=0xCC,
 MATCH_ROM	=0x55,
 SEARCH_ROM	=0xF0
};
enum One_Wire_DS18S20_Network_Command
{
 READ_SCRATCHPAD	=0xBE,
 WRITE_SCRATCHPAD	=0x4E,
 COPY_SCRATCHPAD	=0x48,
 RECALL_E2		=0xB8,
 READ_POWER_SUPPLY	=0xB4,
 CONVERT_T		=0x44
};
enum One_Wire_DS1990_Network_Command
{
 READ_MEMORY		=0xF0,
 EXTENDED_READ_MEMORY	=0xA5,
 READ_SUBKEY		=0xA5,
 WRITE_SUBKEY		=0x99,
 WRITE_PASWSWORD	=0x5A,
 WRITE_MEMORY		=0x0F,
 WRITE_STATUS		=0x55,
 READ_STATUS		=0xAA
};

//----------------------------------
#define ONE_WIRE_BUS_LO()	(1) //PTS&=~(1<<1),PTJ|=(1<<6))
#define ONE_WIRE_BUS_HI()	(1) //PTS|= (1<<1),PTJ|=(1<<6))
#define ONE_WIRE_BUS_HIZ()	(1) //PTJ&=~(1<<6))
#define STRONG_PULLUP()		(1) //PTS|=(1<<1),PTJ|=(1<<6),_asm("nop"),PTJ&=~(1<<6))
#define ONE_WIRE_BUS_READ()	(1) //(PTIS&(1<<0))!=0)
#define INIT_ONE_WIRE_BUS()	(1) //DDRS|=(1<<1),DDRJ|=(1<<6))		//el pin de tx como salida, el de Rx como entradaa sin pullups porque en este circuito tiene una externa con un diodo haciendo on OR
//------------------------------------------------------ 
extern void 		Init_One_Wire_Phicical		(void);
extern void 		One_Wire_Power_On_Reset		(void);
extern unsigned char 	Presence			(void);
extern unsigned char	Write_Bit_One_And_Read		(void);
extern unsigned char 	Write_Bit_Zero			(void);
extern unsigned char 	Write_Read_Bit			(unsigned char Bit);
extern unsigned char 	Read2Bits			(void);
extern unsigned char 	Write_Read_Byte			(unsigned char Data);
//------------------------------------------------------ 
extern void 		Inc_One_Wire_Read_Delay		(void);
extern void 		Dec_One_Wire_Read_Delay		(void);
extern void 		Send_One_Wire_Read_Delay2Tcp	(void);
//------------------------------------------------------ 
#endif

