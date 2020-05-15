/* 
* X10_TX.cpp
*
* Created: 13-05-2020 22:01:26
* Author: Magnus Bisgård Franks
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "X10_TX.h"

#define F_CPU 16000000
#include <util/delay.h>

X10_TX X10TX_obj;

int main(void) {
	
	DDRB |= (1 << 7);
	
	while(1) {
		X10TX_obj.transmit(0b10, 0b0010);
		PORTB |= (1 << 7);
		_delay_ms(1000);
		
		X10TX_obj.transmit(0b10, 0b0001);
		PORTB &= ~(1 << 7);
		_delay_ms(1000);
		
		X10TX_obj.transmit(0b10, 0b1001);
		PORTB |= (1 << 7);
		_delay_ms(1000);
		
		X10TX_obj.transmit(0b10, 0b1101);
		PORTB &= ~(1 << 7);
		_delay_ms(1000);
		
		X10TX_obj.transmit(0b10, 0b1110);
		_delay_ms(1000);
		
		
		
		
		
	}

}

ISR(INT0_vect) {
	
	X10TX_obj.x10_tx_interrupt();
}