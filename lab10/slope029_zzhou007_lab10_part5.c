/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab10_part5.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/

#include <avr/io.h>
#include "io.h"
#include "timer.h"
#include "bit.h"
//#include "keypad.h"

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

//shared variables
static pos1 = 6;
static pos2 = 26;
static player = 1;
static finished = 0;
//unsigned short pos1 = 6;
//unsigned short pos2 = 26;
unsigned char keypadout = 'q';


//

//pos 0 is nothing
//dodger board
int board[2][17];
//sm1 program
//keypad
// Permission to copy is granted provided that this header remains intact.
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
// Keypad arrangement
//        Px4 Px5 Px6 Px7
//	  col 1   2   3   4
//  row  ______________
//Px0 1	| 1 | 2 | 3 | A
//Px1 2	| 4 | 5 | 6 | B
//Px2 3	| 7 | 8 | 9 | C
//Px3 4	| * | 0 | # | D
// Keypad Setup Values
#define KEYPADPORT PORTC
#define KEYPADPIN  PINC
#define ROW1 0
#define ROW2 1
#define ROW3 2
#define ROW4 3
#define COL1 4
#define COL2 5
#define COL3 6
#define COL4 7

enum sm1_States{sm1start, sm1set};
int sm1tick(int state){
	switch (state) { //transition
		case -1:
			state = sm1set;
			break;
		default:
			break;
	}
	switch (state) { //action
		case sm1set:
			
			// Check keys in col 1
			KEYPADPORT = SetBit(0xFF,COL1,0); // Set Px4 to 0; others 1
			asm("nop"); // add a delay to allow PORTx to stabilize before checking
			if ( GetBit(~KEYPADPIN,ROW1) ) { keypadout =  '1'; break; }
			if ( GetBit(~KEYPADPIN,ROW2) ) { keypadout =  '4'; break; }
			if ( GetBit(~KEYPADPIN,ROW3) ) { keypadout =  '7'; break; }
			if ( GetBit(~KEYPADPIN,ROW4) ) { keypadout =  '*'; break; }
						
			// Check keys in col 2
			KEYPADPORT = SetBit(0xFF,COL2,0); // Set Px5 to 0; others 1
			asm("nop"); // add a delay to allow PORTx to stabilize before checking
			if ( GetBit(~KEYPADPIN,ROW1) ) { keypadout =  '2'; break; }
			if ( GetBit(~KEYPADPIN,ROW2) ) { keypadout =  '5'; break; }
			if ( GetBit(~KEYPADPIN,ROW3) ) { keypadout =  '8'; break; }
			if ( GetBit(~KEYPADPIN,ROW4) ) { keypadout =  '0'; break; }

			// Check keys in col 3
			KEYPADPORT = SetBit(0xFF,COL3,0); // Set Px6 to 0; others 1
			asm("nop"); // add a delay to allow PORTx to stabilize before checking
			if ( GetBit(~KEYPADPIN,ROW1) ) { keypadout =  '3'; break; }
			if ( GetBit(~KEYPADPIN,ROW2) ) { keypadout =  '6'; break; }
			if ( GetBit(~KEYPADPIN,ROW3) ) { keypadout =  '9'; break; }
			if ( GetBit(~KEYPADPIN,ROW4) ) { keypadout =  '#'; break; }

			// Check keys in col 4
			KEYPADPORT = SetBit(0xFF,COL4,0); // Set Px7 to 0; others 1
			asm("nop"); // add a delay to allow PORTx to stabilize before checking
			if (GetBit(~KEYPADPIN,ROW1) ) { keypadout =  'A'; break; }
			if (GetBit(~KEYPADPIN,ROW2) ) { keypadout =  'B'; break; }
			if (GetBit(~KEYPADPIN,ROW3) ) { keypadout =  'C'; break; }
			if (GetBit(~KEYPADPIN,ROW4) ) { keypadout =  'D'; break; }
			
			keypadout =  '\0';
			break;
		
		default:
			break;

	}
	return state;
}

//sm2 display 
enum sm2_States{sm2wait, sm2display};
int sm2tick(int state){
    switch (state) { //transition 
		case -1:
			state = sm2wait;
			break;
		case sm2wait:
			state = sm2display;
			break;
		default:
			break;
    }
    
    switch (state) { //action
		int i;
		case sm2wait:
			for(i = 1; i <= 32; i++){
				LCD_Cursor(i);
				//string->data
				board[1][i] = ' ';
				{LCD_WriteData(board[1][i]);}
			}
			break;
		i = 0;
		case sm2display:
			/*for(i = 1; i <= 16; i++){
				LCD_Cursor(i);
				//string->data
				LCD_WriteData(board[0][i]);
			}*/
			for(i = 1; i <= 32; i++){
				LCD_Cursor(i);
				//string->data
				LCD_WriteData(board[1][i]);
			}
			break;
		default:
			break;
    }
	return state;
}

