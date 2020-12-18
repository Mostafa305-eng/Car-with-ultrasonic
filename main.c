/*
 * main.c
 *
 * Created: 12/22/2019 1:49:34 PM
 * Author : Mostafa Metwaly
 */ 


#include "car_sm.h"


int main(void)
{
	sei();
	Car_SM_Init();			
    while (1) 
    {
		Car_SM_Update();
	}
}