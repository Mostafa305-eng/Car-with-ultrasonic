
/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/


#include "Timer.h"
#include "registers.h"
#include "interrupt.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define Started					1
#define NotStarted				0


#define COUNTER_RISING_MASK		0x07
#define COUNTER_FALLING_MASK	0x06

#define T0_COMP_MODE			0x08
#define T0_INTERRUPT_CMP		0x02
#define T0_POLLING				0
#define T0_OUTPUT_COMPARE_FLAG	0x02
#define T0_COUNTER_MAX			255

#define T1_COMP_MODE_OCR1A_TOP	0x0008
#define T1_POLLING				0
#define T1_INTERRUPT_CMP_1A		0x10
#define T1_COUNTER_MAX			65535
#define T1_OUTPUT_COMPARE_FLAG	0x10

#define T2_NORMAL_MODE			0
#define T2_COMP_MODE			0x08
#define T2_POLLING				0
#define T2_INTERRUPT_CMP		0x80
#define T2_COUNTER_MAX			255
#define T2_OUTPUT_COMPARE_FLAG	0x80

#define PRESCALAR_BITS			0x07
#define NO_OF_TIMERS			3


/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/

static uint8 gau8_Timer_Prescalar[]={0,0,0};
static uint8 gau8_Timer_Started[]={NotStarted,NotStarted,NotStarted};
static uint8 gau8_Timer0_PrescalerTable[]={0x00,0x01,0x02,0x00,0x03,0x00,0x04,0x05};
static uint8 gau8_Timer1_PrescalerTable[]={0x00,0x01,0x02,0x00,0x03,0x00,0x04,0x05};
static uint8 gau8_Timer2_PrescalerTable[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};


static  void(*TIMER_OC_CBK_PTR[NO_OF_TIMERS])(void) = {NULL,NULL,NULL};

