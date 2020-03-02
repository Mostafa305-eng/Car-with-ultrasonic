/*
 * ExternalInterrupt.c
 *
 * Created: 12/23/2019 10:11:49 PM
 *  Author: Mostafa Metwaly
 */ 

/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/


#include "ExternalInterrupt.h"
#include "interrupt.h"
#include "registers.h"
#include "std_types.h"
#include "common_macros.h"



/************************************************************************/
/*								 DEFINES						        */
/************************************************************************/


#define NO_OF_INT 3

#define Int0_EdgeLowLevel	0x00
#define Int0_EdgeChange		0x01
#define Int0_EdgeFalling	0x02
#define Int0_EdgeRisiging	0x03
#define Int0_Mask_EdgeBits	0x03

#define Int1_EdgeLowLevel  0x00
#define Int1_EdgeChange		0x04
#define Int1_EdgeFalling	0x08
#define Int1_EdgeRisiging	0x0C
#define Int1_Mask_EdgeBits	0x0C

#define Int2_EdgeFalling	0x00
#define Int2_EdgeRisiging	0x40
#define Int2_Mask_EdgeBits	0x40

#define Int0_Mask				0x40
#define Int1_Mask				0x80
#define Int2_Mask				0x20



/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/



static void(*INT_CBK_PTR[NO_OF_INT])(void) = {NULL,NULL,NULL};




/************************************************************************/
/*				 		Functions ProtoTypes                            */
/************************************************************************/

/*
 * Function: ExternInt_Init 
 * Inputs:	ExternIntCfg -> pointer of type ExternInt_Cfg_s which points to structure that contain the initialized data.
 * Outputs: 
 * In Out: 
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Initiate the required external interrupt configuration as it specify
 *	event of interrupt.
 *	Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set
 */
ERROR_STATUS ExternInt_Init(ExternInt_Cfg_s *ExternIntCfg)
{
	switch(ExternIntCfg->ExternInt_No)
	{
		/*set the edge event*/
		case (EXTRN_INT_0) :     
			switch (ExternIntCfg->ExternInt_Event)
			{
			case LOW_LEVEL:
				MCUCR |= Int0_EdgeLowLevel ;
				break;
			case LOGICAL_CHANGE :
				MCUCR |= Int0_EdgeChange ;
				break;
			case FALLING_EDGE :
				MCUCR |= Int0_EdgeFalling ;
				break;
			case RISING_EDGE:
				MCUCR |= Int0_EdgeRisiging ;
				break;
			default:
				return E_NOK ;
			}
			/* enable the external interrupt channel*/
			GICR |= Int0_Mask; 
			/*store the call back function */
			if ( NULL !=  ExternIntCfg->ExternInt_CBF_Ptr)
			{
				INT_CBK_PTR[EXTRN_INT_0]=ExternIntCfg->ExternInt_CBF_Ptr;
			}
			break;
		case (EXTRN_INT_1) :
			/*set the edge event*/
			switch (ExternIntCfg->ExternInt_Event)
			{
			case LOW_LEVEL:
				MCUCR |= Int1_EdgeLowLevel ;
				break;
			case LOGICAL_CHANGE :
				MCUCR |= Int1_EdgeChange ;
				break;
			case FALLING_EDGE :
				MCUCR |= Int1_EdgeFalling ;
				break;
			case RISING_EDGE:
				MCUCR |= Int1_EdgeRisiging ;
				break;
			default:
				return E_NOK;
			}
			/* enable the external interrupt channel*/
			GICR |= Int1_Mask;
			/*store the call back function */
			if ( NULL !=  ExternIntCfg->ExternInt_CBF_Ptr)
			{
				INT_CBK_PTR[EXTRN_INT_1]=ExternIntCfg->ExternInt_CBF_Ptr;
			}
			break;
		case (EXTRN_INT_2) :
			switch(ExternIntCfg->ExternInt_Event)
			{
			case FALLING_EDGE :
				MCUCSR |= Int2_EdgeFalling ;
				break;
			case RISING_EDGE:
				MCUCSR |= Int2_EdgeRisiging ;
				break;
			default:
				return E_NOK; 
			}
			GICR |= Int2_Mask;
			if ( NULL !=  ExternIntCfg->ExternInt_CBF_Ptr)
			{
				INT_CBK_PTR[EXTRN_INT_2]=ExternIntCfg->ExternInt_CBF_Ptr;
			}			
			break;
		default:
			return E_NOK;		
	}
	return E_OK;
}



