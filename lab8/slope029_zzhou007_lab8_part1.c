/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab8_part1.c
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
    /* Replace with your application code */
    while (1) 
    {
		unsigned short my_short = ADC;
		unsigned char my_charb = (char)my_short; // my_char = 0xCD
		unsigned my_chard = (char)(my_short >> 4); // my_char = 0xBC
		PORTB = my_charb;
		PORTD = my_chard;
    }
}

