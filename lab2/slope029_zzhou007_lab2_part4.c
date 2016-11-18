/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab2_part4.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
// steven lopez
// variables 
// zihang zhou
// bit shifting
#include <avr/io.h>
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0xFF; // Configure port B as output
	DDRC = 0xFF; PORTC = 0x00; // Configure port c's 8 pins as outputs,

	// initialize to 0s
	while(1)
	{
		unsigned char upper = PINA & 0xF0;
		unsigned char lower = PINA & 0x0F;
		unsigned char tempB = 0;
		unsigned char tempC = 0;
		
		//upper A to lower B
		//lower A to upper C
		tempB = upper >> 4;
		tempC = lower << 4;
		
		PORTB = tempB;
		PORTC = tempC; 
		
	}	
	return 0;
}