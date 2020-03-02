/*
 * car_sm.c
 *
 * Created: 12/25/2019 12:38:30 PM
 *  Author: Mostafa Metwaly
 */ 



/************************************************************************/
/*				               Includes                                 */
/************************************************************************/
#include "Steering.h"
#include "car_sm.h"
#include "Us.h"
#include "softwareDelay.h"

#if debug_mode
#include "char_lcd.h"
#endif

/************************************************************************/
/*								DEFINES							        */
/************************************************************************/

#define debug_mode 0

#define SPEED80  80
#define SPEED30	 50
#define SPEED0   0

#define DISTANCE_50CM	50
#define DISTANCE_30CM	30
#define DISTANCE_28CM	28


#define CAR_FORWARD30	10
#define CAR_FORWARD80		11


/*************************************************************/
/*                     FUNCTIONS DECLARATION                 */
/*************************************************************/

/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
*/
ERROR_STATUS Car_SM_Init(void)
{
	Us_Init();
	Steering_Init();
		
		#if debug_mode
		LCD_init();	
		#endif
		
		return E_OK;
}


/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
*/
ERROR_STATUS Car_SM_Update(void)
{
	uint16 u16_Distance=0;
	static uint8 u8_CarState=CAR_FORWARD80;
	
	
		Us_Trigger();
		Us_GetDistance(& u16_Distance);
	
	switch(u8_CarState)
	{
		case CAR_FORWARD80 :
			Steering_SteerCar(CAR_FORWARD,SPEED80);
			if (DISTANCE_50CM>u16_Distance && u16_Distance>DISTANCE_30CM )
			{
				u8_CarState=CAR_FORWARD30;
			} 
			else if(u16_Distance<DISTANCE_30CM)
			{
				u8_CarState=CAR_STOP;
			}
			else
			{
				u8_CarState=CAR_FORWARD80;
			}
			break;
		case CAR_FORWARD30 :
			Steering_SteerCar(CAR_FORWARD,SPEED30);
			if (DISTANCE_50CM>u16_Distance && u16_Distance>DISTANCE_30CM )
			{
				u8_CarState=CAR_FORWARD30;
			}
			else if (u16_Distance >DISTANCE_50CM)
			{
				u8_CarState=CAR_FORWARD80;
			}
			else if(u16_Distance<DISTANCE_30CM)
			{
				u8_CarState=CAR_STOP;
			}
			break;
		case CAR_BACKWARD :
			Steering_SteerCar(CAR_BACKWARD,SPEED30);
			
			if(u16_Distance>DISTANCE_30CM)
			{
				u8_CarState=CAR_STOP;
			}
			else
			{
				u8_CarState=CAR_BACKWARD;
			}
			break;
		case CAR_STOP :
			Steering_SteerCar(CAR_STOP,SPEED0);
			SwDelay_ms(500);	
			if(u16_Distance>=DISTANCE_30CM)
			{
				u8_CarState=CAR_RIGHT;
			}
			else
			{
				u8_CarState=CAR_BACKWARD;
			}
			break;
		case CAR_RIGHT :
			Steering_SteerCar(CAR_RIGHT,SPEED80);
			SwDelay_ms(400);
			u8_CarState=CAR_FORWARD30;
			
		default:
			return E_NOK;
	}
	
	
	
	
	#if debug_mode
	char lcd_buffer[16]; 
	
	if (u8_CarState == CAR_STOP)
	{
		
		LCD_goto_xy(1,1);
		LCD_send_string("stop");
		
	
		
	} 
	else if(u8_CarState == CAR_RIGHT)
	{
		
		LCD_goto_xy(1,1);
		LCD_send_string("right");
			
	}
	else if (u8_CarState==CAR_FORWARD30 || u8_CarState== CAR_FORWARD80)
	{
		LCD_goto_xy(1,1);
		LCD_send_string("forward");
	
	}
	else
	{
		LCD_goto_xy(1,1);
		LCD_send_string("backword");
	}

	
	LCD_goto_xy(0,0);
	itoa(u16_Distance,lcd_buffer,10);
	LCD_send_string(lcd_buffer);
	SwDelay_ms(10);
	LCD_clear();
	
	#endif
	
			
return E_OK;	
}