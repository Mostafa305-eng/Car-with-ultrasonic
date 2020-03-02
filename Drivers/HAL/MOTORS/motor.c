/*
 * motor.c
 *
 * Created: 12/22/2019 10:58:36 PM
 *  Author: Mostafa Metwaly
 */ 


/************************************************************************/
/*				 I				INCLUDES						        */
/************************************************************************/

#include "motor.h"
#include "DIO.h"
#include "PWM.h"


/************************************************************************/
/*								DEFINES							        */
/************************************************************************/


#define MOTOR_EN_1_GPIO		(GPIOD)
#define MOTOR_EN_1_BIT		(BIT4)

#define MOTOR_OUT_1A_GPIO	(GPIOD)
#define MOTOR_OUT_1B_GPIO	(GPIOD)
#define MOTOR_OUT_1A_BIT	(BIT2)
#define MOTOR_OUT_1B_BIT	(BIT3)


#define MOTOR_EN_2_GPIO		(GPIOD)
#define MOTOR_EN_2_BIT		(BIT5)

#define MOTOR_OUT_2A_GPIO	(GPIOD)
#define MOTOR_OUT_2B_GPIO	(GPIOD)
#define MOTOR_OUT_2A_BIT	(BIT6)
#define MOTOR_OUT_2B_BIT	(BIT7)


/************************************************************************/
/*				  MOTOR FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/


/**
 * Func			: Motor_Init
 * Input		: motorNumber
 *	  	  		  #def
 *	  	  		  number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: ERROR_STATUS
 * Description	: Initialize (Enable, IN1, IN2) pins as digital outputs.
 */
 ERROR_STATUS Motor_Init(uint8 Motor_Number)
 {
	DIO_Cfg_st MotorDIO;
	Pwm_Cfg_s_t MotPwm;
	switch (Motor_Number)
	{
	case MOTOR_1:
		MotorDIO.GPIO=MOTOR_OUT_1A_GPIO;
		MotorDIO.pins=MOTOR_OUT_1A_BIT;
		MotorDIO.dir=OUTPUT;
		DIO_init(&MotorDIO);

		MotorDIO.GPIO=MOTOR_OUT_1B_GPIO;
		MotorDIO.pins=MOTOR_OUT_1B_BIT;
		MotorDIO.dir=OUTPUT;
		DIO_init(&MotorDIO);
		

		MotPwm.Channel=PWM_CH1A;
		MotPwm.Prescaler=PWM_PRESCALER_64;
		Pwm_Init(& MotPwm);
		
		break;
		
	case  MOTOR_2 :

		MotorDIO.GPIO=MOTOR_OUT_2A_GPIO;
		MotorDIO.pins=MOTOR_OUT_2A_BIT;
		MotorDIO.dir=OUTPUT;
		DIO_init(&MotorDIO);

		MotorDIO.GPIO=MOTOR_OUT_2B_GPIO;
		MotorDIO.pins=MOTOR_OUT_2B_BIT;
		MotorDIO.dir=OUTPUT;
		DIO_init(&MotorDIO);


		MotPwm.Channel=PWM_CH1B;
		MotPwm.Prescaler=PWM_PRESCALER_64;
		Pwm_Init(& MotPwm);
		
		break;
	
	 default:
		return E_NOK;
	}
	
	return E_OK;
 }


/**
 * Fun		 : Motor_Direction
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2

			   @param Motor_Direction
			   #define
			   Stop, Forward, Backword
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor direction from getting the motor number and the direction.
*/
ERROR_STATUS Motor_Direction(uint8 Motor_Number, uint8 Motor_Direction)
{
	switch (Motor_Number)
	{
	case MOTOR_1:
		switch (Motor_Direction)
		{
		case MOTOR_STOP:
			DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
			DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
			break;
		case MOTOR_FORWARD :
			DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,HIGH);
			DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
			break;			
		case MOTOR_BACKWORD :
			DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
			DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,HIGH);
			break;
		default:
			return E_NOK;
		}
	break;
	
	case MOTOR_2 :
		switch (Motor_Direction)
		{
			case MOTOR_STOP:
				DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
				DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);
				break;
			case MOTOR_FORWARD :
				DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,HIGH);
				DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);
				break;
			case MOTOR_BACKWORD :
				DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
				DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,HIGH);
				break;
			default:
				return E_NOK;
		}
	break;
	
	default :
		return E_NOK;
	}
	return E_OK;
}


/**
 * Fun		 : Motor_Start
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Start the motor.
*/
ERROR_STATUS Motor_Start(uint8 Motor_Number, uint8 Mot_Speed)
{
	switch (Motor_Number)
	{
	case MOTOR_1:
		Pwm_Start(PWM_CH1A,Mot_Speed,1000);
		break;
	case MOTOR_2:
		Pwm_Start(PWM_CH1B,Mot_Speed,1000);
		break;
	 default:
		return E_NOK;
	}
	return E_OK;
}



/**
 * Fun		 : Motor_SpeedUpdate
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor speed from getting the motor number and the speed.
*/
ERROR_STATUS Motor_SpeedUpdate(uint8 Motor_Number, uint8 Speed)
{
	switch (Motor_Number)
	{
	case MOTOR_1:
		Pwm_Update(PWM_CH1A,Speed,1000);
		break;
	case MOTOR_2:
		Pwm_Update(PWM_CH1B,Speed,1000);
		break;
	 default:
		return E_NOK;
	}
	return E_OK;
}


/**
 * Fun		 : Motor_Stop
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: stop the motor.
*/
ERROR_STATUS Motor_Stop(uint8 Motor_Number)
{
	switch (Motor_Number)
	{
	case MOTOR_1:
		Pwm_Stop(PWM_CH1A);
		DIO_Write(MOTOR_OUT_1A_GPIO,MOTOR_OUT_1A_BIT,LOW);
		DIO_Write(MOTOR_OUT_1B_GPIO,MOTOR_OUT_1B_BIT,LOW);
		break;
	case MOTOR_2:
		Pwm_Stop(PWM_CH1B);
		DIO_Write(MOTOR_OUT_2A_GPIO,MOTOR_OUT_2A_BIT,LOW);
		DIO_Write(MOTOR_OUT_2B_GPIO,MOTOR_OUT_2B_BIT,LOW);		
		break;
	 default:
		return E_NOK;
	}
	return E_OK;
}