/************************************************************************/
/*				  TIMER FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/



/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */

ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg)
{
	switch(Timer_cfg->Timer_CH_NO)
	{
		case (TIMER_CH0) :
			switch(Timer_cfg->Timer_Mode)
			{
				case(TIMER_MODE) :
					/*configure the timer as ctc mode */
					TCCR0 |= T0_COMP_MODE;
					/*check if the user enters the right prescaler or not */
					if ((gau8_Timer0_PrescalerTable[Timer_cfg->Timer_Prescaler]) != 0 \
					&& (Timer_cfg->Timer_Prescaler) <8)
					{
						
						/* find the prescaler mask from masks table and save it to use when the user 
						start the timer*/
						gau8_Timer_Prescalar[TIMER_CH0] = gau8_Timer0_PrescalerTable[Timer_cfg->Timer_Prescaler];
					}
					else
					{
						return E_NOK;
					}
					break;
				/*if the the mode is counter mode , save the mask of the prescaler 
				of the external clock*/
				case (COUNTER_RISING_MODE):
					gau8_Timer_Prescalar[TIMER_CH0] = COUNTER_RISING_MASK;
					break;
		
				case (COUNTER_FALLING_MODE):
					gau8_Timer_Prescalar[TIMER_CH0] =COUNTER_FALLING_MASK;
					break;
					
				default :
					return E_NOK;
			}
			switch(Timer_cfg->Timer_Polling_Or_Interrupt)
			{
				case(TIMER_INTERRUPT_MODE) :
					(TIMSK) |= T0_INTERRUPT_CMP;
					break;
				case(TIMER_POLLING_MODE) :
					TIMSK |= T0_POLLING;
					break;
				default :
					return E_NOK;
			}
			/*save the call back function in an array of pointers to function*/
			if ( NULL !=  Timer_cfg->Timer_Cbk_ptr)
			{
				TIMER_OC_CBK_PTR[TIMER_CH0]=Timer_cfg->Timer_Cbk_ptr;
			}
			break;
		case (TIMER_CH1) :
			switch(Timer_cfg->Timer_Mode)
			{
				case(TIMER_MODE):
					TCCR1 |=T1_COMP_MODE_OCR1A_TOP;
					if ((gau8_Timer1_PrescalerTable[Timer_cfg->Timer_Prescaler]) != 0 \
					&& (Timer_cfg->Timer_Prescaler) <8)
					{
						gau8_Timer_Prescalar[TIMER_CH1]=gau8_Timer1_PrescalerTable[Timer_cfg->Timer_Prescaler];
					}
					else
					{
						return E_NOK;
					}
					break;
				case (COUNTER_RISING_MODE):
					gau8_Timer_Prescalar[TIMER_CH1] |=COUNTER_RISING_MASK;
					break;
		
				case (COUNTER_FALLING_MODE):
					gau8_Timer_Prescalar[TIMER_CH1] |=COUNTER_RISING_MASK;
					break;
					
				default :
					return E_NOK;
			}
			switch(Timer_cfg->Timer_Polling_Or_Interrupt)
			{
				case(TIMER_INTERRUPT_MODE ) :
					TIMSK |= T1_INTERRUPT_CMP_1A;
					break;
				case(TIMER_POLLING_MODE) :
					TIMSK |= T1_POLLING;
					break;
				default :
					return E_NOK;
			}
			if ( NULL !=  Timer_cfg->Timer_Cbk_ptr)
			{
				TIMER_OC_CBK_PTR[TIMER_CH1]=Timer_cfg->Timer_Cbk_ptr;
			}
			break;
		case (TIMER_CH2) :
			switch(Timer_cfg->Timer_Mode)
			{
				case(TIMER_MODE):
					TCCR2 |=T2_COMP_MODE;
					if ((gau8_Timer2_PrescalerTable[Timer_cfg->Timer_Prescaler]) != 0 \
					&& (Timer_cfg->Timer_Prescaler) <8)
					{
						gau8_Timer_Prescalar[TIMER_CH2]=gau8_Timer2_PrescalerTable[Timer_cfg->Timer_Prescaler];
					}
					else
					{
						return E_NOK;
					}
					break;
				case (COUNTER_RISING_MODE):
					break;
		
				case (COUNTER_FALLING_MODE):
					break;
					
				default :
					return E_NOK;
			}
			switch(Timer_cfg->Timer_Polling_Or_Interrupt)
			{
				case(TIMER_INTERRUPT_MODE ) :
					TIMSK |= T2_INTERRUPT_CMP;
					break;
				case(TIMER_POLLING_MODE) :
					TIMSK |= T2_POLLING;
					break;
				default :
					return E_NOK;
			}
			if ( NULL !=  Timer_cfg->Timer_Cbk_ptr)
			{
				TIMER_OC_CBK_PTR[TIMER_CH2]=Timer_cfg->Timer_Cbk_ptr;
			}
			break;
	
		default :
			return E_NOK;
		
	}
	return E_OK;
}






/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function strats the needed timer.
 * 							
 */
 
