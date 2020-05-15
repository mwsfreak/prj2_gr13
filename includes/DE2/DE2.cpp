/************************************
 * DE2.cpp
 *
 * Created: 5/13/2020 14:46:53
 *  Author: Allan Lassesen
 *
 ************************************/ 
#define  F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "DE2.h"

 DE2::DE2()
 {
    UCSR1B = 0b10011000;			// enable Tx and Rx, , interrupt enabled
    UCSR1C = 0b00000110;			// 8 bit, no parity, asynchronous, 1 stop bit
    UBRR1 = 103;					// Baudrate 9600 bit/s
	
	// --- LED test
	DDRB = 0xFF; // PortB output
	PORTB = 0;   // LEDs off
	// --- LED test
	// commands 
	activated_ = 0b00000001;		// DE2 command for activated system
	deactivated_ = 0b10000000;		// DE2 command for deactivated system
	error_ = 0b10101010;			// DE2 command for wrong password
	password_ = 0b01010101;
 }

char DE2::getDE2state() const
{
	return DE2state_;
}

void DE2::setDE2state(char state)
{
	DE2state_ = state;
}

char DE2::getPassword() const
{
	return password_;
}

void DE2::setPassword(char pasword)
{
	password_ = pasword;
}

void DE2::checkPassword(char input)
{										// If password passed to DE2 is correct
	if (input == password_)
	{
		while ( (UCSR1A & (1<<5)) == 0 )		// Check for a clear register
		{}
		if (DE2state_ == 'D')					// if current state is Deactivated
		{
			UDR1 = activated_;					// send "activated" as feedback to DE2
			setDE2state('A');					// set state
		}
		else									// if current state is Activated
		{
			UDR1 = deactivated_;				// send "deactivated" as feedback to DE2
			setDE2state('D');					// set state
		}
	}
	else								// If password passed to DE2 is incorrect
	{
		while ( (UCSR1A & (1<<5)) == 0 )		// Check for a clear register
		{}
		UDR1 = error_;							// 0b10101010 == Error
		_delay_ms(3000);						// Error display time
				
		while ( (UCSR1A & (1<<5)) == 0 )		// Check for a clear register
		{}
		if (DE2state_ == 'D')					// current state is Deactivated
		{
			UDR1 = deactivated_;				//  Update DE2 display
		}
		else									// current state is Activated
		{
			UDR1 = activated_;					//  Update DE2 display
		}
	}
}

 DE2::~DE2()
{
}

