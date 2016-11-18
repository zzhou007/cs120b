/* steven lopez sloope029@ucr.edu
 * zihang zhou zzhou007@ucr.edu
 * Lab section 22
 * slope029_zzhou007_lab7_part3.c
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"


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



enum StatesControl {cstart, wait, pressed} cs;
enum StatesPlay {pstart, on, off} ps;

unsigned short ap = 0;
unsigned char playing = 0;
unsigned short timer = 0;
double freq[13] = {329.63, 293.66, 261.63, 293.66, 329.63, 329.63, 329.63, 293.66, 293.66, 293.66, 329.63, 392, 392};
void play(){
	switch(ps){ //transition
		case pstart:
			ps = off ; break;
		case off:
			ap = 0;
			timer = 0;
			if(playing)
				ps = on;
			break;
		case on:
			if(!playing){
				ps = off;
			}
			break;
		default:
			break;
	}
	switch(ps){//action
		case on:
			if(timer >= 75){
				set_PWM(freq[ap]);
				if (timer >= 80)
					playing = 0;				
			}else if (timer >= 70){
				set_PWM(freq[ap]);
				if (timer == 64)
					ap++;		
				playing = 0;		
			}else if (timer >= 55){
				set_PWM(freq[ap]);
				if (timer == 69)
					ap++;				
			}else if (timer >= 50){
				set_PWM(freq[ap]);
				if (timer == 54)
					ap++;				
			}else if (timer >= 45){
				set_PWM(freq[ap]);
				if (timer == 49)
					ap++;		
			}else if (timer >= 40){
				set_PWM(freq[ap]);
				if (timer == 44)
					ap++;			
			}else if (timer >= 35){
				set_PWM(freq[ap]);
				if (timer == 39)
					ap++;		
			}else if (timer >= 30){
				set_PWM(freq[ap]);
				if (timer == 34)
					ap++;			
			}else if (timer >= 25){
				set_PWM(freq[ap]);
				if (timer == 29)	
					ap++;		
			}else if (timer >= 20){
				set_PWM(freq[ap]);
				if (timer == 24)
					ap++;			
			}else if (timer >= 15){
				set_PWM(freq[ap]);
				if (timer == 19)
					ap++;			
			}else if (timer >= 10){
				set_PWM(freq[ap]);
				if (timer == 14)
					ap++;			
			}else if (timer >= 5){
				set_PWM(freq[ap]);
				if (timer == 9)
					ap++;
				ap++;			
			}else if (timer >= 0){
				set_PWM(freq[ap]);
				if (timer == 4)
					ap++;
			}
			++timer;
			break;
		case off:
			set_PWM(0);
			break;
	}
}
void control(){
	unsigned char A0 = ~PINA & 0x01;
	switch(cs){ //transition
		case cstart:
			cs = wait; break;
		case wait:
			if(A0)
				cs = pressed;
			break;
		case pressed:
			cs = wait;
			break;
		default: break;
	}
	switch(cs){ //action
		case cstart:
			cs = wait; break;
		case wait:
			break;
		case pressed:
			playing = 1;
			break;
		default: break;		
	}	
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(150);
	TimerOn();
	cs = cstart;
	ps = pstart;
	PWM_on();
    /* Replace with your application code */
    while (1) 
    {
		control();
		play();
		while(!TimerFlag){}
		TimerFlag = 0;
    }
	PWM_off();
}

