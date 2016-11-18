/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab1_part3.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
// steven lopez
// Fixed bitmask and conditions
// zihang zhou
// made basic program

#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,

	// initialize to 0s
	while(1)
	{
		unsigned char parking = PINA;
		unsigned char open = 4;
		unsigned char allfull = 0; 
		// if PA0 is 1 and PA1 is 1
		if (parking & 0x01) // True if PA0 is 1   
			--open; 
		if (parking & 0x02)
			--open;
		if (parking & 0x04)
			--open;
		if (parking & 0x08)
			--open;
		if (open == 0)
			allfull = 0x80;
		PORTC = open | allfull;
	}	
	return 0;
}