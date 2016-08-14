//archovo dedicado al manejo de strings....
#include <string.h>

//----------------------------------------------------------------------------------------
void String_Copy_Backward(unsigned char* Source,unsigned char* Destiny,unsigned int Length)
{
 while(Length--) Destiny[Length]=Source[Length];
}
void String_Copy_Forward(unsigned char* Source,unsigned char* Destiny,unsigned int Length)
{
 unsigned int i;
 for(i=0;i<Length;i++) Destiny[i]=Source[i];
}
//----------------------------------------------------------------------------------------
unsigned char String_Cmp(unsigned char* A,unsigned char* B,unsigned char Length)
{
 while(Length-- && A[Length]==B[Length]);
 return !++Length;
}

unsigned char Long_Compare(unsigned long* A,unsigned long* B)	{return String_Cmp((unsigned char*)A,(unsigned char*)B,4);}
//---------------------------------------------------------------------------------------------------- 
unsigned int Search_String_On_String(unsigned char* In,unsigned int In_Length,unsigned char *Tag,unsigned int Tag_Length)
{
 unsigned int i;
 for(i=0;i<=(In_Length-Tag_Length);i++) 
  if(In[i]==Tag[0]) 
   if(String_Cmp(In+i,Tag,Tag_Length)) return i;
 return In_Length;
}
unsigned int Search_NString_On_String(unsigned char* In,unsigned int In_Length,unsigned char *Tag,unsigned int Tag_Length,unsigned char N)
{
 unsigned int Pos;
 unsigned char Repetition;
 for(Repetition=Pos=0;Repetition<N && (In_Length-Pos)>Tag_Length;Repetition++)
  Pos+=Search_String_On_String(In+Pos,In_Length-Pos,Tag,Tag_Length)+Tag_Length;
 return Pos;
}

unsigned int Search_NLine_On_String(unsigned char* In,unsigned int In_Length)
{
 unsigned int Pos;
 unsigned char * const Enter="\r\n";
 (Pos=Search_String_On_String(In,In_Length,Enter,2))!=In_Length ||
 (Pos=Search_String_On_String(In,In_Length,Enter  ,1))!=In_Length ||
 (Pos=Search_String_On_String(In,In_Length,Enter+1,1));
 return Pos;
}
unsigned int Search_Null_On_String(unsigned char* In,unsigned int In_Length)
{
 return Search_String_On_String(In,In_Length,"\x00",1);
}
unsigned int Search_Space_On_String(unsigned char* In,unsigned int In_Length)
{
 return Search_String_On_String(In,In_Length," ",1);
}
unsigned int Search_NSpace_On_String(unsigned char* In,unsigned int In_Length,unsigned char N)
{
 return Search_NString_On_String(In,In_Length," ",1,N);
}
//--------------------------------------------------------------------------------
void String_Fill(unsigned char* Source,unsigned char Data,unsigned int Length)
{
 while(Length--) Source[Length]=Data;
}
//----------------------------------------------------------------------------------------
void String2Left(unsigned char* Data,unsigned char Offset,unsigned char Length)
{
 unsigned char i=0;
 for(i=0;i<Length;i++) Data[i]=Data[i+Offset];
}
void Shift_String2Rigth(unsigned char* Source,unsigned int Length, unsigned int Displacement)
{
 while(Length--) Source[Length+Displacement]=Source[Length];
}
//----------------------------------------------------------------------------------------
void Clear_Bit_On_String(unsigned char* Data, unsigned char Bit)
{
 Data[Bit/8]&=~(0x80>>Bit%8);
}
void Set_Bit_On_String(unsigned char* Data, unsigned char Bit)
{
 Data[Bit/8]|=(0x80>>Bit%8);
}
void Clear_Bit_On_String_Array(unsigned char* Data, unsigned char Bit, unsigned char Size, unsigned char Number)
{
 while(Number--) Clear_Bit_On_String(Data+Number*Size,Bit);
}
unsigned char Read_Bit4String(unsigned char* Data,unsigned char Bit)
{
 return (Data[Bit/8]&(0x80>>Bit%8))!=0;
}
//----------------------------------------------------------------------------------------








