/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab10_part4.c
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
unsigned char keypadout = 'Z';

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
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	//period for tasks
	unsigned long int sm1 = 200;
	
	//calculating gcd
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(sm1, sm1);
	
	//setting gcd
	unsigned long int gcd = tmpGCD;
	
	//recalculating period with gcd
	unsigned long int sm1_period = sm1/gcd;
	
	//declare array o tasks
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//task 1
	task1.state = -1;
	task1.period = sm1_period;
	task1.elapsedTime = sm1_period;
	task1.TickFct = &sm1tick;
	
	//init
	LCD_init();
	TimerSet(gcd);
	TimerOn();
	unsigned short i;
	unsigned char pos = 1;
	unsigned char size = 16;
	//run
	LCD_DisplayString(1, "CONGRATULATIONS!");
		
	while(1){
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
		//LCD_ClearScreen();
		LCD_Cursor(pos);
		LCD_WriteData(keypadout);
		++pos;
		if(pos >= size)
		{
		    pos = 1;
		}
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

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