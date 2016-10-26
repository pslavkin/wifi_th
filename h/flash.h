#ifndef  FLASH
#define  FLASH

#include "state_machine.h"
#include "fs.h"
//-----------------------------------------------------------
enum Flash_Event_Code{
				Flash_Null_Event	= 0xD000
		           };
//-----------------------------------------------------------
extern void			Flash_Rti	(void);
extern void			Init_Flash	(void);
extern const State**		Flash		(void);
extern unsigned short int 	Create_File	(unsigned char *Fname,unsigned short int Size);
extern unsigned short int 	Read_File	(unsigned char *Fname,unsigned char *Buf,unsigned short int Size);
extern unsigned short int 	Write_File	(unsigned char *Fname,unsigned char *Buf,unsigned short int Size);
extern unsigned short int 	Info_File	(unsigned char *Fname,SlFsFileInfo_t* Fileinfo);
extern unsigned short int 	Del_File	(unsigned char *Fname);
//---------------------------------------------------------

#endif

