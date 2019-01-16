#include "hw_types.h"/*{{{*/
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "flash.h"
#include "state_machine.h"
#include "events.h"
#include "buttons.h"
#include "simplelink.h"
#include "schedule.h"
#include "debug.h"
#include "str.h"
#include "type_conversion.h"
#include "uart_if.h"/*}}}*/
static const  State 
	Off[];

const State 	*Flash_Sm;
signed long File_Create(unsigned char *Fname,unsigned short int Size)
{
	signed long File,Ret;
	Ret=sl_FsOpen(Fname,FS_MODE_OPEN_CREATE(Size,_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE),0,&File);
	DBG_FLASH_PRINT("Crea archivo %s ok?= %d\r\n",Fname,Ret);
        sl_FsClose(File, 0, 0, 0);
	return Ret;
}
signed long File_Read(unsigned char *Fname,unsigned char *Buf,unsigned short int Offset,unsigned short int Size)
{
	signed long File,Ret;
    	if((Ret=sl_FsOpen(Fname,FS_MODE_OPEN_READ,0,&File))==0) 
		Ret=sl_FsRead(File,Offset,Buf,MIN(File_Len(Fname),Size));
	DBG_FLASH_PRINT("Lee archivo %s ok?= %d\r\n",Fname,Ret);
  	sl_FsClose(File, 0, 0, 0);
	return Ret;
}
signed long File_Write(unsigned char *Fname,unsigned char *Buf,unsigned short int Size)
{
	signed long File,Ret;
   	if(sl_FsOpen(Fname,FS_MODE_OPEN_WRITE,0,&File)==0) 
        	Ret=sl_FsWrite(File,0,Buf,Size);
	DBG_FLASH_PRINT("Graba archivo %s ok?= %d\r\n",Fname,Ret);
    	sl_FsClose(File, 0, 0, 0);
	return Ret;
}
signed int File_Info(unsigned char *Fname,SlFsFileInfo_t* Fileinfo)
{
	signed int Ret;
   	Ret=sl_FsGetInfo(Fname,0,Fileinfo); 
	DBG_FLASH_PRINT("Informacion del archivo %s retorno %d Flags=%d Length=%d Allocated=%d Token=%d\r\n",Fname,Ret,Fileinfo->flags,Fileinfo->FileLen,Fileinfo->AllocatedLen,Fileinfo->Token[0]);
	return Ret;
}
unsigned int File_Len(unsigned char *Fname)
{
	SlFsFileInfo_t Fileinfo;
	File_Info(Fname,&Fileinfo);
   	return Fileinfo.AllocatedLen; 
}
signed int File_Del(unsigned char *Fname)
{
	signed int Ret;
   	Ret=sl_FsDel(Fname,0); 
	DBG_FLASH_PRINT("Archivo %s borrado retorno %d\r\n",Fname,Ret);
	return Ret;
}
signed long File_Copy(unsigned char *Src,unsigned char *Dst)
{
	signed long Ret;
	unsigned char Buf[MAX_FILE_LENGTH];
	Ret=File_Read(Src,Buf,0,MAX_FILE_LENGTH);
	if(Ret>0) {
		File_Create(Dst,Ret);
		Ret=File_Write(Dst,Buf,Ret);
	}	
	DBG_FLASH_PRINT("Copia archivo %s en %s tamanio %d\r\n",Src,Dst,Ret);
	return Ret;
}
signed long File_Append(unsigned char *Fname,unsigned char *Append_Buf,unsigned int Append_Length)
{
	signed long Ret;
	unsigned int Actual_Append_Length;
	unsigned char Buf[MAX_FILE_LENGTH];
	Ret=File_Read(Fname,Buf,0,MAX_FILE_LENGTH);
	if(Ret<0) Ret=0;
	Actual_Append_Length=(Ret+Append_Length)<MAX_FILE_LENGTH?Append_Length:(MAX_FILE_LENGTH-Ret-1);	//encontre que si el file dice que tiene allocated 1000 solo pude grabarle 999...por eso el menos 1.. pero no se... es raro.. ahora no me calienta, pero habria que revisarlo bien..
	String_Copy(Append_Buf,Buf+Ret,Actual_Append_Length);
	File_Create(Fname,MAX_FILE_LENGTH);
	Ret=File_Write(Fname,Buf,Ret+Actual_Append_Length);

	DBG_FLASH_PRINT("Append %d bytes al archivo %s tamanio final %d\r\n",Append_Length,Fname,Ret);
	return Ret;
}	

//-------------------------------------------------------------------
void Init_Flash(void)
{
 Flash_Sm=Off;
}

const State** 	Flash		(void) 			{return &Flash_Sm;}
void 		Flash_Rti	(void) 			{Atomic_Send_Event(ANY_Event,Flash());}

//-------------------------------------------------------------------
static const State Off[] =
{
 ANY_Event                      ,Rien			 	,Off,
};
//-------------------------------------------------------------------------------
//
