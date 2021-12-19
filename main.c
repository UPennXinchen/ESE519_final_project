/*
 * FastPWM_FAN_Speed.c
 *
 * Created: 12/1/2021 4:42:16 PM
 * Author : 81046
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"


#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

char String[25];
volatile int D=5; // duty cycle == (0-100)
volatile int temp_cel;
//volatile int a= 2;

void Serial_Ini()
{
	UART_init(BAUD_PRESCALER);
}

void PWM_Ini()
{
	//GPIO pins setup
	DDRD |= (1<<DDD6);
	DDRD &= ~(1<<DDD3);
	DDRD &= ~(1<<DDD4);
	DDRD &= ~(1<<DDD5);
	
	//prescale the timer0 to 1/256
	TCCR0B &= ~(1<<CS00);
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= (1<<CS02);

	//Set the Fast PWM mode
	TCCR0A |= (1<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0B &= ~(1<<WGM02);
	OCR0A=(256*D/100 - 1);

	// toggle OC0A on compare match
	TCCR0A |= (1<<COM0A1);

}

void ADC_Ini()
{
	//clear power reduction for ADC
	PRR &= (1<<PRADC);
	//Vref = ADC
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	//Set the ADC clock: divided by 64
	ADCSRA |= (1<<ADPS0);
	ADCSRA &= ~(1<<ADPS1);
	ADCSRA |= (1<<ADPS2);
	//select channel 0
	ADMUX &= ~(1<<MUX0);
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX3);
	//Set to free running
	ADCSRA |= (1<<ADATE);  //auto triggering of adc
	ADCSRB &= ~(1<<ADTS0);  //free running mode ADT[2:0] = 000
	ADCSRB &= ~(1<<ADTS1);
	ADCSRB &= ~(1<<ADTS2);
	//disable digital input buffer
	DIDR0 |= (1<<ADC0D);
	//Enable ADC
	ADCSRA |= (1<<ADEN);
	//Enable ADC interrupt
	ADCSRA |= (1<<ADIE);
	//start conversation
	ADCSRA |= (1<<ADSC);
	
}

ISR(ADC_vect)
{
	temp_cel = (int)((5*ADC-500)/10);
	sprintf(String, "temperature in celcuis: %d \n",temp_cel);
	UART_putstring(String);
	
	//a = (PIND & (1<<PIND3));
	//sprintf(String, "buttom press or not: %d \n",a);
	//UART_putstring(String);
	
	if(PIND & (1<<PIND3))
	{
		//sprintf(String, "I am in if \n");
		//UART_putstring(String);
		D = 90;
	}
	else if(PIND & (1<<PIND4))
	{
		D = 33;
		//sprintf(String, "I am in 30 \n");
		//UART_putstring(String);
	}
	else if(PIND & (1<<PIND5))
	{
		D = 5;
		//sprintf(String, "I am in 5 \n");
		//UART_putstring(String);
	}
	else
	{
		//sprintf(String, "I am in else \n");
		//UART_putstring(String);

		if((ADC<=200)&&(ADC>=180))//((temp_cel<=50)&&(temp_cel>=40))
		{
			D=29;
		}
		else if((ADC>200)&&(ADC<=240))
		{
			D=60;
		}
		else if((ADC>240)&&(ADC<=300))
		{
			D=100;
		}
		else
		{
			D=5;
		}
		
	}
	
	
	//D=1;
	OCR0A=(256*D/100 - 1);
	//sprintf(String, "Duty cycle: %d \n",D);
	//UART_putstring(String);
	
}

Initialize()
{
	cli();
	Serial_Ini();
	PWM_Ini();
	ADC_Ini();
	sei();
}

int main(void)
{
	
	Initialize();
	//sprintf(String, "DC1: %d \n,",D);
	//UART_putstring(String);
	while (1)
	{
		//sprintf(String, "Duty cycle: %d \n",D);
		//UART_putstring(String);
		
	}
}

