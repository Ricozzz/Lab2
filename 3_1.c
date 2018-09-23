/*
 * Lab2_3_1.c
 *
 * Created: 2018/9/16 16:41:10
 * Author : Rico
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int adc_result0;
int freq = 0;
#define F_CPU 16000000
#define BUAD  9600
#define  BRC  ((F_CPU/16/BUAD)-1)

char buffer[8];
int l = 0;
int c = 0;

void setup_ADC()                             //ADC set up
{
	ADMUX = (1 << REFS0);//±È½ÏµçÑ¹5v 
	ADCSRA = (1 << ADEN) | ( 1 << ADATE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);//
	//DIDR0 = (1 << ADC5D); Ä¬ÈÏ0     ADC prescalar 128
	ADCSRB = (1 << ADTS2) | (1 << ADTS1);
}

uint16_t adc_read(uint16_t ch)          // can be deleted
{
	ch &= 0x00;
	ADMUX = (ADMUX & 0xF8) | ch;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return(ADC);
}

void SignalProcess()                  //output ADC value
{
	l = sprintf(buffer, "%hu\n", adc_read(0));
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
	_delay_ms(500);
	//UDR0 = '\n';
}

void discreteFreq()             //discrete voltage to eight parts
{
	SignalProcess();   //max 1019  min 37
	
	freq = adc_read(0);
	switch(freq/128)
	{
		case(0):
		{
			OCR0A = 119.45;
			break;
		}
		case(1):
		{
			OCR0A = 106.41;
			break;
		}
		case(2):
		{
			OCR0A = 94.8;
			break;
		}
		case(3):
		{
			OCR0A = 89.48;
			break;
		}
		case(4):
		{
			OCR0A = 79.72;
			break;
		}
		case(5):
		{
			OCR0A = 71.02;
			break;
		}
		case(6):
		{
			OCR0A = 63.27;
			break;
		}
		case(7):
		{
			OCR0A = 59.72;
			break;
		}
	}
}

int main(void)
{
	DDRD = (1 << DDD6);
	PORTD = (1 << PORTD6);
	
	TCCR0A = (1 << WGM01) | (1 << COM0A0);   //CTC, toggle OC0A PD6
	TIMSK0 = (1 << OCIE0A);
	
	setup_ADC();
	
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    //set up tx
	
	//sei();
	TCCR0B |= (1 << CS01) | (1 << CS00);  //64
	
	TCCR1B |= (1 << CS10) | (1 << CS11);
	TIMSK1 = (1 << TOIE1);
		
	sei();
	
	while (1)
	{
		//adc_result0 = adc_read(0)/17 + 59;
		//OCR0A = adc_result0;
	}
}

ISR(TIMER1_OVF_vect)
{
	discreteFreq();
}
