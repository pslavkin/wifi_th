#include "hibernate.h"
#include "../driverlib/adc.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "utils.h"
#include "utils_if.h"
#include "hw_ints.h"
#include "hw_gpio.h"
#include "gpio.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "uart_if.h"
#include "common.h"
#include "debug.h"
#include "leds_session.h"
//-------------------------------------------------------------
void Hibernate_Now	(void) 	
{
	//selecciono el pin 11 como generador de wekup EXTERNO!! porque cuando hiberna es como una piedra.. no tiene ni pullups...
	PRCMHibernateWakeUpGPIOSelect(PRCM_HIB_GPIO11,PRCM_HIB_FALL_EDGE);
	PRCMHibernateWakeupSourceEnable(PRCM_HIB_GPIO11);

	DBG_HIBERNATE_PRINT("HIB: Entering HIBernate...\n\r");
	MAP_UtilsDelay(80000);									//espero un poco para que se estabilice el pullup y no salga antes de empezar

	Utils_SpiFlashDeepPowerDown();
	MAP_PRCMHibernateEnter();
}
//-------------------------------------------------------------

