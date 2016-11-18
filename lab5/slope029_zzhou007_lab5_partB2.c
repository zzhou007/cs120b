/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab5_partB2c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/




#include <avr/io.h>
#include "io.h"
#include "timer.h"



enum States {Start, s1, s2, s3, pressed, released, go, winner} state;

unsigned char count = 0;
unsigned char temp = 0;
unsigned char score = 5;
unsigned char flag = 0;

void Tick(){
	count ++;
	unsigned char A0 = ~PINA & 0x01;
	switch(state){ //Transitions
		case(Start):
			state = s1;
			break;
		case(s1):
		if(A0){					//pressed wrong
			if(score > 0)
				score--;
			temp = 1;
			state = pressed;
		}
		else {
			if (count >= 10) {	//not pressed
				count = 0;		
				state = s2;
			}
		}
		break;
		case(s2):
		if(A0){
				if(score < 8)//right
				{
					score++;
				}
				else
				{
					//score = 10;
					state = winner;
					LCD_DisplayString(1,"Winner !!!");
					flag = 1;
					score = 5;
				}
			
			temp = 2;
			state = pressed;
		}
		else {
			if (count >= 10) {
				count = 0;
				state = s3;
			}
		}
		break;
		case(s3):
		if(A0){
			if(score > 0)
				score--;
			state = pressed;
			temp = 3;
		}
		else{
			if (count >= 10) {
				count = 0;
				state = s1;
			}
		}
		break;
		case(pressed):
		if(!A0){
			state = released;
		}
		break;
		case(released):
				if (A0)
				state = go;
			break;
		case(go):
			count = 0;
			if (!A0)
			{
				if (temp == 1) {
					state = s1;
				}
				if (temp == 2 ) {
					state = s2;
				}
				if (temp == 3) {
					state = s3;
				}
			}
			break;
		case(winner):
			if (A0) {
				state = pressed;
				score = 5;
			}
			flag = 1; //wnner flag
			break;
		default:
			break;
	}
	switch(state){ //Action
		case(Start):
			break;
		case(s1):
			flag = 0;
			PORTB = 0x01;
			break;
		case(s2):
			PORTB = 0x02;
			break;
		case(s3):
			flag = 0;
		PORTB = 0x04;
			break;
		case(pressed):
			break;
		case(released):
			break;
		case(go):
			break;
		default:
			break;
	}
	if (!flag) {
	//set pinB
	LCD_ClearScreen();
	LCD_Cursor(0);
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_WriteData(score + '0');
	}
	if (flag)
		LCD_DisplayString(1, "Winner!!!");
}


int main()
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	state = Start;
	LCD_init();
	
	TimerSet(30);
	TimerOn();
	while(1){
		Tick();
		while (!TimerFlag); // Wait 1 sec
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
	return 0;
}