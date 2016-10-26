#include "wdog.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_gpio.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"
#include "gpio_if.h"
#include "timer.h"
#include "timer_if.h"
#include "interrupt.h"
#include "hw_wdt.h"
#include "wdt.h"
#include "wdt_if.h"
#include "uart_if.h"
#include "uart.h"
#include "common.h"

void Wdog_Handler (void)	
{
  	UART_PRINT("pum...\r\n");
}
void Wdog_Clear(void)	
{
	MAP_WatchdogReloadSet		(WDT_BASE,	MILLISECONDS_TO_TICKS(2000));
}
void Init_Wdog(void)	
{
    	MAP_PRCMPeripheralClkEnable	(PRCM_WDT,	PRCM_RUN_MODE_CLK);
    	MAP_WatchdogUnlock		(WDT_BASE);					//unclock para programar
	MAP_IntPrioritySet		(INT_WDT,	INT_PRIORITY_LVL_1);
        MAP_WatchdogIntRegister		(WDT_BASE,	Wdog_Handler);
    	MAP_WatchdogReloadSet		(WDT_BASE,	MILLISECONDS_TO_TICKS(10000)); //estaba en 2000, estoy probando unas cosas, lo pongo en 10000
//	MAP_WatchdogEnable		(WDT_BASE); 					// Start the timer. Once the timer is started, it cannot be disable.
}
