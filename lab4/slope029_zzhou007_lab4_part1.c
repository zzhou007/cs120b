/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab4_part1.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,

	// initialize to 0s
	while(1)
	{
		unsigned char fuel = ~PINA & 0x0F;
		unsigned char lights = 0;
		unsigned char low = 0;
		if(fuel == 0)
		{
			low = 0x40;
		}
		if (fuel == 2 || fuel == 1) {
			lights = 0x20;
			low = 0x40;
		}
		if (fuel == 3 || fuel == 4) {
			lights = 0x30;
			low = 0x40;
		}
		if (fuel == 5 || fuel == 6) {
			lights = 0x38;
		
		}
		if (fuel == 7 || fuel == 8 || fuel == 9) {
			lights = 0x3C;
		}
		if (fuel == 10 || fuel == 11 || fuel == 12) {
			lights = 0x3E;
		}
		if (fuel ==13 || fuel == 14 || fuel == 15) {
			lights = 0x3F;
		}
		

		PORTC = lights | low;
		//PORTC = PORTC;
	}	
	return 0;
}