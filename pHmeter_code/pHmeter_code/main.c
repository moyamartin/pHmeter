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

void init_adc();
uint16_t adc_read(uint8_t channel);

int main(void)
{
	DDRB |= 1<<DDB1;
	uint16_t resultado = 0;
    init_adc();
    while (1) 
    {
		resultado = adc_read(1);
		if (resultado > 512){
			PORTB |= 1<<DDB1;
		}
		else
		{
			PORTB &= ~(1<<DDB1);
		}
    }
}
/*
ADC Initialization
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
void init_adc()
{
	ADCSRA |= (1<<ADPS2|1<<ADPS1|1<<ADPS0);		//set pre-escaler to 128 -> 125KHz ADC_clock
	ADCSRA |= 1<<ADEN;							//Enables ADC
}

uint16_t adc_read(uint8_t channel)
{
	ADMUX = channel;
	ADCSRA |= 1<<ADSC;
	while((ADCSRA & 1<<ADSC)){};
	return ADCW;
}
