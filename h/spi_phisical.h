#ifndef SPI_PHISICAL
#define SPI_PHISICAL

#include "state_machine.h"

enum Spi_Phisical_Event_Code{
			Begin_Event			= 0x9000,
			Time_B_Event			= 0x9001,	
			Time_E_Event			= 0x9002,	
			Time_C_Event			= 0x9003,	
			Time_F_Event			= 0x9004,	
			Off_Event			= 0x9005,
			Clip_On_Event			= 0x9006,
			Clip_Off_Event			= 0x9007,
			Charger_Connected_Event 	= 0x9008,
			Charger_Disconnected_Event 	= 0x9009,
			Power_Enable_Event 		= 0x900A,
			Power_Disable_Event	 	= 0x900B,
			Spi_Phisical_State_D_Event	= 0x900C
};
//------------------------------------------------------
#define SPI_IF_BIT_RATE  10000000
//------------------------------------------------------
#define SPI_LE_A_OFF()   		(HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x20 << 2))) = 0x00)
#define SPI_LE_A_ON()    		(HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x20 << 2))) = 0x20)
#define SPI_LE_B_OFF()   		(HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x10 << 2))) = 0x00)
#define SPI_LE_B_ON()    		(HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x10 << 2))) = 0x10)
#define SPI_CLR_OFF()   		(HWREG(GPIOA2_BASE + (GPIO_O_GPIO_DATA + (0x20 << 2))) = 0x00)
#define SPI_CLR_ON()    		(HWREG(GPIOA2_BASE + (GPIO_O_GPIO_DATA + (0x20 << 2))) = 0x20)
#define TIMERA0_A_INT_DISABLE()    	(HWREG(TIMERA0_BASE + TIMER_O_IMR) &= ~TIMER_TIMA_TIMEOUT) 
#define TIMERA0_A_INT_ENABLE()     	(HWREG(TIMERA0_BASE + TIMER_O_IMR) |=  TIMER_TIMA_TIMEOUT)
#define TIMERA1_A_INT_DISABLE()    	(HWREG(TIMERA1_BASE + TIMER_O_IMR) &= ~TIMER_TIMA_TIMEOUT) 
#define TIMERA1_A_INT_ENABLE()     	(HWREG(TIMERA1_BASE + TIMER_O_IMR) |=  TIMER_TIMA_TIMEOUT)
#define SET_TIMERA0_uSECx10(T)		(HWREG(TIMERA0_BASE + TIMER_O_TAILR) = MICROSECONDSx10_TO_TICKS(T))
#define SET_TIMERA0_uSEC(T)		(HWREG(TIMERA0_BASE + TIMER_O_TAILR) = MICROSECONDS_TO_TICKS(T))
#define SET_TIMERA1_mSEC(T)		(HWREG(TIMERA1_BASE + TIMER_O_TAILR) = MILLISECONDS_TO_TICKS(T))

//------------------------------------------------------
extern void 			Init_Spi_Phisical	(void);
extern const State** 		Spi_Phisical		(void);
extern void 			Spi_Phisical_Rti	(void);
//extern unsigned int 		Read_Spi_Phisical_State	(void);
//------------------------------------------------------
extern void 			TimerA_Handler		(void);
extern void 			TimerB_Handler		(void);
//------------------------------------------------------
extern void 			Spi_Le_A_On		(void);
extern void 			Spi_Le_A_Off		(void);
extern void 			Spi_Le_B_On		(void);
extern void 			Spi_Le_B_Off		(void);
extern void 			Spi_Clr_On		(void);
extern void 			Spi_Clr_Off		(void);
//------------------------------------------------------
extern void 			Turn_Off_Spi_Phisical 	(void);	
extern void 			Turn_On_Spi_Phisical 	(void);	
//------------------------------------------------------
extern void 			Init_Timers		(void);
extern unsigned char 		Is_Running 		(void);
//------------------------------------------------------

#endif
