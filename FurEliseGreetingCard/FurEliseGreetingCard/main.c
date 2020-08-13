/*
 * FurEliseGreetingCard.c
 *
 * Created: 2/13/2017 4:32:58 PM
 * Author : Farez Halim and Tamara Oldham
 */


#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#define F_CPU 1000000UL //clock is divided by 8
#include <util/delay.h>
#define PD0 0
#define PB3 3
#define PB2 2

// store song notes in flash memory
const uint8_t melody_voice0[] PROGMEM = {0x5D, 0x63, 0x5D, 0x63, 0x5D, 0x7d, 0x69, 0x76, 0x8D, 0x00, 0xED, 0xBC, 0x8D, 0x7D, 0x00, 0xBC, 0x95, 0x7D, 0x76, 0x00, 0xBC, 0x5D, 0x63, 0x5D, 0x63, 0x5D, 0x7D, 0x69, 0x76, 0x8D, 0x00, 0xED, 0xBC, 0x8D, 0x7D, 0x00, 0xBC, 0x76, 0x7D, 0x8D, 0x5D, 0x63, 0x5D, 0x63, 0x5D, 0x7d, 0x69, 0x76, 0x8D, 0x00};
const uint8_t melody_voice1[] PROGMEM = {0x2E, 0x31, 0x2E, 0x31, 0x2E, 0x3E, 0x34, 0x3A, 0x46, 0x00, 0x76, 0x5D, 0x46, 0x3E, 0x00, 0x5D, 0x4A, 0x3E, 0x3A, 0x00, 0x5D, 0x2E, 0x31, 0x2E, 0x31, 0x2E, 0x3E, 0x34, 0x3A, 0x46, 0x00, 0x76, 0x5D, 0x46, 0x3E, 0x00, 0x5D, 0x3A, 0x3E, 0x46, 0x2E, 0x31, 0x2E, 0x31, 0x2E, 0x3E, 0x34, 0x3A, 0x46, 0x00};
const uint8_t delays[] PROGMEM = {1,1,1,1,1,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,2,1,1,1,1,4,1,1,1,1,1,1,1,1,2,1};


void my_delay_ms(uint8_t n) {
	while(n--) {
		_delay_ms(300);
	}
}

void play_note(uint8_t compareLow, uint8_t compareHigh, uint8_t delay) {
	// initialize counter
	TCNT0 = 0;
	TCNT1H = 0;
	TCNT1L = 0;

	// set compare values for low and high octave
	OCR0A = compareLow;
	OCR1A = compareHigh;

	my_delay_ms(delay);
}



int main(void) {
	// one input from the button
	DDRD &= ~(1 << PD0); // Makes first pin of PORTD as Input
	PORTD |= (1 << PD0); // activate pull-up resistor

	// two outputs to buzzers
	DDRB |= (1 << PB2); // set PB2 (OC0A) as an output
	DDRB |= (1 << PB3); // set PB3 (OC1A) as an output

  while (1) {

		if ((PIND & 1) == 0) {
			// set timer0 to CTC mode, pre-scaler = 8, toggle on match
			TCCR0A |= (1 << WGM01) | (1 << COM0A0) | (1 << COM0B0);
			TCCR0B |= (1 << WGM02) | (1 << CS01);

			// set timer1 to CTC mode, pre-scaler = 8, toggle on match
			TCCR1A |= (1 << COM1A0);
			TCCR1B |= (1 << WGM12) | (1 << CS11);

			//intialize output compare registers
			OCR0A = 0;
			OCR1A = 0;

			int count = 50; // size of song
			uint8_t voice1;
			uint8_t voice2;
			uint8_t delay;

			// call the play_note function with compare and delay values
			for (int noteCounter = 0; noteCounter < count; noteCounter++) {
				voice1 = pgm_read_byte(&melody_voice0[noteCounter]);
				voice2 = pgm_read_byte(&melody_voice1[noteCounter]);
				delay = pgm_read_byte(&delays[noteCounter]);
				play_note(voice1, voice2, delay);

			}
		} else {
			// turn off timers
			TCCR0A = 0;
			TCCR0B = 0;
			TCCR1A = 0;
			TCCR1B = 0;
			OCR0A = 0;
			OCR1A = 0;
		}
  }
}
