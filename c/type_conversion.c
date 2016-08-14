#include <type_conversion.h>
#include <string.h>


static const unsigned int Dec_Long[]= {		//este array de long se usa para las restas suscesivas en las funciones de conversion de long a BCD y de char a BCD. Notar que no banca el maximo de los long, porque por ahora no se necesita, en caso de requerir habra que seguir agragando potencias de 10 a este array
 		            	   1000000000,  //hasta aca para longs
 		            	   100000000,
 		            	   10000000,
 		            	   1000000,
 		            	   100000
				   };
static const unsigned short int Dec[]= {		//este array de long se usa para las restas suscesivas en las funciones de conversion de long a BCD y de char a BCD. Notar que no banca el maximo de los long, porque por ahora no se necesita, en caso de requerir habra que seguir agragando potencias de 10 a este array

 		            	   10000,	//hasta aca para integers	
 	        	    	   1000,
 		        	   100,
	 		           10,
			    	   1
			   	   };

static const unsigned short int Hex[]= {		//este array de long se usa para las restas suscesivas en las funciones de conversion de long a BCD y de char a BCD. Notar que no banca el maximo de los long, porque por ahora no se necesita, en caso de requerir habra que seguir agragando potencias de 10 a este array///
 		            	   0x1000,	//si bien esto es un desperdicion de flash (guardar un uno en 
 	        	    	   0x0100,
 		        	   0x0010,
	 		           0x0001,
			   	   };
//---------------------------------------------------
unsigned char* Long2Bcd(unsigned char* Bcd,unsigned long Bin)	//convierte un Long (OJO menor a 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
	unsigned char i;													//contador auxiliar...
	unsigned long Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
	for (i=0;i<10;i++)	
		for(Bcd[i]='0';Aux=-Dec_Long[i],(Aux+=Bin)<Bin;Bcd[i]++)		//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
	   		Bin=Aux;												//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
	return Bcd;
}
//---------------------------------------------------
unsigned char* Int2Bcd(unsigned char* Bcd,unsigned short int Bin)	//convierte un Long (OJO menor a 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
	unsigned char i;												//contador auxiliar...
	unsigned short int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
	for (i=0;i<5;i++,Bin/=10) 
	       Bcd[4-i]=Bin%10+'0';
	return Bcd;
}
unsigned char* Int2Bcd_NLine(unsigned char* Bcd,unsigned short int Bin)	
{
	Int2Bcd(Bcd,Bin);
	Bcd[5]='\r';
	Bcd[6]='\n';
	return Bcd;
}
//---------------------------------------------------
unsigned char* Char2Bcd(unsigned char* Bcd, unsigned char Bin)						//convierte un Char a BCD...
{
	unsigned char Aux,i;										//un auxiliar para recorrer y otro para ir tomando nota de las restas...
	for(i=0;i<3;i++)										//se recorre 3 veces porque banca hasta 255 que ocupa 3 bytes...
		for(Bcd[i]='0';Aux=-Dec[i+2],(Aux+=Bin)<Bin;Bcd[i]++)	//se inicializa la posicion a grabar el BCD en cuestion; se resta bin con la parte de la tabla que corresponde y se guarda en aux; se incrementa el BCD en cuestion y...
	    		Bin=Aux; 											//como no se paso bin pasa a ser aux...
	return Bcd; 
}
unsigned char* Char2Bcd_NLine(unsigned char* Bcd, unsigned char Bin)
{
	Char2Bcd(Bcd,Bin);
	Bcd[3]='\r';
	Bcd[4]='\n';
	return Bcd;
}
//---------------------------------------------------
unsigned char Dec_Bcd2Char(unsigned char* Bcd)
{
	unsigned char i;
	unsigned char Aux=0;						//contador auxiliar...
	for (i=0;i<3;i++)
		Aux+=(Bcd[i]-'0')*Dec[i+2];
	return Aux;
}
//---------------------------------------------------
unsigned int Dec_Bcd2Int(unsigned char* Bcd)
{
 unsigned char i;
 unsigned short int Aux=0;				//contador auxiliar...
 if((i=Search_NLine_On_String(Bcd,5))!=5)	//busca un enter para convertir numeros que no estan rellenos con ceros al inicio....
  {
   String_Copy(Bcd,Bcd+5-i,i);			//copya el dato corrido un poco...
   String_Fill(Bcd,'0',5-i);			//rellena con ceros...
  }
 for (i=0;i<5;i++) Aux+=(Bcd[i]-'0')*Dec[i];
 return Aux;
}
unsigned int X_Digits_Dec_Bcd2Int(unsigned char Digits,unsigned char* Bcd)
{
 unsigned char Buf[5];
 String_Fill(Buf,'0',5-Digits);
 String_Copy(Bcd,Buf+5-Digits,Digits);
 return Dec_Bcd2Int(Buf);
}
//---------------------------------------------------
unsigned char* Ip_Long2Bcd(unsigned char* Bcd, unsigned long Ip)
{
 unsigned char i;
 for(i=0;i<4;i++)
  { 
   Char2Bcd(Bcd+i*4,*((unsigned char*)&Ip+i)); 
   if(i<3) Bcd[i*4+3]='.';
  }
return Bcd;
}
//---------------------------------------------------
unsigned long Ip_Bcd2Long(unsigned char* Ip)	//192.168.001.001
{
 unsigned char i,Bin[4];
 Dot_Separated_Bcd2Chars(Ip,15,Bin,4);
 return *(unsigned long*)Bin;
}
//---------------------------------------------------
unsigned char* String2Dec_Bcd(unsigned char* Bcd,unsigned char* Bin,unsigned int Length)
{
 unsigned char i;
 for(i=0;i<Length;i++)
   Char2Bcd(Bcd+i*3,Bin[i]); 
 return Bcd;
}
//---------------------------------------------------
unsigned char* Chars2Dot_Separated_Bcd(unsigned char* Bin,unsigned char Bin_Length,unsigned char* Bcd)
{
 unsigned char i;
 for(i=0;i<Bin_Length;i++)
  { 
   Char2Bcd(Bcd+i*4,Bin[i]); 
   Bcd[i*4+3]='.';
  }
return Bcd;
}

