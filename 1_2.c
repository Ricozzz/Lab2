/*
 * Lab2_1_2.c
 *
 * Created: 2018/9/11 9:36:16
 * Author : Rico
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRD = 0xF0;
	
	TCCR0A = (1<<WGM01) | (1 << COM0A0);// | (0 << COM0A1)
	//TCNT0 = 0;
	OCR0A = 70;
	TIMSK0 = (1<<OCIE0A);
	sei();
	
	TCCR0B = (1<<CS02);//|(1<<CS00);
}

ISR(TIMER0_COMPA_vect)
{
	PORTD ^= (1<<PORTD6);
	
}