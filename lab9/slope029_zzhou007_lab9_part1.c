/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab9_part1.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"
unsigned char blinkled = 0;
unsigned char threeled = 0;

enum statesThree {startthree, onthree} st;
enum statesBlink {startblink, onblink} sb;
enum statesCom {startcom, out} sc;


void three(){
	switch(st){
		case startthree:
			st = onthree;
			break;
		case onthree:
			if(threeled <= 2)
				threeled++;	
			else
				threeled = 1;
			break;
	}
}
void blink(){
	switch(sb){
		case startblink:
			sb = onblink;
			break;
		case onblink:
			blinkled = ~blinkled;
			break;
	}
}
void comb(){
unsigned char outp = 0;
	switch(sc){
		case startcom:
			sc = out;
			break;
		case out:
			if (threeled == 1)
				outp = 0x01;
			if (threeled == 2)
				outp = 0x02;
			if (threeled == 3)
				outp = 0x04;
	
			if (blinkled > 0)
				outp = outp | 0x08;
			else
				outp = outp & 0xF7;
			break;
	}
	PORTB = outp;
}

int main(void)
{
    
    DDRB = 0xFF; PORTB = 0x00;
	st = startthree;
	sc = startcom;
	sb = startblink;
	
	TimerSet(1000);
	TimerOn();
	
	while (1) 
    {
		blink();
		three();
		comb();
		while (!TimerFlag){}
		TimerFlag = 0;
    }
}

