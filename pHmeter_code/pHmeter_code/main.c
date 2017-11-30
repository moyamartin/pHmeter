/*
 * pHmeter_code.c
 *
 * Created: 01/11/2017 6:14:14 PM
 * Author : Martin Moya
 */ 

 #ifndef F_CPU
 #define F_CPU 16000000UL // 16 MHz Clock Speed
 #endif


#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"

void init_adc();
void adc_single_read(uint16_t * value);
double ph_avg();
void ftoa(double value, char * buffer);

int main(void)
{	
	double ph_result = 0.0;
	char buffer[5];
	DDRD = 0xFF;
	DDRB = 0xFF;
	Lcd8_Init();
	init_adc();
    while (1) 
    {
		Lcd8_Set_Cursor(1,0);
		Lcd8_Write_String("pH-Meter V1.0");
		Lcd8_Set_Cursor(2,0);
		ph_result = ph_avg();
		ftoa(ph_result, buffer);
		Lcd8_Write_String(buffer);
		_delay_ms(10);
    }
}

void ftoa(double value, char * buffer)
{
	int aux_1 = 0, aux_2 = 0;
	char buffer_1[2], buffer_2[2];
	if(value < 10)
	{
		strcpy(buffer, "0");
	}
	aux_1 = (int) value;
	itoa(aux_1, buffer_1, 10);
	itoa(aux_2, buffer_2, 10);
	//strcat(buffer, buffer_1);
	strcat(buffer, ",");
	strcat(buffer, buffer_2);

}



void init_adc()
{
	ADCSRA |= (1<<ADPS2|1<<ADPS1|1<<ADPS0);		//set pre-escaler to 128 -> 125KHz ADC_clock
	ADCSRA |= 1<<ADEN;							//Enables ADC
	ADMUX |= (1<<REFS0 | 1<<REFS1);				//Enables 1V1 Vref
}

/*
ADC Channels
0: ADC 0
1: ADC 1
2: ADC 2
3: ADC 3
4: ADC 4
5: ADC 5
6: ADC 6
7: ADC 7
8: Temperature sensor
14: 1.1V
15: GND
*/
void adc_single_read(uint16_t * result)
{
	ADMUX |= MUX0;
	ADCSRA |= 1<<ADSC;
	while((ADCSRA & 1<<ADSC)){};		
	*result = ADCW;
}

double ph_avg()
{
	uint16_t adc_value = 0, acum = 0;
	for(int i = 0; i<50; i++)
	{
		adc_single_read(&adc_value);
		acum += adc_value;
	}
	return (acum/50.0)*(14.0/1023);
}


