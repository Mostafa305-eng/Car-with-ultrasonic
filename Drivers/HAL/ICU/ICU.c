/*
 * ICU.c
 *
 * Created: 12/24/2019 3:54:12 PM
 *  Author: Mostafa Metwaly
 */ 


/************************************************************************/
/*				               Includes                                 */
/************************************************************************/

#include "Timer.h"
#include "ExternalInterrupt.h"
#include "ICU.h"




/************************************************************************/
/*								 DEFINES						        */
/************************************************************************/
 #define MaxChannels	3


#define NOTSTARTED	0
#define STARTED		1
#define FINISHED	2
#define COUNTS_FOR_15MICRO  240

#define Int2_EdgeFalling	0x00
#define Int2_EdgeRisiging	0x40
#define Int2_Mask_EdgeBits	0x40



/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/


static uint8 au8_EndedEdges[MaxChannels];
//static void(*ICU_CBK_PTR[MaxChannels])(void) = {NULL,NULL,NULL};


 ExternInt_Cfg_s ast_EXT_INT[MaxChannels];
 Timer_cfg_s ast_Timer_Cfg[MaxChannels];

volatile uint8  u8_ICUState=NOTSTARTED;

volatile uint16 u16_TimerCounter=0;



/************************************************************************/
/*					CALLBACK FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/

/***************************************************************************
 * Function		: Icu_TimerOverFlows_Callback_Func						   *
 * Output 		:														   *
 * Return 		:														   *
 * Description	: Called inside the timer overflow ISR by its address	   *
 * to calculate and deliver the timer overflows to the Icu module		   *
 ***************************************************************************/
void Icu_Timer_Callback_Func(void)
{
	u16_TimerCounter +=15 ; //isr comes every 15 micro 
	

			

}




/***************************************************************************
 * Function		: Icu_EdgeToEdge_Callback_Func					    	   *
 * Output 		:														   *
 * Return 		:														   *
 * Description	: Called inside the external interrupt ISR by its address  *
 * to calculate and deliver the timer counts to the Icu module		       *
 ***************************************************************************/
void Icu_EdgeToEdge_Callback_Func(void)
{
	if (u8_ICUState==NOTSTARTED)
	{	
		/* make interrupt every 15 micro*/
		Timer_Start(ast_Timer_Cfg[ICU_CH2].Timer_CH_NO,COUNTS_FOR_15MICRO);
		/*set the other edge to stop timer*/    
		ExternInt_SetEvent(EXTRN_INT_2,au8_EndedEdges[ICU_CH2]); 
		/*flag for first , second or finshing entering the ISR*/
		u8_ICUState=STARTED; 
			


	} 
	else if(u8_ICUState == STARTED)
	{
		Timer_Stop(ast_Timer_Cfg[ICU_CH2].Timer_CH_NO);
		u8_ICUState=FINISHED;

	}
}


/************************************************************************/
/*							Functions			                         */
/************************************************************************/

