/*
 * Lab2_0_3.c
 *
 * Created: 2018/9/9 15:40:36
 * Author : Rico
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRD = 0xF0;
	
	TCCR0A = (1<<WGM01);
	OCR0A = 19500;
	TIMSK0 = (1<<OCIE0A);
	sei();
	
	TCCR0B = (1<<CS02)|(1<<CS00);
    while (1) 
    {
		
    }
}

ISR(TIMER0_COMPA_vect)      //toggle pd6
{
	PORTD ^= (1<<PORTD6);
}
