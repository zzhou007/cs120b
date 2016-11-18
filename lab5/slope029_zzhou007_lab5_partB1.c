/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab5_partB1.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/

#include <avr/io.h>
#include "io.h"
#include "timer.h"

unsigned char count = 0;
unsigned char timer = 0;
enum States {start, wait, inc, dec, reset} state;

Tick(){
	unsigned char P0 = ~PINA & 0x01;
	unsigned char P1 = ~PINA & 0x02;
	switch(state){
		case(start):
			state = wait; break;
		case(wait):
			if(P0)
				{state = inc;}
			if(P1)
				{state = dec;}
			break;
		case(inc):
			if(!P0)
				{state = wait;}
			if(P1)
				{state = reset;}
			break;
		case(dec):
			if(!P1)
				{state = wait;}
			if(P0)
				{state = reset;}
			break;
		case(reset):
			if((!P1) && (!P0))
				state = wait;
				break;
		default: 
			break;
	}
	switch(state){
		case(inc):
			if (timer == 0 && count < 9) {
				count++;
				timer++;
			}
			timer++;
			if (timer == 100) {
				if(count < 9)
					count++;
					timer = 1;
			}
			//PORTB = 0x01;
			break;
		case(dec):
			if (timer == 0 && count > 0) {
				count--;
				timer++;
			}
			timer++;
			if (timer == 100) {
			if(count > 0)
				count--;
				timer = 1;
			}
			//PORTB = 0x02;
			break;
		case(reset):
			count = 0;
			timer = 0;
			break;
		case(wait):
			timer = 0;
		default:
			break;
	}
	LCD_ClearScreen();
	LCD_Cursor(0);
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_WriteData(count + '0');
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	state = start;
	
	// Initializes the LCD display
	LCD_init();
	
	TimerSet(10);
	TimerOn();
	while(1){
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
	return 0;
}