/*
 * PWM.c
 *
 * Created: 12/23/2019 1:08:07 PM
 *  Author: Mostafa Metwaly
 */ 


/*********************************************
 *				Includes  					 *
 ********************************************/

#include "registers.h"
#include "PWM.h"
#include "DIO.h"

/************************************************************************/
/*								DEFINES							        */
/************************************************************************/


#define T0_PWM_PHASE_CORRECT			0x40
#define T0_CLEAR_ON_OC					0x20

#define T1_PWM_PHASE_CORRECT			0x0010
#define T1A_CLEAR_ON_OC					0x8000
#define T1B_CLEAR_ON_OC					0x2000

#define T2_PWM_PHASE_CORRECT			0x40
#define T2_CLEAR_ON_OC					0x20


#define		PWM0						0
#define		PWM1						1
#define		PWM2						2

#define PWM0_GPIO						GPIOB
#define PWM0_BIT						BIT3

#define PWM1A_GPIO						GPIOD
#define PWM1A_BIT						BIT5

#define PWM1B_GPIO						GPIOD
#define PWM1B_BIT						BIT4

#define PWM2_GPIO						GPIOD
#define PWM2_BIT						BIT7

#define TCCNT_MAX						255

#define PRESCALER_NO_VALUE				1
#define PRESCALER_8_VALUE				8
#define PRESCALER_32_VALUE				8

#define Started					1
#define NotStarted				0

#define PREscaler_BITS			0x07


/************************************************************************/
/*				  Static Global variables				        */
/************************************************************************/

static uint8 gau8_PwmPrescalerMask[]={0,0,0};
static const uint8 gau8_Pwm0PrescalerMaskTable[]={0x01,0x02,0x00,0x03,0x00,0x04,0x05};
static const uint8 gau8_Pwm1PrescalerMaskTable[]={0x01,0x02,0x00,0x03,0x00,0x04,0x05};
static const uint8 gau8_Pwm2PrescalerMaskTable[]={0x01,0x02,0x03,0x04,0x05,0x06,0x07};


static uint16 gsau8_PrescalerValue[] = {0, 0, 0};
static const uint16 gsau8_Pwm0PrescalerValueTable[]={1, 8, 0 , 64, 0  , 256, 1024};
static const uint16 gsau8_Pwm1PrescalerValueTable[]={1, 8, 0 , 64, 0  , 256, 1024};
static const uint16 gsau8_Pwm2PrescalerValueTable[]={1, 8, 32, 64, 128, 256, 1024};
	
static uint8 gau8_PwmStarted[]={NotStarted,NotStarted,NotStarted};

/************************************************************************/
/*					 FUNCTIONS IMPLEMENTATION					        */
/************************************************************************/


