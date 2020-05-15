/* 
* X10_TX.cpp
*
* Created: 13-05-2020 22:01:26
* Author: Magnus Bisgård Franks
*/


#include "X10_TX.h"
#include <avr/interrupt.h>

#define F_CPU 16000000
#include <util/delay.h>

// default constructor
X10_TX::X10_TX() {
	burstIndex_ = 0;
	dataReady_ = 0;
	
	burstTrain_[0] = 1;
	burstTrain_[1] = 1;
	burstTrain_[2] = 1;
	burstTrain_[3] = 0;

	for (int i = 4; i <= 15; i++) {
		burstTrain_[i] = 0;
	}
	
	EICRA = 0b00000001;
	EIMSK = 0b00000001;
	sei();
	
	// PB outputs and all LEDs off
	DDRB = 0xFF;
	PORTB = 0;
	
	// Toggle OC1A on compare match
	// Mode = 4 (CTC)
	// Clock prescaler = 1
	TCCR1A = 0b01000000;
	TCCR1B = 0b00001000;
	
	// Frekvens = 0,5 Hz
	// 120000 = 16000000/(2*1*(1+OCR1A))
	OCR1A =66;
}

bool X10_TX::transmit(char address, char command) {
	
	
	
	if ((command == 0) || (command > 15) || (address > 3)) {
		dataReady_ = 0;
		return false;
	} else {	
		if ((address & 0b10) > 0) {
			burstTrain_[4] = 1;
			burstTrain_[5] = 0;
		} else {
			burstTrain_[4] = 0;
			burstTrain_[5] = 1;
		}
		
		if ((address & 0b01) > 0) {
			burstTrain_[6] = 1;
			burstTrain_[7] = 0;
		} else {
			burstTrain_[6] = 0;
			burstTrain_[7] = 1;
		}
		
		if ((command & 0b1000) > 0) {
			burstTrain_[8] = 1;
			burstTrain_[9] = 0;
		} else {
			burstTrain_[8] = 0;
			burstTrain_[9] = 1;
		}
		
		if ((command & 0b0100) > 0) {
			burstTrain_[10] = 1;
			burstTrain_[11] = 0;
		} else {
			burstTrain_[10] = 0;
			burstTrain_[11] = 1;
		}
		
		if ((command & 0b0010) > 0) {
			burstTrain_[12] = 1;
			burstTrain_[13] = 0;
		} else {
			burstTrain_[12] = 0;
			burstTrain_[13] = 1;
		}
		
		if ((command & 0b0001) > 0) {
			burstTrain_[14] = 1;
			burstTrain_[15] = 0;
		} else {
			burstTrain_[14] = 0;
			burstTrain_[15] = 1;
		}
		
		dataReady_ = 1;
		return true;
	}
	
}
	
	
void X10_TX::x10_tx_interrupt() {		//Interruptrutine for Zerocrossing - Tjek syntaks!
	
	EIMSK = 0b00000000;		//Disable interrupt
							//nulstil evt watchdog timer for interrupt
	
	if (dataReady_ && burstTrain_[burstIndex_]) {

		TCCR1B = 0b00001001;			//aktiver burst (timer output i CTC mode?)

		_delay_us(1000);	//(eller brug en timer? evt. watchdog?)
		
		TCCR1B = 0b00001000;			//deaktiver burst
	}
	
	
if (dataReady_)
{
	if ( burstIndex_ >= 15) {
		burstIndex_ = 0;
		dataReady_ = 0;
	}
	else {
		burstIndex_++;
	}
}
		
	EIFR = 0b00000001;			//Clear interrupt flags
	EIMSK = 0b00000001;			//Enable INT0
}