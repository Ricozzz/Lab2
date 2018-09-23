/*
 * Lab2_1_1.c
 *
 * Created: 2018/9/10 23:42:26
 * Author : Rico
 */ 
#define  F_CPU  16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(TIMER1_CAPT_vect)
{
	TIFR1 |= (1 << ICF1);  //clear ICP
	if (!(PINB & 0x01))    //
	{
		PORTB |= 0x20;         //PB5 = 1
		TCCR1B |=(1 << ICES1);    //rising edge trigger
	}
	else
	{
		PORTB &= ~(0x20);       //PB5 = 0
		TCCR1B &= ~(1 << ICES1);    //falling edge
	}
}

int main(void)
{
    DDRB = 0x20;
	PORTB |= (1 << PORTB0);

	TCCR1B = (0 << ICES1) | (1 << CS12); //falling edge，256 prescalar
	TIMSK1 = (1 << ICIE1);  //capture input enable
	TIFR1 |= (1 << ICF1);  //clear ICP
	
	sei();
	
	//TCCR1B = (1 << CS11);
	while(1)
	{
		_delay_ms(1);
	}
}
