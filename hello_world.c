/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
/*******************************************************************************
 * Function Prototypes Section
 ******************************************************************************/

void vfnMemSet ( unsigned char* bpDst, unsigned char bData, unsigned short wSize );
static void vfnLCDWaitState();
static void vfnWelcomeMsgState();
static void vfnMsgDlyState();
static void vfninformacion();
static void vfnTimeUpdate();
static void vfnStopWatch();
static void vfnTimer();


/******************************************************************************/
/*Include files                                                             ***/
/******************************************************************************/
//#include "derivative.h"
#include "stdio.h"
#include "LCDDriver.h"
#include "PDriver.h"
#include "board.h"
#include "stdint.h"
//#include "UART.h"
//#include "MGCDvr.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern GPIO_Type *rGpioB;
extern GPIO_Type *rGpioC;

 static uint8_t bHour = 11;
 static uint8_t bMin = 10;
 static uint8_t bSeg = 0;
 static uint8_t bStop = 1;
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TIME_MESSAGE 10000//0      //200000 // 2000
#define PORTC_MASK 	(0x7F)
#define	PORTB_MASK	(0X0F)

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
typedef enum
{
    STATE_LCD_WAIT,
    STATE_WELCOME_MSG,
    STATE_MSG_DLY_STATE,
    STATE_INFO
}eStates;

static void (* vfnMSG_States[])(void) =
{
	(void (*)(void))vfnLCDWaitState,
	(void (*)(void))vfnWelcomeMsgState,
	(void (*)(void))vfnMsgDlyState,
	(void (*)(void))vfninformacion
};

static unsigned char bCurrentState=STATE_LCD_WAIT;
static unsigned char bPreviousState;
static unsigned char bNextState=STATE_WELCOME_MSG;
static volatile unsigned int wDly = TIME_MESSAGE;
unsigned char bData=0x01;

int main (void)
{

	vfnInicializacion_LEDS();
	vfnLCDInit();
	while(1)
	{
	vfnLCDDriver();
	vfnMSG_States[bCurrentState]();
	}
	return 0;
}
/*******************************************************************************
* vfnLCDWaitState
* DESCRIPTION:
*
* RETURNS:
* void
*******************************************************************************/
static void vfnLCDWaitState()
{
	if (!bfnLCDBusy())
	{
		bCurrentState=bNextState;
	}
}
/*******************************************************************************
* vfnMsgDlyState
* DESCRIPTION:
*
* RETURNS:
* void
*******************************************************************************/
static void vfnMsgDlyState()
{
	if ((wDly--)==0)
	{
		bCurrentState=bPreviousState;
		//count++;
	}
}
static void vfnWelcomeMsgState()
{

	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ',(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"    WELCOME     ",(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[1][0],"    LG & SZ     ");
	vfnLCDUpDate();
	wDly=TIME_MESSAGE;
	bCurrentState=STATE_LCD_WAIT;
	bNextState=STATE_MSG_DLY_STATE;
	bPreviousState=STATE_INFO;
}
/*******************************************************************************
* vfninformacion
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
static void vfninformacion()
{

	vfnMemSet(&gbLCDWelcomeMSG[0][0],' ',(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[0][0],"  THE TIME IS:  ",(sizeof(gbLCDWelcomeMSG)/sizeof(gbLCDWelcomeMSG[0])));
	sprintf(&gbLCDWelcomeMSG[1][0],"    %d:%d:%d    ",bHour,bMin,bSeg);
	vfnTimeUpdate();
//	vfnStopWatch();
//	vfnTimer();
	vfnLCDUpDate();
	wDly=TIME_MESSAGE;
	bCurrentState=STATE_LCD_WAIT;
	bNextState=STATE_MSG_DLY_STATE;
	bPreviousState=STATE_INFO;
	bData++;

}
/*******************************************************************************
* vfnTimeUpdate - CLOCK
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
static void vfnTimeUpdate()
{
	if(bSeg >= 60)
	{
		bSeg = 0;

		if(bMin >= 60)
		{
			bMin = 0;
			if(bHour >= 24)
			{

				bHour = 0;
			}
			else
			{
				bHour++;
			}
		}
		else
		{
			bMin++;
		}
	}
	else
	{
		bSeg++;
	}
}
/*******************************************************************************
* vfnStopWatch
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
static void vfnStopWatch()
{
	if(bStop > 0)
	{
	if(bSeg >= 60)
	{
		bSeg = 0;

		if(bMin >= 60)
		{
			bMin = 0;
			if(bHour >= 24)
			{

				bHour = 0;
			}
			else
			{
				bHour++;
			}
		}
		else
		{
			bMin++;
		}
	}
	else
	{
		bSeg++;
	}
	}
	else
	{
		// Do nothing
	}
}
/*******************************************************************************
* vfnTimer
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
static void vfnTimer()
{
			if(bSeg == 0)
			{
				bSeg = 59;
				bMin--;
				if(bMin == 0)
				{
					bMin = 59;
					bHour--;
				}
			}
			else
			{
				bSeg--;
			}

}
/*******************************************************************************
* vfnMemSet
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
void vfnMemSet ( unsigned char* bpDst, unsigned char bData, unsigned short wSize )
{
	while ( wSize-- )
	{
		*bpDst++ = bData;
	}
}
