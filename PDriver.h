/*
 * LEDMG.h
 *
 *  Created on: 23 ene 2022
 *      Author: Godinez
 */

#ifndef PDRIVER_H_
#define PDRIVER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern GPIO_Type *rGpioD;
extern GPIO_Type *rGpioB;
extern GPIO_Type *rGpioC;

//Port B
#define PTB0     ( 0 )
#define	PTB1	 ( 1 )
#define	PTB2	 ( 2 )
#define	PTB3	 ( 3 )
#define COUNT 500U
#define ALL_LEDS 0XFF



/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void vfnInicializacion_LEDS(void);
void vfnTURN_ON(uint8_t pins);
void delay(void);

#endif /* LEDMG_H_ */
