/*
 * Us.c
 *
 * Created: 12/24/2019 11:08:25 PM
 *  Author: Mostafa Metwaly
 */ 



/************************************************************************/
/*				 I				INCLUDES						        */
/************************************************************************/
#include "Us.h"
#include "DIO.h"
#include "ICU.h"
#include "softwareDelay.h"





/************************************************************************/
/*								 DEFINES						        */
/************************************************************************/

#define TriggerGpio GPIOB
#define TriggerBit  BIT0

#define EchoGpio	GPIOB
#define EchoBit		BIT2





/************************************************************************/
/*				   functions' prototypes	                            */
/************************************************************************/

/****************************************************************************************************
 * Function Name   : Us_Init.                                                                       *
 * Input Parameters : None.                                                                         *                                                                   *                                                                                                *
 * Output Parameters: None.                                                                         *
 * In_Out Parameters: None.                                                                         *
 * Return           : ERROR_STATUS.                                                                 *
 * Description      : This function initiate the UltraSonic by set the value of the Dio_Dirction of *
 *                    the pin trigger as an output then initiate the ICU to use it to calculate the *
 *                    On time of the ECHO signal.                                                   *
 *                                                                                                  *
 *                                                                                                  *
 ***************************************************************************************************/



ERROR_STATUS Us_Init(void)
{
	DIO_Cfg_st st_USDio;
	st_USDio.GPIO=TriggerGpio;
	st_USDio.pins=TriggerBit;
	st_USDio.dir=HIGH;
	DIO_init(&st_USDio);
	
	Icu_cfg_s ST_USIcu={ICU_CH2,ICU_TIMER_CH2};
	Icu_Init(&ST_USIcu);
	return E_OK;
}






/**************************************************************************************************
 * Function Name    : Us_Trigger.                                                                 *
 * Input Parameters : None.                                                                       *                                                                              *                                                                                                *
 * Output Parameters: None.                                                                       *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS                                                                *
 * Description      : This function send the  pulse, to trigger pin of the UltraSonic to enhance  *
 *                    the  ECUO signal                                                            *
 *************************************************************************************************/


ERROR_STATUS Us_Trigger(void)
{
			volatile int x;
			DIO_Write(TriggerGpio,TriggerBit,HIGH);
			for (x=0;x<1000;x++);
			DIO_Write(TriggerGpio,TriggerBit,LOW);
					

			return E_OK;			
}





/**************************************************************************************************
 * Function Name    : Us_GetDistance.                                                             *
 * Input Parameters : pointer to uint16 that will return the distance on it.                      *                                                                                              *                                                                                                *
 * Output Parameters: Distance.                                                                   *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS.                                                               *
 * Description      : This function get the time of high signal of ECUO pulse, then               *
 *                    use it to calculate the distance, then return the value in the              *
 *                    Distance parameter.                                                         *
 *************************************************************************************************/


ERROR_STATUS Us_GetDistance(uint16 *Distance)
{
	
		
		uint32 u32_TimerValue;

		Icu_RiseToFall(ICU_CH2 , & u32_TimerValue);
		(*Distance)=u32_TimerValue/ 58.2;
		return E_OK;
}
