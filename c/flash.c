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
#include "uart_if.h"/*}}}*/
static const  State 
	Off[];

const State 	*Flash_Sm;
long 		lFileHandle;
unsigned long 	ulToken;
//-------------------------------------------------------------------
//long WriteFileToDevice(unsigned long *ulToken, long *lFileHandle)
//{/*{{{*/
//    long lRetVal = -1;
//    int iLoopCnt = 0;
//
//    //
//    //  create a user file
//    //
//    lRetVal = sl_FsOpen((unsigned char *)USER_FILE_NAME,
//                FS_MODE_OPEN_CREATE(65536, \
//                          _FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE),
//                        ulToken,
//                        lFileHandle);
//    if(lRetVal < 0)
//    {
//        //
//        // File may already be created
//        //
//        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
//        ASSERT_ON_ERROR(lRetVal);
//    }
//    else
//    {
//        //
//        // close the user file
//        //
//        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
//        if (SL_RET_CODE_OK != lRetVal)
//        {
//            ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
//        }
//    }
//    
//    //
//    //  open a user file for writing
//    //
//    lRetVal = sl_FsOpen((unsigned char *)USER_FILE_NAME,
//                        FS_MODE_OPEN_WRITE, 
//                        ulToken,
//                        lFileHandle);
//    if(lRetVal < 0)
//    {
//        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
//        ASSERT_ON_ERROR(FILE_OPEN_WRITE_FAILED);
//    }
//    
//    //
//    // write "Old MacDonalds" child song as many times to get just below a 64KB file
//    //
//    for (iLoopCnt = 0; 
//            iLoopCnt < (SL_MAX_FILE_SIZE / sizeof(gaucOldMacDonald)); 
//            iLoopCnt++)
//    {
//        lRetVal = sl_FsWrite(*lFileHandle,
//                    (unsigned int)(iLoopCnt * sizeof(gaucOldMacDonald)), 
//                    (unsigned char *)gaucOldMacDonald, sizeof(gaucOldMacDonald));
//        if (lRetVal < 0)
//        {
//            lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
//            ASSERT_ON_ERROR(FILE_WRITE_FAILED);
//        }
//    }
//    
//    //
//    // close the user file
//    //
//    lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
//    if (SL_RET_CODE_OK != lRetVal)
//    {
//        ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
//    }
//
//    return SUCCESS;
//}/*}}}*/
//
//long ReadFileFromDevice(unsigned long ulToken, long lFileHandle)
//{/*{{{*/
//    long lRetVal = -1;
//    int iLoopCnt = 0;
//
//    //
//    // open a user file for reading
//    //
//    lRetVal = sl_FsOpen((unsigned char *)USER_FILE_NAME,
//                        FS_MODE_OPEN_READ,
//                        &ulToken,
//                        &lFileHandle);
//    if(lRetVal < 0)
//    {
//        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
//        ASSERT_ON_ERROR(FILE_OPEN_READ_FAILED);
//    }
//
//    //
//    // read the data and compare with the stored buffer
//    //
//    for (iLoopCnt = 0;
//            iLoopCnt < (SL_MAX_FILE_SIZE / sizeof(gaucOldMacDonald));
//            iLoopCnt++)
//    {
//        lRetVal = sl_FsRead(lFileHandle,
//                    (unsigned int)(iLoopCnt * sizeof(gaucOldMacDonald)),
//                     gaucCmpBuf, sizeof(gaucOldMacDonald));
//        if ((lRetVal < 0) || (lRetVal != sizeof(gaucOldMacDonald)))
//        {
//            lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
//            ASSERT_ON_ERROR(FILE_READ_FAILED);
//        }
//
//        lRetVal = memcmp(gaucOldMacDonald,
//                         gaucCmpBuf,
//                         sizeof(gaucOldMacDonald));
//        if (lRetVal != 0)
//        {
//            ASSERT_ON_ERROR(FILE_NOT_MATCHED);
//        }
//    }
//
//    //
//    // close the user file
//    //
//    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
//    if (SL_RET_CODE_OK != lRetVal)
//    {
//        ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
//    }
//
//    return SUCCESS;
//}/*}}}*/


	long lFileHandle;
	unsigned long ulToken;
#define USER_FILE_NAME "abc"

unsigned short int Create_File(unsigned char *Fname,unsigned short int Size)
{
	long 	File;
	unsigned long Token=10,Ret;
	Ret=sl_FsOpen(Fname,FS_MODE_OPEN_CREATE(Size,_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE),&Token,&File);
	DBG_FLASH_PRINT("Crea archivo %s ok?= %d\r\n",Fname,Ret);
        sl_FsClose(File, 0, 0, 0);
	return Ret;
}
unsigned short int Read_File(unsigned char *Fname,unsigned char *Buf,unsigned short int Size)
{
	long 	File;
	unsigned long Token=10,Ret;
    	if((Ret=sl_FsOpen(Fname,FS_MODE_OPEN_READ,&Token,&File))==0) 
				Ret=sl_FsRead(File,0,Buf,Size);
	DBG_FLASH_PRINT("Lee archivo %s ok?= %d\r\n",Fname,Ret);
  	sl_FsClose(File, 0, 0, 0);
	return Ret;
}
unsigned short int Write_File(unsigned char *Fname,unsigned char *Buf,unsigned short int Size)
{
	long 	File;
	unsigned long Token=10,Ret;
   	if(sl_FsOpen(Fname,FS_MODE_OPEN_WRITE,&Token,&File)==0) 
        	Ret=sl_FsWrite(File,0,Buf,Size);
	DBG_FLASH_PRINT("Graba archivo %s ok?= %d\r\n",Fname,Ret);
    	sl_FsClose(File, 0, 0, 0);
	return Ret;
}
unsigned short int Info_File(unsigned char *Fname,SlFsFileInfo_t* Fileinfo)
{
	unsigned short int Ret;
   	Ret=sl_FsGetInfo(Fname,0,Fileinfo); 
	DBG_FLASH_PRINT("Informacion del archivo %s retorno %d Flags=%d Length=%d Allocated=%d Token=%d\r\n",Fname,Ret,Fileinfo->flags,Fileinfo->FileLen,Fileinfo->AllocatedLen,Fileinfo->Token[0]);
	return Ret;
}
unsigned short int Del_File(unsigned char *Fname)
{
	unsigned short int Ret;
   	Ret=sl_FsDel(Fname,0); 
	DBG_FLASH_PRINT("Archivo %s borrado retorno %d\r\n",Fname,Ret);
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
