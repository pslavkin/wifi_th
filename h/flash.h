#ifndef  FLASH
#define  FLASH

#include "state_machine.h"
#include "fs.h"
//-----------------------------------------------------------
enum Flash_Event_Code{
				Flash_Null_Event	= 0xD000
		           };

#define	MAX_FILE_LENGTH 3656
//-----------------------------------------------------------
//
extern void			Flash_Rti	(void);
extern void			Init_Flash	(void);
extern const State**		Flash		(void);
extern signed long	 	File_Create	(unsigned char *Fname,unsigned short int Size);
extern signed long	 	File_Read	(unsigned char *Fname,unsigned char *Buf,unsigned short int Offset,unsigned short int Size);
extern signed long	 	File_Write	(unsigned char *Fname,unsigned char *Buf,unsigned short int Size);
extern signed int	 	File_Info	(unsigned char *Fname,SlFsFileInfo_t* Fileinfo);
extern signed int 		File_Del	(unsigned char *Fname);
extern unsigned int 		File_Len	(unsigned char *Fname);
extern signed long 		File_Copy	(unsigned char *Src,unsigned char *Dst);
extern signed long 		File_Append	(unsigned char *Fname,unsigned char *Append_Buf,unsigned int Append_Length);
//---------------------------------------------------------

#endif

