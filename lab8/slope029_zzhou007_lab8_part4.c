/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab_part.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	ADC_init();
	unsigned short max = ADC;
    unsigned short light = ADC;
	/* Replace with your application code */
    while (1) 
   {
		light = ADC;
		
		if (light > max) {
			max = light;
		}
		if (light >= max)
			PORTB = 0xFF;
		else if (light > max/1.25)
			PORTB = 0x7F;
		else if (light > max/1.5)
			PORTB = 0x3F;
		else if (light > max/1.75)
			PORTB = 0x1F;
		else if (light > max/2)
			PORTB = 0x0F;
		else if (light > max/2.25)
			PORTB = 0x07;
		else if (light > max/2.5)
			PORTB = 0x03;
		else if (light > max/2.75)
			PORTB = 0x01;
		else
			PORTB = 0x00;
    }
}

