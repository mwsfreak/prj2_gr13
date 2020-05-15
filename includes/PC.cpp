/************************************
 * PC.cpp
 *
 * Created: 5/13/2020 14:46:53
 *  Author: Allan Lassesen
 *
 ************************************/ 
#define  F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PC.h"

// default constructor
PC::PC()
{
    UCSR0B = 0b10011000;			// enable Tx and Rx, , interrupt enabled
    UCSR0C = 0b00000110;			// 8 bit, no parity, asynchronous, 1 stop bit
    UBRR0 = 103;					// Baudrate 9600 bit/s	
	
	// --- LED test
	DDRB = 0xFF; // PortB output
	PORTB = 0;   // LEDs off
	// --- LED test
	
	// command settings
	UnitTest_ = 50;
	changePassword_ = 150;
	exit_ = 255;
} 

void PC::unitTest(char &adress,char &command)
{
		
	
	adress = recieveData();					// set address on Unit
	command = recieveData();			// set desired command
}

char PC::changePassword()
{
	int runLoop = 1;
	while (runLoop)						// Change password loop	
	{
		char PCchoice = recieveData();		// Receive PC option
	
		if (PCchoice == exit_)			 	// Exit from PC
		{									//
			runLoop = 0;					// Idle state
		}
		return PCchoice;
	}
}

char PC::recieveData()
{
		while ( (UCSR0A & (1<<7)) == 0 )	// wait for data
		{}
		char rxData = UDR0;
		return rxData;						// return recieved Data
}

void PC::sendFeedback(char feedback) const
{
	while ( (UCSR0A & (1<<5)) == 0 )	// Wait for clear register
	{ }
	UDR0 = feedback;					// send feedback to PC
}

bool PC::passwordRequest(char command) const
{
	return (changePassword_ == command);
}

bool PC::UnitTestRequest(char command) const
{
	return (UnitTest_ == command);
}

// default destructor
PC::~PC()
{
}