/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/
ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)
{
	switch(Icu_Cfg->ICU_Ch_No)
		{
		case ICU_CH0:
			/*init the external interrupt*/
			ast_EXT_INT[ICU_CH0].ExternInt_No=EXTRN_INT_0 ;
			ast_EXT_INT[ICU_CH0].ExternInt_Event=RISING_EDGE;
			ast_EXT_INT[ICU_CH0].ExternInt_CBF_Ptr=Icu_EdgeToEdge_Callback_Func;
			ExternInt_Init(&ast_EXT_INT[ICU_CH0]);
			/*init the timer*/
			switch(Icu_Cfg->ICU_Ch_Timer)
				{
				case ICU_TIMER_CH0:
					ast_Timer_Cfg[ICU_CH0].Timer_CH_NO=TIMER_CH0;
					ast_Timer_Cfg[ICU_CH0].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH0].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH0].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH0].Timer_Cbk_ptr= Icu_Timer_Callback_Func ;
					Timer_Init(&ast_Timer_Cfg[ICU_CH0]);
					break;

				case ICU_TIMER_CH1:
					ast_Timer_Cfg[ICU_CH0].Timer_CH_NO=TIMER_CH1;
					ast_Timer_Cfg[ICU_CH0].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH0].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH0].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH0].Timer_Cbk_ptr=Icu_Timer_Callback_Func  ;
					Timer_Init(&ast_Timer_Cfg[ICU_CH0]);
					break;

				case ICU_TIMER_CH2:
					ast_Timer_Cfg[ICU_CH0].Timer_CH_NO=TIMER_CH2;
					ast_Timer_Cfg[ICU_CH0].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH0].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH0].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH0].Timer_Cbk_ptr=Icu_Timer_Callback_Func ;
					Timer_Init(& (ast_Timer_Cfg[ICU_CH0]));
					break;

				default:
					return E_NOK;

			}
			break;

		case ICU_CH1:
			ast_EXT_INT[ICU_CH1].ExternInt_No=EXTRN_INT_1 ;
			ast_EXT_INT[ICU_CH1].ExternInt_Event=RISING_EDGE;
			ast_EXT_INT[ICU_CH1].ExternInt_CBF_Ptr=Icu_EdgeToEdge_Callback_Func  ;
			ExternInt_Init(&ast_EXT_INT[ICU_CH1]);
			switch(Icu_Cfg->ICU_Ch_Timer)
				{
				case ICU_TIMER_CH0:
					ast_Timer_Cfg[ICU_CH1].Timer_CH_NO=TIMER_CH0;
					ast_Timer_Cfg[ICU_CH1].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH1].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH1].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH1].Timer_Cbk_ptr= Icu_Timer_Callback_Func ;
					Timer_Init(&ast_Timer_Cfg[ICU_CH1]);
					break;

				case ICU_TIMER_CH1:
					ast_Timer_Cfg[ICU_CH1].Timer_CH_NO=TIMER_CH1;
					ast_Timer_Cfg[ICU_CH1].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH1].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH1].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH1].Timer_Cbk_ptr=Icu_Timer_Callback_Func  ;
					Timer_Init(&ast_Timer_Cfg[ICU_CH1]);
					break;

				case ICU_TIMER_CH2:
					ast_Timer_Cfg[ICU_CH1].Timer_CH_NO=TIMER_CH2;
					ast_Timer_Cfg[ICU_CH1].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH1].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH1].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH1].Timer_Cbk_ptr=Icu_Timer_Callback_Func ;
					Timer_Init(& (ast_Timer_Cfg[ICU_CH1]));
					break;

				default:
					return E_NOK;
				}
				
			break;

		case ICU_CH2:
			ast_EXT_INT[ICU_CH2].ExternInt_No=EXTRN_INT_2 ;
			ast_EXT_INT[ICU_CH2].ExternInt_Event=RISING_EDGE;
			ast_EXT_INT[ICU_CH2].ExternInt_CBF_Ptr=Icu_EdgeToEdge_Callback_Func  ;
			ExternInt_Init(&ast_EXT_INT[ICU_CH2]);
			switch(Icu_Cfg->ICU_Ch_Timer)
				{
				case ICU_TIMER_CH0:
					ast_Timer_Cfg[ICU_CH2].Timer_CH_NO=TIMER_CH0;
					ast_Timer_Cfg[ICU_CH2].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH2].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH2].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH2].Timer_Cbk_ptr= Icu_Timer_Callback_Func ;
					Timer_Init(&ast_Timer_Cfg[ICU_CH2]);
					break;

				case ICU_TIMER_CH1:
					ast_Timer_Cfg[ICU_CH2].Timer_CH_NO=TIMER_CH1;
					ast_Timer_Cfg[ICU_CH2].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH2].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH2].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH2].Timer_Cbk_ptr=Icu_Timer_Callback_Func  ;
					Timer_Init(&ast_Timer_Cfg[ICU_CH2]);
					break;

				case ICU_TIMER_CH2:
					ast_Timer_Cfg[ICU_CH2].Timer_CH_NO=TIMER_CH2;
					ast_Timer_Cfg[ICU_CH2].Timer_Mode=TIMER_MODE;
					ast_Timer_Cfg[ICU_CH2].Timer_Prescaler=TIMER_PRESCALER_NO;
					ast_Timer_Cfg[ICU_CH2].Timer_Polling_Or_Interrupt=TIMER_INTERRUPT_MODE;
					ast_Timer_Cfg[ICU_CH2].Timer_Cbk_ptr=Icu_Timer_Callback_Func ;
					Timer_Init(& (ast_Timer_Cfg[ICU_CH2]));
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



/***************************************************************************
 * Function		: Icu_RiseToFall										   *
 * Input		: ICU_Ch_No : external interrupt number					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from rising edge to falling edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK  : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the rising and the falling   *
 * 				  edges												 	   *
 ***************************************************************************/
