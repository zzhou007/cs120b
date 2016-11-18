/*
 * slope029_zzhou007_lab1_part4.c
 *
 * Created: 3/29/2016 6:24:39 PM
 *  Author: student
 */ 
// steven lopez
// made basic program
// zihang zhou
// debuged and fixed weight of passangers

#include <avr/io.h>
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs,
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs,

	// initialize to 0s
	while(1)
	{
		unsigned char tempA = PINA;
		unsigned char tempB = PINB;
		unsigned char tempC = PINC;
		unsigned char tempD = 0x00;
		
		unsigned short total; 
		unsigned char max;
		unsigned char min;
		unsigned char dif;
		total = tempA + tempB + tempC;
		//PORTD = total;
		/*if(total > 140)
		{
			PORTD = 0x01;
		}*/
		if(total > 255)
		{
			total = 255;
		}
		if(tempA > tempC)
		{
			max = tempA;
			min = tempC;
		}
		else
		{
			max = tempC;
			min = tempA;
		}
		dif = max - min;
		if((dif > 80) && (total > 140))
		{
			tempD = 0x03;
		}
		else if((dif > 80) && (!(total > 140)))
		{
			tempD = 0x02;
		}
		else if((!(dif > 80)) && (total > 140))
		{
			tempD = 0x01;
		}
		else{
			tempD = 0x00;
		}
		total = total >> 2;
		
		PORTD = tempD | (total << 2);
		
	}
	
	
	return 0;
}