/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab2_part5.c
 *
 * Created: 3/29/2016 6:24:39 PM
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
// steven lopez
// structure
// zihang zhou
// weight and conditions for branch
#include <avr/io.h>
int main(void)
{
	//DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFE; PORTB = 0x01; // Configure port B's 8 pins as inputs
	//DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs,
	DDRD = 0x00; PORTD = 0xFF; // Configure port D's 8 pins as inputs,

	// initialize to 0s
	while(1)
	{
		//unsigned char tempA = PINA;
		unsigned char tempB = PINB;
		//unsigned char tempC = PINC;
		unsigned char tempD = PIND;
		
		
		
		unsigned short weight = tempD << 1 | (PINB & 0x01); 
		
		if(weight < 70 && weight > 5){
			tempB = 0x04 | tempB;

		}
		else if(weight >= 70){
			tempB = 0x02 | tempB;
			
		}
		else{
			tempB = tempB & 0xF8;
		}
	PORTB = tempB;
	}
	return 0;
}