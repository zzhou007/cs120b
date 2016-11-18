/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab1_part2.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/

// steven lopez
// fixed conditions and bit mask
// zihang zhou
// made logic structure
#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,

	// initialize to 0s
	while(1)
	{
		unsigned char tempA = PINA;
		unsigned char tempC = 4; 
		// if PA0 is 1 and PA1 is 1
		if (tempA & 0x01) // True if PA0 is 1   
			--tempC; 
		if (tempA & 0x02)
			--tempC;
		if (tempA & 0x04)
			--tempC;
		if (tempA & 0x08)
			--tempC;
		PORTC = tempC;
	}	
	return 0;
}