enum sm3_States{sm3wait, sm3start, sm3init, sm3motion, sm3check};
int sm3tick(int state){
	static timer = 0;
	unsigned char A0 = ~PINA & 0x10;
	unsigned char A1 = ~PINA & 0x20;
	unsigned char A2 = ~PINA & 0x40;
	switch (state) {//transition
		case -1:
			state = sm3wait;
			break;
			case sm3wait:
				if(finished == 1)
				{
					LCD_DisplayString(1, "GAME OVER, A0 to restart");
				}
				if(A0)
				{
					finished = 0;
					state = sm3start;
				}
				else
				{
					state = sm3wait;
				}
				break;
		case sm3start:
			state = sm3init;
			break;
		case sm3init:
			state = sm3motion;
			break;
		case sm3motion:
			timer++;
			//if (timer >= 5) {
				state = sm3check;
				timer = 0;
			//}
			break;
		case sm3check:
			if(finished == 1)
			{
				//finished = 0;
				state = sm3wait;
			}
			else
			{
				finished = 0;
				state = sm3init;
			}
			break;
	}
	switch (state) {//action
		case sm3wait:
			pos1 = 6;
			pos2 = 26;
			player = 1;
			int i;
			//EMPTY BOARD//
			
			//empty board//
			//board[1][10] = 'D';
			board[1][player] = 'X';
			board[1][pos1] = '#';
			board[1][pos2] = '#';
			if(!finished){
			for(i = 1; i <= 32; i++){
				LCD_Cursor(i);
				//string->data
				if(board[1][i] != '\0')
				{LCD_WriteData(board[1][i]);}
			}
			}
			break;
			//clearscreen;
			break;
		case sm3start:
			break;
		case sm3init:
			//LCD_ClearScreen();
			
			board[1][player] = 'X';
			board[1][pos1] = '#';
			board[1][pos2] = '#';
			break;
		case sm3motion:
			//LCD_ClearScreen();
			board[1][pos1] = ' ';
			board[1][pos2] = ' ';
			if(pos1 <= 1){pos1 = 17;}
			--pos1;
			if(pos2 <= 17){pos2 = 33;}
			--pos2;
			if(A1)
			{
				if(player <= 2)
				{
					board[1][player] = ' ';
					player +=16;

				}
			}
			if(A2)
			{
				if(player >= 15)
				{
					board[1][player] = ' ';
					player -=16;
				}
			}
			//check collision of # with cursor pos
			if(player == pos1)
			{
				finished = 1;
				A0 = 0;
			}
			if(player == pos2)
			{
				board[1][player] = ' ';
				finished = 1;
				A0 = 0;
			}
			break;
		case sm3check:
			break;
	}
	return state;
}

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
int main(void)
{
	//input output 
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRA = 0x0F; PORTA = 0xF0;
	DDRD = 0xFF; PORTD = 0x00;
	
	
	//buttons
	//unsigned char A0 = ~PINA & 0x10;
	//period for tasks  
	//change if adding new
	unsigned long int sm1 = 200;
	unsigned long int sm2 = 200;
	unsigned long int sm3 = 200;
	unsigned long int sm4 = 200;
	
	//calculating gcd   
	//change if adding new
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(sm1, sm2);
	tmpGCD = findGCD(tmpGCD, sm3);
	tmpGCD = findGCD(tmpGCD, sm4);
	
	//setting gcd
	unsigned long int gcd = tmpGCD;
	
	//recalculating period with gcd 
	//change if adding new
	unsigned long int sm1_period = sm1/gcd;
	unsigned long int sm2_period = sm2/gcd;
	unsigned long int sm3_period = sm3/gcd;
	//unsigned long int sm4_period = sm4/gcd;
	
	//declare array o tasks 
	//change if adding new
	static task task1, task2, task3;//,task4;
	task *tasks[] = {&task1, &task2, &task3};//, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//task 1
	task1.state = -1;
	task1.period = sm1_period;
	task1.elapsedTime = sm1_period;
	task1.TickFct = &sm1tick;
	
	//task 2
	task2.state = -1;
	task2.period = sm2_period;
	task2.elapsedTime = sm2_period;
	task2.TickFct = &sm2tick;
	
	//task 3
	task3.state = -1;
	task3.period = sm3_period;
	task3.elapsedTime = sm3_period;
	task3.TickFct = &sm3tick;
	
	//task 4
	//task4.state = -1;
	//task4.period = sm4_period;
	//task4.elapsedTime = sm4_period;
	//task4.TickFct = &sm4tick;
	//init
	
	LCD_init();
	TimerSet(gcd);
	TimerOn();
	unsigned short i;
	
	//run
	while(1){
		//if(finished == 1){while(!A0){}}
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

//keypad testing section 
//include original keypad c code
/*
int main(){
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(100);
	TimerOn();
	LCD_init();
	while(1){
		unsigned char x = GetKeypadKey();
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData(x);
		while (!TimerFlag){}
			TimerFlag = 0;
	}
	return 0;
}
*/
