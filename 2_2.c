/*
 * Lab2_2_2.c
 *
 * Created: 2018/9/13 17:44:21
 * Author : Rico
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000
#define BUAD  9600
#define  BRC  ((F_CPU/16/BUAD)-1)

int flag = 0;
char buffer[8];
int c = 0;
int l = 0;
uint16_t lenth = 0;

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1 << PORTB1);
	flag = 1;
}

void CaptureInput()
{
	TCCR1B |= 0x40;
	TIFR1 |= 0x20;
	while (!(TIFR1 & 0x20));
	
	unsigned int temp = ICR1;
	
	TCCR1B &= 0xBF;
	TIFR1 |= 0x20;
	while(!(TIFR1 & 0x20));
	
	lenth =  ICR1 - temp;
}

void SignalOutput()
{
	
	l = sprintf(buffer, "%hu\n", lenth);
	//for (i)
	while(c < l)
	{
		UDR0 = buffer[c];
		++c;
		_delay_ms(1000);
		while(!(UCSR0A & 0x40));
		UCSR0A |= 0x40;
	}
	c = 0;
//	UDR0 = '8';
	_delay_ms(5000);
	//UDR0 = '\n';
	flag = 0;
}


int main(void)
{
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	
	DDRB |= (1 << PORTB1);
	PORTB |= (1 << PORTB1);
	
	//TCCR1A = (1 << WGM11) | (1 << COM1A0);
	OCR1A = 40;                               // 5 us
	TIMSK1 = (1 << OCIE1A);
	sei();

	
	while (1)
	{
		if (flag == 0)                               //产生下降沿
		{
			TCCR1B = (1 << CS10);                     //1 prescalar
		}
		else if (flag == 1)                          //捕获脉宽
		{
			DDRB |= (0 << PORTB0);
			CaptureInput();
			flag = 2;
			cli();
		}
		else                                         //串口输出
		{
			
			SignalOutput();
			PORTB |= (1 << PORTB1);
			sei();
		}
	}
}