unsigned char Dot_Separated_Bcd2Chars(unsigned char *Bcd,unsigned char Bcd_Length,unsigned char *Bin,unsigned char Bin_Length)
{
 unsigned char Buf[6],Buf_Pos=3,i,Bin_Pos=0;
 Bcd_Length=Search_NLine_On_String(Bcd,Bcd_Length);
 String_Fill(Buf,'0',3);
 for(i=0;i<Bcd_Length && Bin_Pos<Bin_Length;i++)
  if(Bcd[i]!='.' && Bcd[i]!=' ' && Buf_Pos<6) 	Buf[Buf_Pos++]=Bcd[i];
   else						Bin[Bin_Pos++]=Dec_Bcd2Char(Buf+Buf_Pos-3),Buf_Pos=3;
 if(Bin_Pos<Bin_Length) Bin[Bin_Pos++]=Dec_Bcd2Char(Buf+Buf_Pos-3);
 return Bin_Pos;   
}
//---------------------------------------------------
unsigned char Space_Separated_Bcd2Ints(unsigned char *Bcd,unsigned char Bcd_Length,unsigned int *Bin,unsigned char Bin_Length)
{
 unsigned char Buf[5+5],Buf_Pos=5,i,Bin_Pos=0;
 Bcd_Length=Search_NLine_On_String(Bcd,Bcd_Length);
 String_Fill(Buf,'0',5);
 for(i=0;i<Bcd_Length && Bin_Pos<Bin_Length;i++)
  if(Bcd[i]!=' ' && Buf_Pos<10)		 	Buf[Buf_Pos++]=Bcd[i];
   else						Bin[Bin_Pos++]=Dec_Bcd2Int(Buf+Buf_Pos-5),Buf_Pos=5;

 if(Bin_Pos<Bin_Length) Bin[Bin_Pos++]=Dec_Bcd2Int(Buf+Buf_Pos-5);
 return Bin_Pos;   
}
//---------------------------------------------------
unsigned char* Int2Hex_Bcd(unsigned char* Bcd,unsigned int Bin)	//convierte un Long (OJO menor a 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
 unsigned char i;													//contador auxiliar...
 unsigned int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
 for (i=0;i<4;i++)	
  for(Bcd[i]=0;Aux=-Hex[i],(Aux+=Bin)<Bin;Bcd[i]++)		//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
    Bin=Aux;												//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
 for(i=0;i<4;i++) Bcd[i]+=(Bcd[i]>9)?('A'-10):'0';
 return Bcd;
}
//---------------------------------------------------
unsigned char Char2Nibble(unsigned char Bin)	//convierte un char a nibble, por ejemplo, 0x0C sale con 0x12
{
 return Bin%10|(Bin/10)<<4;
}
//-------------------------------------------------------------
unsigned char* Char2Hex_Bcd(unsigned char* Bcd,unsigned char Bin)	//convierte un Long (OJO menor a 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
 unsigned char i	;													//contador auxiliar...
 unsigned int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
 for (i=0;i<2;i++)	
  for(Bcd[i]=0;Aux=-Hex[i+2],(Aux+=Bin)<Bin;Bcd[i]++)		//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
    Bin=Aux;												//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
 for(i=0;i<2;i++) Bcd[i]+=(Bcd[i]>9)?('A'-10):'0';
 return Bcd;
}
//---------------------------------------------------
unsigned char* String2Hex_Bcd(unsigned char* Bcd,unsigned char* String,unsigned char Length)
{
 while(Length--) Char2Hex_Bcd(Bcd+2*Length,String[Length]);
 return Bcd;
}
//---------------------------------------------------
/*
 * unsigned char* Mac_Bin2Hex_Bcd(unsigned char* Bcd, struct Mac_Address_Struct* Mac)
{
 unsigned char i;
 for(i=0;i<6;i++)
  { 
   Char2Hex_Bcd(Bcd+i*3,Mac->Mac[i]); 
   Bcd[i*3+2]=':';
  }
 Bcd[17]='\n';
 return Bcd;
}
*/
//---------------------------------------------------
unsigned char* Signed_Int2_2Dec_Fix_Point_Bcd(unsigned char* Bcd,signed int Bin)	//convierte un entero pero lo considera como punto fijo y lo pasa a bcd considerando 2 decimales mas signo, del tipo "+655.35"
{
 String_Copy((unsigned char*)"+655.35",Bcd,7);			//copia el formato en el destino
 if(Bin<0) {Bcd[0]='-';Bin=-Bin;}		//si el valore es negativo, se cambia el signo y se complementa el daro para poder convertir a bcd
 Int2Bcd(Bcd+1,Bin);				//convioerte a bcd y pisa la respuesta salteando la primera posicion reservada para el signo...
 Shift_String2Rigth(Bcd+4,2,1);			//hace lugar para poner el punto decimal..
 Bcd[4]='.';					//agrega el puntito...
 return Bcd;
}
signed int Signed_2Dec_Fix_Point_Bcd2Signed_Int(unsigned char* Bcd)	//convierte un  lo pasa a bcd considerando 2 decimales mas signo, del tipo "+655.35" en un entero signado pero que se interpreta como de punto fijo con 2 decimales...Por ejemplo +002.35 se convierte en 235.
{
 signed int Bin;
 Bcd[4]=Bcd[5];
 Bcd[5]=Bcd[6];					//elimiina el '.' pisandolo con los decimales.
 Bin=Dec_Bcd2Int(Bcd+1);
 return Bcd[0]=='+'?Bin:-Bin;
}
//---------------------------------------------------
unsigned char* Signed_Int2Sign_4Digit_Bcd(unsigned char* Bcd,signed int Bin)	//-300 ---> "-0300"
{
 unsigned char Sign='+';
 String_Copy((unsigned char*)"+0300",Bcd,5);			//copia el formato en el destino
 if(Bin<0) {Bin=-Bin;Sign='-';};			//si es negativo...
 Int2Bcd(Bcd,Bin);				//convioerte a bcd y pisa la respuesta salteando la primera posicion reservada para el signo...
 Bcd[0]=Sign;					//agrega el signo..
 return Bcd;
}
//---------------------------------------------------
unsigned char* Char2_2Digit_Dec_Bcd(unsigned char* Bcd, unsigned char Bin)
{
 unsigned char Buf[3];
 Char2Bcd(Buf,Bin);
 Bcd[0]=Buf[1];
 Bcd[1]=Buf[2];
 return Bcd; 
}
//---------------------------------------------------
