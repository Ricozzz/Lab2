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
	TIFR1 |= (1 << ICF1);  //���ICP�жϱ�־
	if (!(PINB & 0x01))    //���PB0Ϊ�͵�ƽ
	{
		PORTB |= 0x20;         //PB5����ߵ�ƽ
		TCCR1B |=(1 << ICES1);    //��Ϊ�����ش���
	}
	else
	{
		PORTB &= ~(0x20);       //PB5����͵�ƽ
		TCCR1B &= ~(1 << ICES1);    //�½��ش���
	}
}

int main(void)
{
    DDRB = 0x20;
	PORTB |= (1 << PORTB0);

	TCCR1B = (0 << ICES1) | (1 << CS12); //�½��ش�����256��Ƶ
	TIMSK1 = (1 << ICIE1);  //���벶��ʹ��
	TIFR1 |= (1 << ICF1);  //���ICP�жϱ�־
	
	sei();
	
	//TCCR1B = (1 << CS11);
	while(1)
	{
		_delay_ms(1);
	}
}


/*
ISR(TIMER1_CAPT_vect)
{
	if (!(TCCR1B & (1 << ICES1)))
	{
		PORTB |= 0x20;
		TCCR1B |=(1 << ICES1);
	}
	else
	{
		PORTB &= ~(0x20);
		TCCR1B &= ~(1 << ICES1);
	}
}

int main(void)
{
	DDRB = 0x20;
	PORTB = 0x01;
	TIMSK1 = (1 << ICIE1);
	if (PINB & 0x01)
	{
		TCCR1B &= ~(1 << ICES1);
	}
	else
	{
		TCCR1B |= (1 << ICES1);
	}
	sei();
	TCCR1B |= (1 << CS10);
}
*/