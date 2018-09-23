/*
 * Lab2_2_4.c
 *
 * Created: 2018/9/16 14:59:22
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
int mid = 0;

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

void FrequencyOutput1()                           //discrete frequency to eight part
{
	//mid = lenth / 1093 +60;           // 2_3 
	//OCR0A = mid;
	mid = lenth;
	switch(mid/4096)
	{
		case(7):
		{
			OCR0A = 119.45;
			break;
		}
		case(6):
		{
			OCR0A = 106.41;
			break;
		}
		case(5):
		{
			OCR0A = 94.8;
			break;
		}
		case(4):
		{
			OCR0A = 89.48;
			break;
		}
		case(3):
		{
			OCR0A = 79.72;
			break;
		}
		case(2):
		{
			OCR0A = 71.02;
			break;
		}
		case(1):
		{
			OCR0A = 63.27;
			break;
		}
		case(0):
		{
			OCR0A = 59.72;
			break;
		}
	}
	sei();
}

void FrequencyOutput2()                     // 2_3
{
	mid = lenth / 1093 +60;
	OCR0A = mid;
	sei();
}

int main(void)
{
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	
	DDRB |= (1 << PORTB1) | (0 << PORTB5);
	PORTB |= (1 << PORTB1) | (1 << PORTB5);
	
	//TCCR1A = (1 << WGM11) | (1 << COM1A0);
	OCR1A = 40;                               // 5 us
	TIMSK1 = (1 << OCIE1A);
	
	
	DDRD = (1 << DDD6);
	PORTD = (1 << PORTD6);
	
	
	TCCR0A = (1 << WGM01) | (1 << COM0A0);   //CTC, toggle OC0A PD6
	TIMSK0 = (1 << OCIE0A);
	TCCR0B |= (1 << CS01) | (1 << CS00);  //64
	
	sei();
	
	while (1)
	{
		if (flag == 0)                               // generate falling edge 
		{
			TCCR1B = (1 << CS10);                     //1 prescalar
			//UDR0 = '\n';
			//_delay_ms(0.005);
			//PORTB ^= (1 << PORTB1);
			//flag = 1;
		}
		else if (flag == 1)                          //capture input
		{
			DDRB |= (0 << PORTB0);
			CaptureInput();
			flag = 2;
			//cli();
		}
		else                                         //output
		{
			//PD6输出频率
			if (!(PINB & 0x20))            //switch button
			{
				FrequencyOutput1();   //discrete
			}
			else
			{
				FrequencyOutput2();  //continuous
			}
			
			//FrequencyOutput1();
			//_delay_ms(1000);
			SignalOutput();
			PORTB |= (1 << PORTB1);
			sei();
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	PORTD ^= (1 << PORTD6);
}

	
	
