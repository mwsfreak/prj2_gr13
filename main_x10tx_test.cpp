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
	
	while(1) {
		X10TX_obj.transmit(1, 1);
		
		_delay_ms(1000);
		
		
	}

}

ISR(INT0_vect) {
	
	X10TX_obj.x10_tx_interrupt();
}