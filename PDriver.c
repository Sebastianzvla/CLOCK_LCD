/*
 * LEDMG.c
 */

/*******************************************************************************
 * vfnLEDsInit
 * DESCRIPTION: Pin leds Initialization
 *
 * RETURNS:
 * void
 ******************************************************************************/
#include "PDriver.h"
#include "LCDDriver.h"
/*******************************************************************************
 * Variables global
 ******************************************************************************/
GPIO_Type *rGpioB = GPIOB;			/* Peripheral GPIOB base address */
GPIO_Type *rGpioC = GPIOC;			/* Peripheral GPIOB base address */

/*******************************************************************************
 * vfnInicializacion_LEDS
 * DESCRIPTION: Pin leds Initialization
 *
 * RETURNS:
 * void
 ******************************************************************************/
void vfnInicializacion_LEDS()
{
	SIM_Type *rclkSIM_gpio= SIM;		/* Peripheral SIM base pointer */
	/*Enable Clks*/
	rclkSIM_gpio->SCGC5 |= SIM_SCGC5_PORTC_MASK |SIM_SCGC5_PORTB_MASK;

	//PORT B Input (Switch And Button)
	PORT_Type *rPortB = PORTB;			/* Peripheral PORTB base pointer */

	rPortB->PCR[PTB0] = PORT_PCR_MUX(1);
	rPortB->PCR[PTB1] = PORT_PCR_MUX(1);
	rPortB->PCR[PTB2] = PORT_PCR_MUX(1);
	rPortB->PCR[PTB3] = PORT_PCR_MUX(1);

	rGpioB->PDDR |= (1u << PTB0);
	rGpioB->PDDR |= (1u << PTB1);
	rGpioB->PDDR |= (1u << PTB2);	//ALL_LEDS
	rGpioB->PDDR |= (1u << PTB3);

	rGpioB->PCOR |= ALL_LEDS;

	//PORT C
	PORT_Type *rPortC = PORTC;			/* Peripheral PORTB base pointer */
	rPortC->PCR[RS] = PORT_PCR_MUX(1);	/*Alternative pin MUX*//*PIN CONTROL REGISTER*/
	rPortC->PCR[ENABLE] = PORT_PCR_MUX(1);	/*Alternative pin MUX*//*PIN CONTROL REGISTER*/

	rGpioC->PDDR |=1u << RS;			/*Port Data Direction Register*/
	rGpioC->PDDR |=1u << ENABLE;

	rGpioC->PCOR |=ALL_LEDS;
}
void vfnTURN_ON(uint8_t pins)
	{
	rGpioB->PCOR |= pins;//ON
	rGpioB->PSOR |= 	~(pins);//OFF ~pins
	}

