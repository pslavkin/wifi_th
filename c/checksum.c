#include <checksum.h>

unsigned int One_Complement_Checksum(unsigned int* Data,unsigned int Length)
{
 unsigned int Ans=0,Aux;
 if(Length%2) Ans|=Data[Length/2]&0xFF00;	//excelecnte! Si la longitud a computar es impar completa con ceros, como dice el manual
 Length/=2;			//como recorre de a enteros seran la mitad de las sumas...es por eso que ademas el paquete tiene que tener longitud par...
 while(Length--) 		//hasta que se terminen los datos..
  {
   Aux=Data[Length];		//como luego se consultara este valor se lo guard an eun auxilia...
   Ans+=Aux;			//se suma al resultado el valor del dato actual	
   if(Ans<Aux) Ans++;		//si se pasa del tamanio de un entero...agrega uno...ese es el chiste del complemento a uno...
  }
 return ~Ans;			//antes de mandar se invierten todos los bits....
}
    
//---------------------------------------------------
unsigned char Update_crc(unsigned char New, unsigned char Last)
{
//8-bit CRC value  using polynomial  X^8 + X^5 + X^4 + 1 
 #define POLYVAL 0x8C
 unsigned char i;
 for(i=0;i<8;i++) 
 {
  Last=((Last^New)&0x01)?(Last>>1)^POLYVAL:Last>>1;
  New>>=1;
 }
 return Last;
}
//---------------------------------------------------
unsigned char One_Wire_Crc(unsigned char* Data,unsigned char Length)
{
 unsigned char Crc=0;
 while(Length--) Crc=Update_crc(Data[Length],Crc);
 return Crc==0;
}
//---------------------------------------------------





