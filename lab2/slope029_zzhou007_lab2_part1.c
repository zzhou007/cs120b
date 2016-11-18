/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab2_part1.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
// steven lopez
// came up with logic
// zihang zhou
// crated for loops
#include <avr/io.h>
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; //B is input
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,

	// initialize to 0s
	while(1)
	{
		unsigned char tempA = PINA;
		unsigned char tempB = PINB;
		unsigned char total = 0;
		
		for(unsigned char i = 0; i <= 7; ++i) {
			if (GetBit(tempA, i)) {
				++total;
			}
		}
		for (unsigned char i = 0; i <= 7; ++i) {
			if (GetBit(tempB, i)) {
				++total;
			}
		}

		PORTC = total;
	}	
	return 0;
}