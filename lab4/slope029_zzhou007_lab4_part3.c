/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab4_part3.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>
//#include "RIMS.h"
//during state PB0 PB0 is on and PB1 is off reverse for other state
enum States {Start, wait, outer1wait, outer2wait, outer3wait, outer4wait, outter1wait, outter2wait, outter3wait, outer1, outer2, outer3, outer4, outter1, outter2, outter3, off, offwait} state;
	
unsigned char count = 0;
	
void Tick(){
	unsigned char A0 = ~PINA & 0x01; //A0 is 1 if button is pressed
	//unsigned char A1 = ~PINA & 0x02; //A1 is 1 if button is pressed
	switch(state){ //Transitions
		case(Start): 
			state = wait; break;
		case(wait):
			if(A0)
				state = outer1wait;
			else
				state = wait; break;
		case(outer1wait):
			if(!A0)
				state = outer1;
			break;
		case(outer1):
			if(A0)
				state = outer2wait;
			break;
		case(outer2wait):
			if(!A0)
				state = outer2;
			break;
		case(outer2):
			if(A0)
				state = outer3wait;
			break;
		case(outer3wait):
			if(!A0)
				state = outer3;
			break;
		case(outer3):
			if(A0)
				state = outer4wait;
			break;
		case(outer4wait):
			if(!A0)
				state = outer4;
			break;
		case(outer4):
			if(A0)
				state = outter3wait;
			break;
		case(outter3wait):
			if(!A0)
				state = outter3;
			break;
		case(outter3):
			if(A0)
				state = outter2wait;
			break;
		case(outter2wait):
			if(!A0)
				state = outter2;
			break;
		case(outter2):
			if(A0)
				state = outter1wait;
			break;
		case(outter1wait):
			if(!A0)
				state = outter1;
			break;
		case(outter1):
			if(A0)
				state = off;
			break;
		case(off):
			if(!A0)
				state = offwait; break;
		case(offwait):
			if(A0)
				state = Start; break;
			
		default:
			break;
	}
	switch(state){ //Action
		case(Start):
			count = 0x00;
			break;
		case(wait):
			count = 0x00;
			break;
		case(outer1):
			count = 0x41; break;
		case(outer2):
			count = 0x63; break;
		case(outer3):
			count = 0x77; break;
		case(outer4):
			count = 0x7F; break;
		case(outer1wait):
			count = 0x41; break;
		case(outer2wait):
			count = 0x63; break;
		case(outer3wait):
			count = 0x77; break;
		case(outer4wait):
			count = 0x7F; break;
		case(outter1):
			count = 0x41; break;
		case(outter2):
			count = 0x63; break;
		case(outter3):
			count = 0x77; break;
		case(outter1wait):
			count = 0x41; break;
		case(outter2wait):
			count = 0x63; break;
		case(outter3wait):
			count = 0x77; break;
		case(off):
			count = 0x00; break;
		case(offwait):
			count = 0x00; break;
		default:
			break;
	}
	//set PORT
	PORTC = count;
 }

int main(){
	//initialize the state machine and start state
	//B = 0x00;
	state = Start;
	//Program start
	DDRA = 0x00; PORTA = 0xFF; //DDRA is input
	DDRC = 0xFF; PORTC = 0x00; //DDRB is output
	while(1) {Tick();}
}
