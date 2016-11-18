/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab3_part4.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>
//#include "RIMS.h"
//during state PB0 PB0 is on and PB1 is off reverse for other state
enum States {Start, wait, pressed, released, check, reset} state;
unsigned char mem[10];
unsigned char ap = 0;
unsigned char locked = 1;
unsigned char count = 0;
unsigned char wrong = 0;
	
void Tick(){
	unsigned char lock = PINA & 0x80;
	unsigned char everything = PINA;
	switch(state){ //Transitions
		case(Start):
			state = wait; break;
		case(wait):
			if (!locked && lock)
			{
				state = reset;
			}
			if (ap >= 4)
			{
				state = check;
			}
			if(everything) 
			{
				state = pressed;
				mem[ap] = everything;
				ap++;
			}
			break;
		case(pressed):
			if (!everything)
			{
				state = released;
			}
			if (ap == 4)
			{
				state = check;
			}
			if (wrong)
				state = reset;
			break;
		case(released):
			state = check;
			break;
		case(check): 
			if (wrong)
				state = reset;
			else
				state = wait;
			break;
		case(reset):
			state = wait;
			break;
		default: break;
	}
	switch(state){ //Action
		case(Start): break;
		case(wait):
			if(locked && ap != 4)
				PORTB = 0x00;
			if (ap >= 4)
			{
				if (locked) {
					locked = 0;
					PORTB = 0x01;
					wrong = 1;
				}
				else {
					locked = 1;
					PORTB = 0x00;
					wrong = 1;
				}
			}
			break;
		case(pressed):
			if(!locked)
			{
				if(lock)
				{
					locked = 1;
					wrong = 1;
				}
			}
		case(check):
			if (ap == 1)
				if (mem[0] != 0x04)
					wrong = 1;
			if (ap == 2)
				if (mem[1] != 0x01)
					wrong = 1;
			if (ap == 3)
				if (mem[2] != 0x02)
					wrong = 1;
			if (ap == 4)
				if (mem[3] != 0x01)
					wrong = 1;
			break;
		case(reset):
			ap = 0;
			wrong = 0;
			break;
		default: break;
	}
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

