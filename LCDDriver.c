/*
* LCDDriver.c
*
*  Created on: 14-Feb-2022
*      Author: Mario Castañeda
*/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "LCDDriver.h"
#include  "PDriver.h"
/*******************************************************************************
 * Local Variables
 ******************************************************************************/
unsigned char gbLCDWelcomeMSG[2][16];

typedef enum
{
    STATEMSN,
    STATEDOWN,
    DELAY_STATE,
	STATELSN,
    VALIDATION_STATE,
    STATEIDLE  
}InitialStates; 

static unsigned char baLCDConfig[]=
{
    LCD_INIT_CMD0,  //initial 3 steps
    LCD_INIT_CMD1,  //4bits
    LCD_INIT_CMD2,  //specify lines and fonts
    LCD_INIT_CMD3,  //display off
    LCD_INIT_CMD4  //display clear
   // LCD_INIT_CMD5   // mode set
    //LCD_INIT_CMD6
}; 
static unsigned char balJump[]=
{
        0xC0        //Jump x and y
};
static unsigned char bCursor_Home[]=
{
        0x03        //Cursor home
};

unsigned char Data_Out=0x00;
unsigned char bCursor=0x00;
volatile unsigned char bCnt=DELAY;
static unsigned char bDataCnt=0x00;
static unsigned char *bpLCDData=0x00;
static unsigned char xbword=0x00;
static unsigned char brow=0x00;
static unsigned char bRegister_Select=0x00;
static unsigned char bCurrentState=STATEIDLE;    
static unsigned char bNextState = STATEMSN;
static unsigned char bPreviousState;
/*******************************************************************************
* Local Function Prototypes
******************************************************************************/
void vfnLCDDriver();
void vfnDly500nsState();
void vfnState0MSN();
void vfnState1EDown();
void vfnState2LSN();
void vfnStateExecution();
void vfnState_Idle();

void (* vfnDriverLCD[])(void) =
{
    (void (*)(void))vfnState0MSN,
    (void (*)(void))vfnState1EDown,
    (void (*)(void))vfnDly500nsState,
	(void (*)(void))vfnState2LSN,
    (void (*)(void))vfnStateExecution,
    (void (*)(void))vfnState_Idle
};
/*******************************************************************************
* vfnLCDInit
* DESCRIPTION: Set the first config parameters for LCD init 4 bits
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDInit()
{

	if(bCurrentState==STATEIDLE)
    { 
    bDataCnt = sizeof(baLCDConfig);
    bpLCDData = &baLCDConfig[xbword];
    bCurrentState = STATEMSN;
    } 
}
/*******************************************************************************
* vfnLCDDriver
* DESCRIPTION: Array with the funtion pointers
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDDriver()
{

    vfnDriverLCD[bCurrentState]();

}
/*******************************************************************************
* bfnLCDBusy
* DESCRIPTION: Verify if the State machine is busy
*
* RETURNS:
* void
******************************************************************************/
unsigned char bfnLCDBusy()
{
    if (bCurrentState == STATEIDLE)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}
/*******************************************************************************
* vfn
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDUpDate ()
{
    if(bCurrentState==STATEIDLE)
    {
    	bRegister_Select = 1;
    	bDataCnt = LCD_X;
    	//bpLCDData = &gbLCDWelcomeMSG[brow][xbword];
    	bCurrentState = STATEMSN;

    }
}
/*******************************************************************************
* vfnLCDGotoxy
* DESCRIPTION: Select the Line X or Y
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDGotoxy (void)
{
	bDataCnt = (sizeof(balJump)/sizeof(balJump[0]));
	bpLCDData = &balJump[xbword];
}
/*******************************************************************************
* vfnLCDGotoxy_Home
* DESCRIPTION: Go to the first position
*
* RETURNS:
* void
******************************************************************************/
void vfnLCDGotoxy_Home()
{
	bDataCnt = (sizeof(bCursor_Home)/sizeof(bCursor_Home[0]));
		bpLCDData = &bCursor_Home[xbword];

}
/*******************************************************************************
* vfn
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/***********************STATE Machine******************************************/
////////////////////////////////////////////////////////////////////////////////
void vfnDly500nsState(void) 
{
	if((bCnt--)==0)
	{
		bCnt= DELAY;
		bCurrentState =  bNextState;
	}
}
/*******************************************************************************
* vfnState0MSN
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
void vfnState0MSN()
{
		if(bRegister_Select)
		{
			if(bCursor)
			{
				if(brow == 1 && bCursor == 1)
				{
					vfnLCDGotoxy_Home();
				}
				else
				{
					vfnLCDGotoxy();
				}

			}
			else
			{
			rGpioC->PSOR |= (1u << RS);
			bpLCDData = &gbLCDWelcomeMSG[brow][xbword];
			}

		}
		else
		{
			 bpLCDData = &baLCDConfig[xbword];
		}


	rGpioB->PDOR &= ~(0xff);
	rGpioB->PDOR |= (*bpLCDData >> 4);
	rGpioC->PSOR |= (1u << ENABLE);
	 bCurrentState = DELAY_STATE;
	 bNextState = STATEDOWN;
}
/*******************************************************************************
* vfnState1EDown
* DESCRIPTION: Write 0 to the Enable pin
*
* RETURNS:
* void
******************************************************************************/
void vfnState1EDown(void)
{ 
	static uint8_t bSum = 2;
	rGpioC->PCOR |= (1u << ENABLE);
    
    bCurrentState = DELAY_STATE; 
    bNextState = STATELSN;

    bSum--;
    if(!bSum)
    {
    	bNextState = VALIDATION_STATE;
    	bSum = 2;
    }

}
/*******************************************************************************
* vfnState2LSN Least Significant  
* DESCRIPTION:
*
* RETURNS:
* void
******************************************************************************/
void vfnState2LSN()
{
	rGpioB->PDOR &= ~(0xff);
	rGpioB->PDOR |= (*bpLCDData);
	rGpioC->PSOR |= (1u << ENABLE);
	bCurrentState = DELAY_STATE;
	bNextState = STATEDOWN;
	xbword++;
}
/*******************************************************************************
* vfnStateExecution
* DESCRIPTION: Routine for the Execution Rouitne RS 
*
* RETURNS:
* void
******************************************************************************/
void vfnStateExecution()
{
	static uint8_t bcnt = 0;

	if(bcnt >= bDataCnt)
	{
		bCurrentState = STATEIDLE;
		xbword = 0x00;
		bcnt = 0;


		if(bRegister_Select)
		{
			if(brow)
			{
				if(brow == 1 && bCursor == 1)
				{
					brow = 0;
					bCursor = 0;
					bCurrentState = STATEIDLE;
				}
				else
				{
				bCursor = 1;
				rGpioC->PCOR |= (1u << RS);
				bCurrentState = DELAY_STATE;
				bNextState = STATEMSN;
				}

			}

			else if(bCursor)
			{
				bCursor = 0;
				brow = 0x01;
				bDataCnt = LCD_X;
				bCurrentState = STATEMSN;
			}
			else
			{
				bCursor = 1;
				rGpioC->PCOR |= (1u << RS);
				bCurrentState = DELAY_STATE;
				bNextState = STATEMSN;
			}

		}


		}
	else
	{

		bCurrentState = DELAY_STATE;
		bNextState = STATEMSN;

	}
	bcnt++;


}
/*******************************************************************************
* vfnState_Idle
* DESCRIPTION: Funtion for IDLE State Machine
*
* RETURNS:
* void
******************************************************************************/
void vfnState_Idle()
{
    //Nothing to do
}
