#include "adc.h"
#include "../driverlib/adc.h"
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
//-------------------------------------------------------------
void 	Init_Adc	(void)	
{
//	MAP_PRCMPeripheralClkEnable(PRCM_GPIOA0,PRCM_RUN_MODE_CLK);

      	MAP_PinTypeADC(PIN_58,PIN_MODE_255); 
	MAP_ADCChannelEnable(ADC_BASE,ADC_CH_1);

      	MAP_PinTypeADC(PIN_59,PIN_MODE_255); 
	MAP_ADCChannelEnable(ADC_BASE,ADC_CH_2);

        MAP_PinTypeADC(PIN_60,PIN_MODE_255); 
	MAP_ADCChannelEnable(ADC_BASE,ADC_CH_3);

	MAP_ADCEnable(ADC_BASE);
}

unsigned short int Read_Adc	(unsigned char Channel) 	
{
	unsigned short int Ans;
	while(MAP_ADCFIFOLvlGet(ADC_BASE,Channel)>=1)  		//con este truco me solo con el ultimo dato leido.. si habia alguno antes lo tiro... eso es porque estoy seguro que leo cada mucho mas que 16useg que es lo que tarda entre una lectura y la otra, y me interesa solo la ultima en este caso..asi que vacio la fifo hasta que queda un solo valor y leeo ese...
		Ans=MAP_ADCFIFORead(ADC_BASE,Channel);		//leo el dato..
	return Ans>>2;
}
//-------------------------------------------------------------

