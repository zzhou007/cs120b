/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab6_part1.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <stdio.h>
#include <avr/io.h>
#include "timer.h"

unsigned char timer = 0;
enum States {start, on, off} state;
unsigned char B4 = 0xFF;
Tick(){
    unsigned char A2 = ~PINA & 0x04;
    switch(state){//transition
        case(start):
            state = off; break;
        case(off):
            if(A2)
                state = on; 
            break;
        case(on):
            if(!A2)
                state = off; 
            break;
        default:
            break;
    }
    switch(state){//action
        case(start):
            break;
        case(on):
            B4 = ~B4; //when on switch on and off
			break;
        case(off):
            B4 = 0x00;
			break;
        default:
            break;
    }
	PORTB = B4 & 0x10; //only change B4
}

void main(){
    DDRB = 0xFF; PORTB = 0x00; //output
    DDRA = 0x00; PORTA = 0xFF; //input
	
    
    state = start;
	
    TimerSet(2);
    TimerOn();

    while(1){
        Tick();
        while(!TimerFlag){}
        TimerFlag = 0;
    }
}