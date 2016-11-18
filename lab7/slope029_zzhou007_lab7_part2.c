/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab7_part2.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
*/
#include <avr/io.h>



// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
			if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
			else { TCCR3B |= 0x03; } // resumes/continues timer/counter
			// prevents OCR3A from overflowing, using prescaler 64
			// 0.954 is smallest frequency that will not result in overflow
		
			if (frequency < 0.954) { OCR3A = 0xFFFF; }
			// prevents OCR3A from underflowing, using prescaler 64
			// 31250 is largest frequency that will not result in underflow
			else if (frequency > 31250) { OCR3A = 0x0000; }
			// set OCR3A based on desired frequency
			else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
				
				
			TCNT3 = 0; // resets counter
			current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
		// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		// WGM32: When counter (TCNT3) matches OCR3A, reset counter
		// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}



enum StatesControl {cstart, wait, up, down, pressed} cs;
enum StatesPlay {pstart, on, off} ps;

unsigned short ap = 0;
double freq[8] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};
void play(){
	unsigned char A1 = ~PINA & 0x02;
	switch(ps){ //transition
		case pstart:
			ps = off ; break;
		case off:
			if(A1)
				ps = on;
			break;
		case on:
			if(!A1)
				ps = off;
			break;
		default:
			break;
	}
	switch(ps){//action
		case on:
			set_PWM(freq[ap]);
			break;
		case off:
			set_PWM(0);
			break;
	}
}
void control(){
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A2 = ~PINA & 0x04;
	switch(cs){ //transition
		case cstart:
			cs = wait; break;
		case wait:
			if(A0)
				cs = up;
			else if(A2)
				cs = down;
			break;
		case up:
			cs = pressed;
			break;
		case down:
			cs = pressed;
			break;
		case pressed:
			if(!A0 && !A2)
				cs = wait;
			break;
		default: break;
	}
	switch(cs){ //action
		case cstart:
			cs = wait; break;
		case wait:
			break;
		case up:
			if (ap < 7)
				ap++;
			break;
		case down:
			if (ap > 0)
				ap--;
			break;
		default: break;		
	}	
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	cs = cstart;
	ps = pstart;
	PWM_on();
    /* Replace with your application code */
    while (1) 
    {
		control();
		play();
    }
	PWM_off();
}

