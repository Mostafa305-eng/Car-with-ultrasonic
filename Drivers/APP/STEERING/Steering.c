/*
 * Steering.c
 *
 * Created: 12/25/2019 12:09:12 PM
 *  Author: Mostafa Metwaly
 */ 

/************************************************************************/
/*				               Includes                                 */
/************************************************************************/

#include "Steering.h"
#include "motor.h"



/************************************************************************/
/*						 FUNCTIONS IMPLEMENTATION				        */
/************************************************************************/


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the motors.
 * 							
 */

ERROR_STATUS Steering_Init(void)
{
	Motor_Init(MOTOR_1);
	Motor_Init(MOTOR_2);
	
	return E_OK;
}


/**
 * Input: 
 * 	carDirection: The direction of the car from the predefined states of the module.
 * 	speed: The speed of the car from 0% to 100%.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function controls the command and the speed of the car.
 * 							
 */
ERROR_STATUS Steering_SteerCar(uint8 Steering_CarCmd, uint8 speed)
{
	switch(Steering_CarCmd)
	{
		case CAR_FORWARD:
			Motor_Direction(MOTOR_1, MOTOR_FORWARD);
			Motor_Direction(MOTOR_2, MOTOR_FORWARD);
			Motor_Start(MOTOR_1, speed);
			Motor_Start(MOTOR_2, speed);
			break;
		
		case CAR_BACKWARD :
			Motor_Direction(MOTOR_1,MOTOR_BACKWORD);
			Motor_Direction(MOTOR_2,MOTOR_BACKWORD);
			Motor_Start(MOTOR_1, speed);
			Motor_Start(MOTOR_2, speed);
			break;
			
		case CAR_RIGHT :
			Motor_Direction(MOTOR_1,MOTOR_FORWARD);
			Motor_Direction(MOTOR_2,MOTOR_BACKWORD);
			Motor_Start(MOTOR_1, speed);
			Motor_Start(MOTOR_2, speed);
			break;
			
		case CAR_LEFT :
			Motor_Direction(MOTOR_1,MOTOR_BACKWORD);
			Motor_Direction(MOTOR_2,MOTOR_FORWARD);
			Motor_Start(MOTOR_1, speed);
			Motor_Start(MOTOR_2, speed);
			break;
			
		case CAR_STOP:
			Motor_Stop(MOTOR_1);
			Motor_Stop(MOTOR_2);
			break;
			
		default:
			return E_NOK;
	}
	return E_OK;
}
