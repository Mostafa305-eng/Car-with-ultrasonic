/*
 * CFile1.c
 *
 * Created: 12/3/2019 6:03:23 PM
 *  Author: Mostafa Metwaly
 */ 
#include "std_types.h"

void SwDelay_ms(uint32 n){
	uint32 counter1;
	for (counter1=0; counter1<(n*1545) ;counter1 ++){
		asm("nop");
	}
}
