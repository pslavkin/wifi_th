#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "common.h"
#include "state_machine.h"
#include "events.h"
#include <one_wire_phisical.h>
#include <type_conversion.h>
#include <wifi_socket.h>

//esto deberia ir en flash...pero aun no se como...
unsigned int One_Wire_Read_Delay	=4;	//es el parametro critico que depende del cable... nominalmente 5 microsegs...
//------------------------------------------------------------------
void Inc_One_Wire_Read_Delay			(void)		{1;}//if(One_Wire_Read_Delay<50) Inc_Flash_Int(&One_Wire_Read_Delay);}
void Dec_One_Wire_Read_Delay			(void)		{1;}//if(One_Wire_Read_Delay>0)  Dec_Flash_Int(&One_Wire_Read_Delay);}
void Send_One_Wire_Read_Delay2Socket		(void)		{Send_Int_NLine2Socket(One_Wire_Read_Delay);}
//------------------------------------------------------------------
void Init_One_Wire_Phisical(void)
{
	//Pin de 1wire interno y externo pero no a traves del sn176, directo la gpio
    MAP_PRCMPeripheralClkEnable	(PRCM_GPIOA1, 	PRCM_RUN_MODE_CLK);
    MAP_PinTypeGPIO		(PIN_03,	PIN_MODE_0, 	true);			//setea pin modo 0 (gpio) open drain y 2mA
    MAP_GPIODirModeSet		(GPIOA1_BASE, 	0x10, 		GPIO_DIR_MODE_OUT);
}
//------------------------------------------------------------------
void One_Wire_Power_On_Reset(void)
{
  ONE_WIRE_BUS_LO();			//chau alimentacion... OJO, que alguien se encargue de esperar....
}
unsigned char Presence(void)
{
 unsigned char Ans;
 ONE_WIRE_BUS_HIZ();			//y si esta en corto... mejor lo suelto y me fijo..
 Delay_Useg(1);				//aguanta que se estabilice todo.
 if(ONE_WIRE_BUS_READ()==0) 
  {
   ONE_WIRE_BUS_LO();			//hasta dentro de 1 segundo que vuelve por aca, queda el bus en cero, que equivale a apagar los sensores...
   return 1;				//liberen el barco!! porque asi no laburo... todo '0' es un CRC valido, asi que si esta en corto, cuenta como nodo valido!!!!
  }
 ONE_WIRE_BUS_LO();					//baja
 Delay_Useg(510);					//480<T<960 espera el tiempo de reset
 STRONG_PULLUP();					//sube el bus con fuerza, espera y luego lo libera...

// _asm("SEI");
 Delay_Useg(70);					//minimo 75, maximo 300 
 Ans=ONE_WIRE_BUS_READ();				//lobo esta?
// _asm("CLI");

 Delay_Useg(430);					//480-70 minimo, completa el tiempo de slot...
 STRONG_PULLUP();					//despues de un presense, solo lo deja en pullup, porque si no hay nadie ara que tenerlo en alta que podria haber un corto...
 return Ans;
}
//------------------------------------------------------------------
unsigned char Write_Bit_Zero(void)
{
// _asm("SEI");
 ONE_WIRE_BUS_LO();					//baja
 Delay_Useg(70);					//60<T<120
 STRONG_PULLUP();					//sube el bus con fuerza, espera y luego lo libera...
// _asm("CLI");

 Delay_Useg(2);						//1u<T<inf.
 ONE_WIRE_BUS_HI();					//arriba
 return 0;
}
//------------------------------------------------------------------
unsigned char Write_Bit_One_And_Read(void)
{
 unsigned char Ans;

// _asm("SEI");
 ONE_WIRE_BUS_LO();					//baja
 Delay_Useg(3);						//1<T<15	espera 6useg (offset 2+3)
 STRONG_PULLUP();					//sube el bus con fuerza, espera y luego lo libera...
 Delay_Useg(One_Wire_Read_Delay);	 					//1u<T<15 del total, es 2+3+4+3=12 + lo que tarda el Strong_Pullup	espera 8useg (offset 3+5)
 Ans=ONE_WIRE_BUS_READ();				//samplea a los 14useg (Texas obliga a 15 como maximo)
 Delay_Useg(55);					//45 como minimo.. despues de los 15 de lectura...+ 1us de recupero
// _asm("CLI");

 ONE_WIRE_BUS_HI();					//arriba
 return Ans;
}
//------------------------------------------------------------------
unsigned char Write_Read_Bit	(unsigned char Bit)	{return Bit?Write_Bit_One_And_Read():Write_Bit_Zero();}
//------------------------------------------------------------------
unsigned char Read2Bits		(void)			{return (Write_Read_Bit(1)<<1) + Write_Read_Bit(1);}	//hace 2 lecturas, la primera en el bit 0 y la segunda la corre una posicion.. codigos de salida 00-01-10-11
//------------------------------------------------------------------
unsigned char Write_Read_Byte	(unsigned char Data)
{
 unsigned char i,Ans=0;
 for(i=0x01;i;i<<=1) if(Write_Read_Bit(Data&i)) Ans|=i;
 return Ans;
}
//------------------------------------------------------------------