/*********************************************************************************
* Function: Error_State Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg);                         *
* Type: public                                                                  *
* Input parameters: Pwm_Cfg Structure (channel number, Prescaler)               *
* Return type : void                                                            *
*                                                                               *
* Description: initialize the PWM configuration                                 *
*********************************************************************************/
extern ERROR_STATUS Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg)
{
	switch(Pwm_Cfg->Channel)
	{
		/*declare PWM struct to use it in configuration */
		DIO_Cfg_st PwmDio;
		case PWM_CH0 :
			/*check if the user enters the right prescaler or not */
			if ((gau8_Pwm0PrescalerMaskTable[Pwm_Cfg->Prescaler]) != 0 \
			&& (Pwm_Cfg->Prescaler) < 7)
			{
				/* find the prescaler mask from masks table and save it to use when the user 
				start the PWM*/
				gau8_PwmPrescalerMask[PWM0]=gau8_Pwm0PrescalerMaskTable[Pwm_Cfg->Prescaler];
			}
			else
			{
				return E_NOK;
			}
			/*configure the PWM as phase correct and the output compare pin as 
			clear on matching*/
			TCCR0 |= (T0_CLEAR_ON_OC	|  T0_PWM_PHASE_CORRECT);
			
			/*finding the value of the prescaler from the table and save it in an array to 
			use in the calculations of the Frequncy */
			gsau8_PrescalerValue[PWM0]=gsau8_Pwm0PrescalerValueTable[Pwm_Cfg->Prescaler];
			
			/*configure oc pin as an output*/
			PwmDio.GPIO=PWM0_GPIO;
			PwmDio.pins=PWM0_BIT;
			PwmDio.dir=OUTPUT;
			DIO_init(&PwmDio);
			break;

		case PWM_CH1A :
			if ((gau8_Pwm1PrescalerMaskTable[Pwm_Cfg->Prescaler]) != 0 \
			&& (Pwm_Cfg->Prescaler) <7)
			{
				gau8_PwmPrescalerMask[PWM1]=gau8_Pwm1PrescalerMaskTable[Pwm_Cfg->Prescaler];
			}
			else
			{
				return E_NOK;
			}
			TCCR1 |= (T1A_CLEAR_ON_OC	|  T1_PWM_PHASE_CORRECT);
			gsau8_PrescalerValue[PWM1]=gsau8_Pwm1PrescalerValueTable[Pwm_Cfg->Prescaler];
			PwmDio.GPIO=PWM1A_GPIO;
			PwmDio.pins=PWM1A_BIT;
			PwmDio.dir=OUTPUT;
			DIO_init(&PwmDio);
			break;
		case PWM_CH1B:
			if ((gau8_Pwm1PrescalerMaskTable[Pwm_Cfg->Prescaler]) != 0 \
			&& (Pwm_Cfg->Prescaler) <7)
			{
				gau8_PwmPrescalerMask[PWM1]=gau8_Pwm1PrescalerMaskTable[Pwm_Cfg->Prescaler];
			}
			else
			{
				return E_NOK;
			}
			TCCR1 |= (T1B_CLEAR_ON_OC	|  T1_PWM_PHASE_CORRECT);
			gsau8_PrescalerValue[PWM1]=gsau8_Pwm1PrescalerValueTable[Pwm_Cfg->Prescaler];			
			PwmDio.GPIO=PWM1B_GPIO;
			PwmDio.pins=PWM1B_BIT;
			PwmDio.dir=OUTPUT;
			DIO_init(&PwmDio);
			break;
		
		case PWM_CH2 :
			if ((gau8_Pwm2PrescalerMaskTable[Pwm_Cfg->Prescaler]) != 0 \
			&& (Pwm_Cfg->Prescaler) <7)
			{
				gau8_PwmPrescalerMask[PWM2]=gau8_Pwm2PrescalerMaskTable[Pwm_Cfg->Prescaler];
			}
			else
			{
				return E_NOK;
			}
			PwmDio.GPIO=PWM2_GPIO;
			PwmDio.pins=PWM2_BIT;
			PwmDio.dir=OUTPUT;
			DIO_init(&PwmDio);
			TCCR2 |= (T2_CLEAR_ON_OC	|  T2_PWM_PHASE_CORRECT);
			gsau8_PrescalerValue[PWM2]=gsau8_Pwm2PrescalerValueTable[Pwm_Cfg->Prescaler];			
			break;
		default:
			return E_NOK;
	}
		return E_OK;
}


/*********************************************************************************
* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                *
* Type: public                                                                  *
* Input parameters: Channel ID, Duty cycle(0:100)                               *
* Return type : Error State                                                     *
*                                                                               *
* Description: starts the PWM on the dedicated channel with the required duty   *
*				cycle and frequency                                             *
*-------------------------------------------------------                        *
*(IN timer 0) there are only 5 available Frequencies:                           *
*-------------------------------------------------------                        *
*Frequency (NO-Prescaler) --> 31,372HZ                                          *
*Frequency (Prescaler 8) --> 3,921HZ                                            *
*Frequency (Prescaler 64) --> 490HZ                                             *
*Frequency (Prescaler 256) --> 122HZ                                            *
*Frequency (Prescaler 1024) --> 31HZ                                            *
*--------------------------------------------------------                       *
*(IN timer 2) there are only 7 available Frequencies:                           *
*-------------------------------------------------------                        *
*Frequency (NO-Prescaler) --> 31,372HZ                                          *
*Frequency (Prescaler 8) --> 3,921HZ                                            *
*Frequency (Prescaler 32) --> 980HZ                                             *
*Frequency (Prescaler 64) --> 490HZ                                             *
*Frequency (Prescaler 128) --> 245HZ                                            *
*Frequency (Prescaler 256) --> 122HZ                                            *
*Frequency (Prescaler 1024) --> 31HZ                                            *
*-------------------------------------------------------                        *
*((all this calculations are for phase correct mode))                           *
*all this calculation for F_CPU 16MHz                                           *
*********************************************************************************/
extern ERROR_STATUS Pwm_Start(uint8 Channel,uint8 Duty,uint32 Frequncy)
{
	switch(Channel)
	{
		case PWM_CH0 :
			/*check the duty and the prescaler*/
			if (Duty>100 || gau8_PwmPrescalerMask[PWM0] == 0)
			{
				return E_OK;
			}
			/*calculate the counts which achieve the required duty cycle*/
			OCR0 = (Duty*TCCNT_MAX)/100;
			/*start the PWM*/
			TCCR0 |= gau8_PwmPrescalerMask[PWM0];
			/*init the flag of the required PWM channel as started*/
			gau8_PwmStarted[PWM0]=Started;	
			break;
		
		case PWM_CH1A :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM1] == 0 )
			{
				return E_OK;
			}
			/*calculate the frequency */
			ICR1=F_CPU/( gsau8_PrescalerValue[PWM1] * Frequncy * 2);
			OCR1A = (Duty*(F_CPU/( gsau8_PrescalerValue[PWM1] * Frequncy * 2)))/100;
			TCCR1 |= gau8_PwmPrescalerMask[PWM1];
			gau8_PwmStarted[PWM1]=Started;
			break;

		case PWM_CH1B :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM1] == 0 )
			{
				return E_OK;
			}
			ICR1=F_CPU/( gsau8_PrescalerValue[PWM1] * Frequncy * 2);
			OCR1B = (Duty*(F_CPU/( gsau8_PrescalerValue[PWM1] * Frequncy * 2)))/100;
			TCCR1 |= gau8_PwmPrescalerMask[PWM1];
			gau8_PwmStarted[PWM1]=Started;
			break;

		case PWM_CH2 :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM2] == 0 )
			{
				return E_OK;
			}
			OCR2 = (Duty*TCCNT_MAX)/100;
			TCCR2 |= gau8_PwmPrescalerMask[PWM2];	
			gau8_PwmStarted[PWM2]=Started;
			break;
	}
	return E_OK;
}