/*
 * Function: ExternInt_SetEvent
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, Int2_Mask).
 * 	       InterruptEvent -> indicate required event for Int0_Mask and Int1_Mask there are 4 events to check (RISING_EDGE,FALLING_EDGE,LOW_LEVEL,LOGICAL_CHANGE).
							  But for Interrupt 2 there are only Two cases (Rising,Falling)	
 * Outputs: 
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to set event of required external interrupt. 
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_SetEvent(uint8 ExternInt_No,uint8 InterruptEvent)
{
		switch(ExternInt_No)
		{
		case (EXTRN_INT_0) :
			/* clear the bits which control the interrupt channel to set it again with new event*/
			CLR_BIT(MCUCR,Int0_Mask_EdgeBits);
			/* set the bits which control the interrupt channel*/
			switch (InterruptEvent)
			{
			case LOW_LEVEL:
				MCUCR |= Int0_EdgeLowLevel ;
				break;
			case LOGICAL_CHANGE :
				MCUCR |= Int0_EdgeChange ;
				break;
			case FALLING_EDGE :
				MCUCR |= Int0_EdgeFalling ;
				break;
			case RISING_EDGE:
				MCUCR |= Int0_EdgeRisiging ;
				break;
			default:
				return E_NOK ;
			}
			break;
		case (EXTRN_INT_1) :
			switch (InterruptEvent)
			{
			CLR_BIT(MCUCR,Int1_Mask_EdgeBits);
			case LOW_LEVEL:
				MCUCR |= Int1_EdgeLowLevel ;
				break;
			case LOGICAL_CHANGE :
				MCUCR |= Int1_EdgeChange ;
				break;
			case FALLING_EDGE :
				MCUCR |= Int1_EdgeFalling ;
				break;
			case RISING_EDGE:
				MCUCR |= Int1_EdgeRisiging ;
				break;
			default:
				return E_NOK;
			}
			break;
		case (EXTRN_INT_2) :
			CLR_BIT(MCUCSR,Int2_Mask_EdgeBits);
			switch(InterruptEvent)
			{
			case FALLING_EDGE :
				MCUCSR |= Int2_EdgeFalling ;
				break;
			case RISING_EDGE:
				MCUCSR |= Int2_EdgeRisiging ;
				break;
			default:
				return E_NOK; 
			}
			break;
		default:
			return E_NOK;		
	}
	return E_OK;	
}


/*
 * Function: ExternInt_Enable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Enable the required external interrupt.
 */
ERROR_STATUS ExternInt_Enable(uint8 ExternInt_No)
{
	/* set the the bit to enable the external interrupt channel*/
	switch (ExternInt_No)
	{
		case EXTRN_INT_0:
			SET_BIT(GICR ,Int0_Mask);
			break;
		case EXTRN_INT_1 :
			SET_BIT(GICR ,Int1_Mask);
			break;
		case EXTRN_INT_2 :
			SET_BIT(GICR ,Int2_Mask);
			break;
		default:
			return E_NOK;
	}
	return E_OK;	
}


/*
 * Function: ExternInt_Disable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, Int2_Mask)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Disable the required external interrupt.
 */
ERROR_STATUS ExternInt_Disable(uint8 ExternInt_No)
{
	switch (ExternInt_No)
	{
		case EXTRN_INT_0:
			CLR_BIT(GICR ,Int0_Mask);
			break;
		case EXTRN_INT_1 :
			CLR_BIT(GICR ,Int1_Mask);
			break;
		case EXTRN_INT_2 :
			CLR_BIT(GICR ,Int2_Mask);
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}	



/************************************************************************/
/*					ISR FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/




ISR(INT0_vect)
{
	if ( NULL !=  INT_CBK_PTR[EXTRN_INT_0])
	{
		INT_CBK_PTR[EXTRN_INT_0]();
	}
}



ISR(INT1_vect)
{
	if ( NULL !=  INT_CBK_PTR[EXTRN_INT_1])
	{
		INT_CBK_PTR[EXTRN_INT_1]();
	}
}



ISR(INT2_vect)
{
		if ( NULL !=  INT_CBK_PTR[EXTRN_INT_2])
		{
			INT_CBK_PTR[EXTRN_INT_2]();
		}
}