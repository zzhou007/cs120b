/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab3_part3.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>
//#include "RIMS.h"
//during state PB0 PB0 is on and PB1 is off reverse for other state
enum States {Start, s0, s1, s2, s3, s4} state;
	
unsigned char count = 0;
	
void Tick(){
	unsigned char x = PINA & 0x01; //A0 is 1 if button is pressed
	unsigned char y = PINA & 0x02; //A1 is 1 if button is pressed
	unsigned char pound = PINA & 0x04;
	unsigned char lock = PINA & 0x80;
	unsigned char other = PINA & 0x78;
	unsigned char everything = PINA;
	unsigned char tempB = 0;
	switch(state){ //Transitions
		case(Start):
			state = s0; break;
		case(s0): //press #
			if (pound && !other && !x && !y && !lock) //only # is pressed
				state = s1;
			break;
		case(s1): //release #
			if(!everything) //nothing is pressed
				state = s2;
			break;
		case(s2): //press y
			if(y && !other && !x && !pound && !lock) //only y is pressed
				state = s3;
			break;
		case(s3): //unlocked 
			if(lock && !other && !x && !pound && !y) //only lock is pressed y is also release
				state = s4;
			break;
		case(s4): //locked
			state = s0; break; //go back to original state
		default: break;
	}
	switch(state){ //Action
		case(Start): break;
		case(s0):
			PORTC = 0x00;
			break;	
		case(s1):
			PORTC = 0x01;
			break;
		case(s2):
			PORTC = 0x02;
			break;
		case(s3): 
			tempB = 0x01;
			PORTC = 0x03;
			break;
		case(s4):
			tempB = 0x00; 
			PORTC = 0x04;
			break;
		default: break;
	}
	//set PORT
	PORTB = tempB;
 }

int main(){
	//initialize the state machine and start state
	//B = 0x00;
	state = Start;
	//Program start
	DDRA = 0x00; PORTA = 0xFF; //DDRA is input
	DDRB = 0xFF; PORTB = 0x00; //DDRB is output
	DDRC = 0xFF; PORTC = 0x00; //DDRC is output
	while(1) {Tick();}
}
