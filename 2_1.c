/*
 * Lab2_2_1.c
 *
 * Created: 2018/9/11 10:27:25
 * Author : Rico
 */ 

#include <avr/io.h>
//#include <avr/interrupt.h>

int main(void)
{
    DDRD = (1 << PORTD6);
	TCCR0A = (1 << WGM01) | (1 << COM0A0);// | (1 << COM0A0);
	OCR0A = 70;
	//TIMSK0 = (1 << OCIE0A);	
	TCCR0B = (1 << CS02); // | (1 << CS00);
    /*if(TCNT0 >= OCR0A) 
    {
		PORTD ^= ~(1 << PORTD6);
		TCNT0 = 0;
    }*/
}

