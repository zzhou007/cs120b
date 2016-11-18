/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab6_part2.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <stdio.h>
#include <avr/io.h>
#include "timer.h"

unsigned char timer = 0;
enum StatesSound {startss, on, off} ss;
enum StatesControl {startsc, incp, incr, decp, decr, wait} sc;

unsigned char B4 = 0xFF;
unsigned char frequency = 0;
void sound(){
    unsigned char A2 = ~PINA & 0x04;
    switch(ss){ //transitions
        case(startss):
            ss = off;
            break;
        case(off):
            if (A2)
                ss = on;
            break;
        case(on):
            if (!A2)
                ss = off;
            break;
        default:
            break;
    }
    switch(ss){ //actions
        case(startsc):
            break;
        case(off):
            PORTB = 0x00; //everything off
            break;
        case(on):
            timer++;
            if (timer >= frequency){ //repeatly switch on and off at the rate of frequency
                timer = 0;
                B4 = ~B4;
            }
            break;
        default:
            break;
    }
	PORTB = B4 & 0x10; // only need B4 to output
}
void control(){
    unsigned char A1 = ~PINA & 0x01;	//messed up reversed functionality of A0 and A1 quick fix
    unsigned char A0 = ~PINA & 0x02;
    switch(sc){ //transitions
        case(startsc):
            sc = wait;
            break;
        case(wait):
            if (A0)
                sc = incp;
            else if (A1)
                sc = decp;
            break;
        case(incp):
            if (!A0)
                sc = incr;
            break;
        case(incr):
            sc = wait;
            break;
        case(decp):
            if (!A1)
                sc = decr;
            break;
        case(decr):
            sc = wait;
            break;
        default:
            break;
    }
    //changes the frequency upon the button release
    switch(sc){ //actions
        case(startsc): 
            break;
        case(incp):
            break;
        case(incr):
			if (frequency <= 8)
				frequency++;
            break;
        case(decp):
            break;
        case(decr):
			if (frequency >= 1)
				frequency--;
            break;
        default:
            break;
    }
}

void main(){
    DDRA = 0x00; PORTA = 0xFF; //input
    DDRB = 0xFF; PORTB = 0x00; //output
    
    ss = startss;
    sc = startsc;
    TimerSet(1);
    TimerOn();
    
    while(1){
        sound();
        control();
        while(!TimerFlag) {}
        TimerFlag = 0;
    }
    
}