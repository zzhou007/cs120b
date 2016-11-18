/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab3_part1.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>
//#include "RIMS.h"
//during state PB0 PB0 is on and PB1 is off reverse for other state
enum States {Start, pressed0, released0, pressed1, released1} state;
	
void Tick(){
	unsigned char A0 = PINA & 0x01; //A0 is if button is pressed or not
	switch(state){ //Transitions
		case(Start): 
			if (A0)
				state = pressed1; 
			break;
		case(pressed0):
			if (!A0)
				state = released0; 
			break;
		case(released0):
			if (A0)
				state = pressed1;
			break;
		case(pressed1):
			if(!A0)
				state = released1;
			break;
		case(released1):
			if(A0)
				state = pressed0;
			break;
		default:
			break;
	}
	switch(state){ //Action
		case(Start):
			PORTB = 0x01; //PB0 led is on, PB1 led is off
			break;
		case(pressed0):
			PORTB = 0x01;  //every time the button is pressed bits 1 and 0 reverse
			break;
		case(released0):
			break; //nothing happens on button release
		case(pressed1):
			PORTB = 0x02;
			break;
		case(released1):
			break;
		default:
			break;
	}
	//set pinB
 }

int main(){
	//initialize the state machine and start state
	//B = 0x00;
	state = Start;
	//Program start
	DDRA = 0x00; PORTA = 0xFF; //DDRA is input
	DDRB = 0xFF; PORTB = 0x00; //DDRB is output
	while(1) {Tick();}
}