ERROR_STATUS Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count)
{
	switch(Timer_CH_NO)
	{
		case TIMER_CH0 :
			/*checks the timer counts */
			if(Timer_Count > T0_COUNTER_MAX)
			{
				return E_NOK;
			}
			else if(gau8_Timer_Prescalar[TIMER_CH0] == TIMER_NO_CLOCK)
			{
				return E_NOK;
			}
			else
			{	
				/*init the flag as started*/
				gau8_Timer_Started[TIMER_CH0]=Started;
				OCR0 = Timer_Count;
				/*start the timer*/
				TCCR0 |= gau8_Timer_Prescalar[TIMER_CH0];
			}
			break;

		case TIMER_CH1 :
			if(Timer_Count > T1_COUNTER_MAX)
			{
				return E_NOK;
			}
			else if(gau8_Timer_Prescalar[TIMER_CH1] == TIMER_NO_CLOCK)
			{
				return E_NOK;
			}
			else
			{
				gau8_Timer_Started[TIMER_CH1]=Started;
				OCR1A = Timer_Count;
				TCCR1 |= gau8_Timer_Prescalar[TIMER_CH1];
			}
			break;
			
		case TIMER_CH2 :
			if(Timer_Count > T2_COUNTER_MAX)
			{
				return E_NOK;
			}
			else if(gau8_Timer_Prescalar[TIMER_CH2] == TIMER_NO_CLOCK)
			{
				return E_NOK;
			}
			else
			{
				gau8_Timer_Started[TIMER_CH2]=Started;
				OCR2 =Timer_Count;
				TCCR2 |= gau8_Timer_Prescalar[TIMER_CH2];
			}
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}







/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */
ERROR_STATUS Timer_Stop(uint8 Timer_CH_NO)
{
	switch(Timer_CH_NO)
	{
		case TIMER_CH0 :
			/*check if the timer is started or not*/
			if(gau8_Timer_Started[TIMER_CH0] != Started)
			{
				return E_NOK;
			}
			else
			{
				/*stop the timer by clearing the bits of the prescaler */
				TCCR0 &= ~(PRESCALAR_BITS);
				gau8_Timer_Started[TIMER_CH0]=NotStarted;
			}
			break;
		case TIMER_CH1 :
			if(gau8_Timer_Started[TIMER_CH1] != Started)
			{
				return E_NOK;
			}
			else
			{
				TCCR1 &= ~(PRESCALAR_BITS);
				gau8_Timer_Started[TIMER_CH1]=NotStarted;
			}
			break;
		case TIMER_CH2 :
			if(gau8_Timer_Started[TIMER_CH2] != Started)
			{
				return E_NOK;
			}
			else
			{
				TCCR2 &= ~(PRESCALAR_BITS);
				gau8_Timer_Started[TIMER_CH2]=NotStarted;
			}
			break;
		default:
			return E_NOK;
	}
	return E_OK;
}






/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 *				It is used in polling mode 
 * 							
 */


ERROR_STATUS Timer_GetStatus(uint8 Timer_CH_NO, bool* Data)
{
	switch(Timer_CH_NO)
	{
		case TIMER_CH0 :
			/*get the flag of the output compare timer*/
			* Data = GET_BIT(TIFR,T0_OUTPUT_COMPARE_FLAG);
			if ((*Data) == TRUE)
			{
				/*if the flag is raised , set the flag to clear */
				SET_BIT(TIFR,T0_OUTPUT_COMPARE_FLAG);
			}
			break;
		case TIMER_CH1 :
			* Data = GET_BIT(TIFR,T1_OUTPUT_COMPARE_FLAG);
			if ((*Data) == TRUE)
			{
				SET_BIT(TIFR,T1_OUTPUT_COMPARE_FLAG);
			}
			break;
		case TIMER_CH2 :
			* Data = GET_BIT(TIFR,T2_OUTPUT_COMPARE_FLAG);
			if ((*Data) == TRUE)
			{
				SET_BIT(TIFR,T2_OUTPUT_COMPARE_FLAG);
			}			
			break;
		default :
			return E_NOK;
	}
	return E_OK;
}


/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */

ERROR_STATUS Timer_GetValue(uint8 Timer_CH_NO, uint16* Data)
{
	switch(Timer_CH_NO)
	{
		case TIMER_CH0 :
			* Data = TCNT0;
			break;
		case TIMER_CH1 :
			* Data = TCNT1;
			break;
		case TIMER_CH2 :
			* Data = TCNT2;
			break;
		default :
			return E_NOK;
	}	
	return E_OK;
}



/************************************************************************/
/*					ISR FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/





ISR(TIMER0_COMP_vect)
{
	if ( NULL != TIMER_OC_CBK_PTR[TIMER_CH0] )
	{
		TIMER_OC_CBK_PTR[TIMER_CH0]();
	}
	
}



ISR(TIMER1_COMPA_vect)
{
		if ( NULL != TIMER_OC_CBK_PTR[TIMER_CH1] )
		{
			TIMER_OC_CBK_PTR[TIMER_CH1]();
		}
			
}



ISR(TIMER2_COMP_vect)
{
		if ( NULL != TIMER_OC_CBK_PTR[TIMER_CH2] )
		{
			TIMER_OC_CBK_PTR[TIMER_CH2]();
		}
}

