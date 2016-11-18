/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab2_part2.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
// steven lopez
// added condition
// zihang zhou
// mae if structions
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
		unsigned char fuel = PINA & 0x07;
		unsigned char lights = 0;
		unsigned char low = 0;
		
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
	}	
	return 0;
}