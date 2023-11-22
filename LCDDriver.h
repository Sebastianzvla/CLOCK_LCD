/*
 * LCDDriver.h
 *
 *  Created on:14-Feb-2022
 *      Author: Mario Castañeda
 */
/******************************************************************************/
/*Include files                                                             ***/
/******************************************************************************/
//#include "derivative.h"
#include "stdio.h"
#include "stdint.h"
/*******************************************************************************
* Definitions
******************************************************************************/
#define RS (1)
#define R_W (0x20)
#define ENABLE (2)
#define	DELAY (0X2F)
#define   LCD_INIT_CMD0 ( 0x33 )
#define   LCD_INIT_CMD1 ( 0x32 )
#define   LCD_INIT_CMD2 ( 0x28 )
#define   LCD_INIT_CMD3 ( 0x06 )
#define   LCD_INIT_CMD4 ( 0x0c )
#define   LCD_INIT_CMD5 ( 0x0f )
//#define   LCD_INIT_CMD6 ( 0x01 )
#define   LCD_Y (2)
#define   LCD_X (16)
/*******************************************************************************
* Local Function Prototypes
******************************************************************************/
void vfnLCDDriver();
void vfnLCDUpDate ();
void vfnLCDInit();
void vfnDelay(void);
unsigned char bfnLCDBusy();
void vfnLCDGotoxy ();
void vfnLCDGotoxy_Home ();
/*******************************************************************************
* Local Variables
******************************************************************************/
// static U08 baInitCommands[] = {0x01, 0x33, 0x32, 0x28, 0x06, 0x0F, 0x01};
extern unsigned char gbLCDWelcomeMSG[2][16];
//extern uint8_t bExeFlag = 0;

