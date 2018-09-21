/*
 * Lab2_3_3_FINAL.c
 *
 * Created: 2018/9/19 19:31:27
 * Author : Rico
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int adc_result0;
int volu = 0;
#define F_CPU 16000000
#define BUAD  9600
#define  BRC  ((F_CPU/16/BUAD)-1)

char buffer[8];
int l = 0;
int c = 0;
int flag = 0;
int lenth = 0;
int freq = 0;

void setup_ADC()
{
	ADMUX = (1 << REFS0);//比较电压5v
	ADCSRA = (1 << ADEN) | ( 1 << ADATE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);//
	//DIDR0 = (1 << ADC5D); 默认0     ADC prescalar 128
	ADCSRB = (1 << ADTS2) | (1 << ADTS1);
}

void SignalProcess()
{
	l = sprintf(buffer, "%hu\n", lenth);//adc_read(0)
	//for (i)
	while(c < l)
	{
		UDR0 = buffer[c];
		++c;
		_delay_ms(500);
		while(!(UCSR0A & 0x40));
		UCSR0A |= 0x40;
	}
	c = 0;
	//	UDR0 = '8';
	//_delay_ms(5000);
	//UDR0 = '\n';
}

void discreteFreq()
{
	//SignalProcess();   //max 1019  min 37
	
	volu = ADC;//adc_read(0);
	switch(volu/128)//volu/128
	{
		case(0):
		{
			//OCR0A = 119.45;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (0 << PORTB2);
			break;
		}
		case(1):
		{
			//OCR0A = 106.41;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB2);
			break;
		}
		case(2):
		{
			//OCR0A = 94.8;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB3);
			break;
		}
		case(3):
		{
			//OCR0A = 89.48;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB2) | (1 << PORTB3);
			break;
		}
		case(4):
		{
			//OCR0A = 79.72;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB4);
			break;
		}
		case(5):
		{
			//OCR0A = 71.02;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB2) | (1 << PORTB4);
			break;
		}
		case(6):
		{
			//OCR0A = 63.27;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB3) | (1 << PORTB4);
			break;
		}
		case(7):
		{
			//OCR0A = 59.72;
			PORTB &= 0xE3;//PB4:2=000
			PORTB |= (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4);
			break;
		}
	}
}




void CaptureInput()          //测量返回脉冲宽度
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

void FrequencyOutput()
{
	
	freq = lenth;
	switch(freq/100)
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
	//不需要了 谢谢
	DDRD = (1 << DDD6);
	PORTD = (1 << PORTD6);
	
	TCCR0A = (1 << WGM01) | (1 << COM0A0);   //CTC, toggle OC0A PD6
	TIMSK0 = (1 << OCIE0A);
	
	//CNDY
	DDRB |= (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4);
	
	setup_ADC();
	
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    //set up tx
	
	//sei();
	TCCR0B |= (1 << CS01) | (1 << CS00);  //64
	
	TCCR1B |= (1 << CS10) | (1 << CS11);
	TIMSK1 = (1 << TOIE1);
	
	
	
	DDRB |= (1 << PORTB1);
	PORTB &= 0xFD;
	

	OCR2A = 40;                               // 5 us
	TIMSK2 = (1 << OCIE1A);
	
	
	sei();
	
	while (1)
	{
		if (flag == 0)                               //产生下降沿
		{
			TCCR2B = (1 << CS20);
			
			//PORTB |= (1 << PORTB1);
			
			//PORTB ^= (1 << PORTB1);//&= 0xFD;//PB4:2=000
			//flag = 1;
			
			//TCCR1B = (1 << CS10);                     //1 prescalar
			//UDR0 = '\n';
		}
		else if (flag == 1)                          //捕获脉宽
		{
			//TCCR1B |= (1 << CS10) | (1 << CS11);
			//TIMSK1 = (1 << TOIE1);
			
			DDRB |= (0 << PORTB0);
			CaptureInput();
			_delay_ms(1);
			SignalProcess();
			//_delay_ms(5000);
			flag = 2;
			//cli();
		}
		else                                         //串口输出
		{
			
			//PD6输出频率
			FrequencyOutput();
			//_delay_ms(1000);
			flag = 0;
			sei();
			
		}
		//_delay_ms(1);
	}
}

ISR(TIMER1_OVF_vect)
{
	discreteFreq();
	
}

ISR(TIMER0_COMPA_vect)
{
	PORTD ^= (1 << PORTD6);
}

ISR(TIMER2_COMPA_vect)
{
	PORTB ^= (1 << PORTB1);
	flag = 1;
}