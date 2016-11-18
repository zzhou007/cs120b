/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab9_part3.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"
unsigned short timer = 0;
unsigned short rate = 0;
unsigned char num = 0;
unsigned char fp = 1;
unsigned char secs = 0;

enum controlS {startc, inc, dec, reset, wait} cs;
enum outputS {starto, on} os;

void control(){
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;
	
	switch(cs){ //transition
		case startc:
			cs = wait;
			break;
		case wait:
			if(A0)
				cs = inc;
			if(A1)
				cs = dec;
			timer = 0;
			rate = 0;
			fp = 1;
			secs = 0;
			break;
		case inc:
			if (!A0)
				cs = wait;
			if(A1)
				cs = reset;
			break;
		case dec:
			if (!A1)
				cs = wait;
			if (A0)
				cs = reset;
			break;
		case reset:
			num = 0;
			timer = 0;
			rate = 0;
			if (!A0 && !A1)
				cs = wait;
			break;
	}
	switch(cs){ //action
		case inc:
			timer++;
			secs ++;
			if (timer >= 750){
				rate++;
				if(rate >= 100){
					if (num < 9)
						num++;
					rate = 0;
				}
			}
			else
				if (fp){
					if (num < 9)
						num++;
					fp = 0;
				}
				if (secs >= 250){
					secs = 0;
					if (num < 9)
						num ++;
				}
			break;
		case dec:
			timer++;
			secs ++;
			if (timer >= 750){
				rate++;
				if(rate >= 100){
					if (num > 0)
						num--;
					rate = 0;
				}
			}
			else
				if (fp){
					if (num > 0)
						num--;
					fp = 0;
				}
				if (secs >= 250){
					secs = 0;
					if (num > 0)
						num--;
				}
			break;
		
	}
}

void out(){
	switch(os){ //transition
		case starto:
			os = on;
			break;
	}
	switch(os){
		case on:
			PORTB = num;
			break;
	}
}





int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	cs = startc;
	os = starto;
		
	TimerSet(4);
	TimerOn();
	
	for (;;) {
		control();
		out();
		while (!TimerFlag){}
		TimerFlag = 0;
    }
}

