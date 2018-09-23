/*
 * GccApplication2.c
 *
 * Created: 2018/9/6 22:00:14
 * Author : Rico
 */ 
#define F_CPU  20000000
#include <avr/io.h>
//#include <avr/delay.h>

int main(void)
{
	DDRB = 0x20;//1<<DDB5;
	PORTB = 0x01;
	
    while (1) 
    {
		if (!(PINB & 0x01))
		{
			PORTB |= 0x20;
			//_delay_ms(3);
		}
		else
		{
			PORTB &= ~(0x20);
			//_delay_ms(3);
		}
    }
}

