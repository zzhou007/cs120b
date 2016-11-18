/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab5_partA2.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer
//should clear to 0.
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.

unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;	// bit3 = 0: CTC mode (clear timer on compare)
					// bit2bit1bit0=011: pre-scaler /64
					// 00001011: 0x0B
					// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
					// Thus, TCNT1 register will count at 125,000 ticks/s
					// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
					// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
					// So when TCNT1 register equals 125,
					// 1 ms has passed. Thus, we compare to 125.
					// AVR timer interrupt mask register
	TIMSK1 = 0x02;	// bit1: OCIE1A -- enables compare match interrupt
					//Initialize avr counter
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
					// TimerISR will be called every _avr_timer_cntcurr milliseconds
					//Enable global interrupts
					SREG |= 0x80; // 0x80: 1000000
}
void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}
void TimerISR() {
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
	void TimerSet(unsigned long M) {
		_avr_timer_M = M;
		_avr_timer_cntcurr = _avr_timer_M;		
	}




enum States {Start, s1, s2, s3, pressed, released, go} state;

unsigned char count = 0;
unsigned char temp = 0;

void Tick(){
	count ++;
	unsigned char A0 = ~PINA;
	switch(state){ //Transitions
		case(Start):
			state = s1; 
			break;
		case(s1):
			if(A0){
				temp = 1;
				state = pressed; 
			}
			else {
				if (count >= 10) {
					count = 0;
					state = s2;
				}
			}
			break;
		case(s2):
			if(A0){
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
		default:
			break;
	}
	switch(state){ //Action
		case(Start):
			break;
		case(s1):
			PORTB = 0x01; 
			break;
		case(s2):
		PORTB = 0x02;
			break; 
		case(s3):
			PORTB = 0x04;
			break;
		case(released):
			break;
		case(go):
			break;
		default:
			break;
	}
	//set pinB
 }


int main()
{
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
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
