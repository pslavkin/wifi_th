#ifndef TYPE_CONVERSION
#define TYPE_CONVERSION

#define Hi(Int)		(*(unsigned char*)&Int)
#define Lo(Int)		(*((unsigned char*)&Int+1))
#define Char(Int)	(*(unsigned char*)&Int+1))
#define Int(Char)	(*(unsigned int*)(Char))
#define MIN(A,B)	(A<B?A:B)

extern unsigned char* 	Long2Bcd				(unsigned char* Bcd,unsigned long Bin);
extern unsigned char* 	Int2Bcd					(unsigned char* Bcd,unsigned short int Bin);
extern unsigned char* 	Char2Bcd				(unsigned char* Bcd,unsigned char Bin);
extern unsigned char* 	Char2Hex_Bcd				(unsigned char* Bcd,unsigned char Bin);
extern unsigned char* 	Char2Bcd_NLine				(unsigned char* Bcd,unsigned char Bin);
extern unsigned char* 	Int2Bcd_NLine				(unsigned char* Bcd,unsigned short int Bin);
extern unsigned char* 	Char2_2Digit_Dec_Bcd			(unsigned char* Bcd,unsigned char Bin);
extern unsigned int 	Dec_Bcd2Int				(unsigned char* Bcd);
extern unsigned int 	X_Digits_Dec_Bcd2Int			(unsigned char Digits,unsigned char* Bcd);
extern unsigned char 	Dec_Bcd2Char				(unsigned char* Bcd);
extern unsigned char* 	Ip_Long2Bcd				(unsigned char* Bcd,unsigned long Ip);
extern unsigned long 	Ip_Bcd2Long				(unsigned char* Ip);
extern unsigned char* 	String2Dec_Bcd				(unsigned char* Bcd,unsigned char* Bin,unsigned int Length);
extern unsigned char* 	String2Hex_Bcd				(unsigned char* Bcd,unsigned char* String, unsigned char Length);
//extern unsigned char* 	Mac_Bin2Hex_Bcd				(unsigned char* Bcd,struct Mac_Address_Struct* Mac);
extern unsigned char* 	Signed_Int2_2Dec_Fix_Point_Bcd		(unsigned char* Bcd,signed int Bin);
extern signed int 	Signed_2Dec_Fix_Point_Bcd2Signed_Int	(unsigned char* Bcd);
//---------------------------------------------------
extern unsigned char* 	Chars2Dot_Separated_Bcd			(unsigned char* Bin,unsigned char Bin_Length,unsigned char* Bcd);
extern unsigned char 	Dot_Separated_Bcd2Chars			(unsigned char* Bcd,unsigned char Bcd_Length,unsigned char *Bin,unsigned char Bin_Length);
extern unsigned char 	Space_Separated_Bcd2Ints		(unsigned char *Bcd,unsigned char Bcd_Length,unsigned int *Bin,unsigned char Bin_Length);
extern unsigned char 	Char2Nibble				(unsigned char Bin);
extern unsigned char* 	Signed_Int2Sign_4Digit_Bcd		(unsigned char* Bcd,signed int Bin);
//---------------------------------------------------


#endif
