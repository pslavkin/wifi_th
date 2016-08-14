#include "stdint.h"
#include "stdbool.h"
#include "rti.h"
#include "everythings.h"
#include "systick.h"
#include "hw_types.h"
#include "hw_nvic.h"
#include "systick.h"
#include "common.h"
#include "timer_if.h"
//---------------------------------------------------
void Func1(void);
void Func2(void);
//---------------------------------------------------
void (*Rti) (void);
//---------------------------------------------------
void Func1(void)
{
 Everythings_Rti();
 Rti=Func2;
}
void Func2(void)
{
 Rti=Func1;
}
//---------------------------------------------------
void Init_Rti(void)
{
 Rti=Func1;			//arranca ejecutando func1...
 SysTickIntRegister(Rti_Handler);
 SysTickPeriodSet(MILLISECONDS_TO_TICKS(5));
 SysTickEnable();
 SysTickIntEnable();
}

void Rti_Handler 	(void)		
{
 Rti();
}
//---------------------------------------------------

