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
	TIFR1 |= (1 << ICF1);  //清除ICP中断标志
	if (!(PINB & 0x01))    //如果PB0为低电平
	{
		PORTB |= 0x20;         //PB5输出高电平
		TCCR1B |=(1 << ICES1);    //改为上升沿触发
	}
	else
	{
		PORTB &= ~(0x20);       //PB5输出低电平
		TCCR1B &= ~(1 << ICES1);    //下降沿触发
	}
}

int main(void)
{
    DDRB = 0x20;
	PORTB |= (1 << PORTB0);

	TCCR1B = (0 << ICES1) | (1 << CS12); //下降沿触发，256分频
	TIMSK1 = (1 << ICIE1);  //输入捕获使能
	TIFR1 |= (1 << ICF1);  //清除ICP中断标志
	
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