extern ERROR_STATUS Pwm_Update(uint8 Channel,uint8 Duty,uint32 Frequncy)
{
		switch(Channel)
	{
		case PWM_CH0 :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM0] == 0 )
			{
				return E_OK;
			}
			OCR0 = (Duty*TCCNT_MAX)/100;
			break;
		
		case PWM_CH1A :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM1] == 0 )
			{
				return E_OK;
			}
			ICR1=F_CPU/( gsau8_Pwm1PrescalerValueTable[PWM1] * Frequncy * 2);

			OCR1A = (Duty*(F_CPU/( gsau8_Pwm1PrescalerValueTable[PWM1] * Frequncy * 2)))/100;
			break;

		case PWM_CH1B :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM1] == 0 )
			{
				return E_OK;
			}
			ICR1=F_CPU/( gsau8_Pwm1PrescalerValueTable[PWM1] * Frequncy * 2);
			
			OCR1B = (Duty*(F_CPU/( gsau8_Pwm1PrescalerValueTable[PWM1] * Frequncy * 2)))/100;
			break;

		case PWM_CH2 :
			if (Duty>100 || gau8_PwmPrescalerMask[PWM2] == 0 )
			{
				return E_OK;
			}
			OCR2 = (Duty*TCCNT_MAX)/100;
			break;
	}
	return E_OK;

}


extern ERROR_STATUS Pwm_Stop(uint8 Channel)
{
	switch(Channel)
	{
		case PWM_CH0 :
			/*check if the PWM is started or not */
			if (gau8_PwmStarted[PWM0] != Started)
			{
				return E_NOK;
			}
			OCR0 = 0;
			CLR_BIT(TCCR0,PREscaler_BITS);
			gau8_PwmStarted[PWM0] = NotStarted;
			break;
		
		case PWM_CH1A :
			if (gau8_PwmStarted[PWM0] != Started)
			{
				return E_NOK;
			}
			OCR1A = 0;
			CLR_BIT(TCCR1,PREscaler_BITS);
			gau8_PwmStarted[PWM1] = NotStarted;
			break;

		case PWM_CH1B :
			if (gau8_PwmStarted[PWM0] != Started)
			{
				return E_NOK;
			}
			OCR1B = 0;
			CLR_BIT(TCCR1,PREscaler_BITS);
			gau8_PwmStarted[PWM1] = NotStarted;
			break;

		case PWM_CH2 :
			if (gau8_PwmStarted[PWM0] != Started)
			{
				return E_NOK;
			}
			OCR2 = 0;
			CLR_BIT(TCCR2,PREscaler_BITS);
			gau8_PwmStarted[PWM2] = NotStarted;
			break;
	}
	return E_OK;
}


















