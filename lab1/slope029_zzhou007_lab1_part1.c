/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab1_part1.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
// steven lopez
// I helped correct bracing errors
// zihang zhou
// made basic program
#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs,
	//first_pin = PA0;
	//second_pin = PA1;
	// initialize to 0s
	while(1)
	{
		// if PA0 is 1 and PA1 is 1
		if (PINA & 0x01)// True if PA0 is 1  and PA1 is 0
		{
			if(!(PINA & 0x02))
			{
			PORTB = 1;
			}
		}
		else{
			PORTB = 0;
		} 
	}	
	return 0;
}