ERROR_STATUS Icu_RiseToFall(uint8 ICU_Ch_No,uint32 * Icu_Time)
{
	u8_ICUState=NOTSTARTED; //flag for first , second or finshing entering the ISR
	u16_TimerCounter=0;
	switch(ICU_Ch_No)
	{
		case ICU_CH0 :
			ExternInt_SetEvent(ICU_CH0,RISING_EDGE);
			au8_EndedEdges[ICU_CH0]=FALLING_EDGE;
			ExternInt_Enable(EXTRN_INT_0);
			break;
			
		case ICU_CH1 :
			ExternInt_SetEvent(ICU_CH1,RISING_EDGE);
			au8_EndedEdges[ICU_CH1]=FALLING_EDGE;
			ExternInt_Enable(EXTRN_INT_1);
			break;
		
		case ICU_CH2 :
			ExternInt_SetEvent(ICU_CH2,RISING_EDGE);
			au8_EndedEdges[ICU_CH2]=FALLING_EDGE;
			ExternInt_Enable(EXTRN_INT_2);
			break;
		
		default:
			return E_NOK;
	}
	



	while(u8_ICUState != FINISHED);//polling waiting for finished counting	



	u8_ICUState=NOTSTARTED;
	
	* Icu_Time=u16_TimerCounter;
	return E_OK;
}




/***************************************************************************
 * Function		: Icu_FallToRise										   *
 * Input		: ICU_Ch_No : external interrupt number					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from falling edge to rising edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the falling and the rising   *
 * 				  edges													   *
 ***************************************************************************/

ERROR_STATUS Icu_FallToRise(uint8 ICU_Ch_No,uint32 * Icu_Time)
{
	u8_ICUState=NOTSTARTED; //flag for first , second or finshing entering the ISR
	u16_TimerCounter=0;
	switch(ICU_Ch_No)
	{
		case ICU_CH0 :
			ExternInt_SetEvent(ICU_CH0,FALLING_EDGE);
			au8_EndedEdges[ICU_CH0]=RISING_EDGE;
			ExternInt_Enable(EXTRN_INT_0);
		
		case ICU_CH1 :
			ExternInt_SetEvent(ICU_CH1,FALLING_EDGE);
			au8_EndedEdges[ICU_CH1]=RISING_EDGE;
			ExternInt_Enable(EXTRN_INT_1);
		
		case ICU_CH2 :
			ExternInt_SetEvent(ICU_CH2,FALLING_EDGE);
			au8_EndedEdges[ICU_CH2]=RISING_EDGE;
			ExternInt_Enable(EXTRN_INT_2);
		
		default:
			return E_NOK;
	}

	while(u8_ICUState != FINISHED);//polling waiting for finished counting
	u8_ICUState=NOTSTARTED;
	
	* Icu_Time=u16_TimerCounter;
	
	return E_OK;
	
}


/***************************************************************************
 * Function		: Icu_RiseToRise										   *
 * Input		: ICU_Ch_No : external interrupt number					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from rising edge to next rising edge					   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the rising edge and the next *
 * 				  rising edge											   *
 ***************************************************************************/
ERROR_STATUS Icu_RiseToRise(uint8 ICU_Ch_No,uint32 * Icu_Time)
{
	u8_ICUState=NOTSTARTED; //flag for first , second or finshing entering the ISR
	u16_TimerCounter=0;
	switch(ICU_Ch_No)
	{
		case ICU_CH0 :
			ExternInt_SetEvent(ICU_CH0,RISING_EDGE);
			au8_EndedEdges[ICU_CH0]=RISING_EDGE;
			ExternInt_Enable(EXTRN_INT_0);
		
		case ICU_CH1 :
			ExternInt_SetEvent(ICU_CH1,RISING_EDGE);
			au8_EndedEdges[ICU_CH1]=RISING_EDGE;
			ExternInt_Enable(EXTRN_INT_1);
		
		case ICU_CH2 :
			ExternInt_SetEvent(ICU_CH2,RISING_EDGE);
			au8_EndedEdges[ICU_CH2]=RISING_EDGE;
			ExternInt_Enable(EXTRN_INT_2);
		
		default:
			return E_NOK;
	}

	while(u8_ICUState != FINISHED);//polling waiting for finished counting
	u8_ICUState=NOTSTARTED;
	
	* Icu_Time=u16_TimerCounter;
	
	return E_OK;
	
}
	
