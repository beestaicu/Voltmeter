/*
 * Voltmeter.c
 *
 * Created: 21/06/2020 16:02:20
 * Author : bee
 */ 


#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART_M328P.h"
#include <stdlib.h>

#include <stdio.h>
float v=0;
uint8_t buffer;
volatile uint8_t RX_flag = 0;

ISR(USART_RX_vect){
	buffer = UDR0;
	RX_flag = 1;
}//end ISR USART

void InitADC()
{
    ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	
}

uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}
int main(void)
{
	DDRC = 0b00000000; // port c0 as analog input
	DDRB = 0b11111111;
	DDRD = 0b00000000; // portd0,1 as input for tx rx
	USART0Init();
	sei();
	PORTB |= (0 << PORTB4); //LED
	InitADC();
	
	
	while(1){
		
		v = ReadADC(0);
		float voltage = v*5/1024;
		char buffer[4];
		dtostrf(voltage,4,2,buffer);
		USART_putstring(buffer);
		USART0SendByte('v');
		USART0SendByte('\n');
		_delay_ms(1000);
		
	}
}
