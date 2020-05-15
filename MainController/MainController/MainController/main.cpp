/******************************************
 * PRJ2 Main Controller
 *
 * Created: 3/10/2020 19:56:55
 * Author : Allan Lassesen
 *****************************************/ 

#define  F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "DE2.h"
#include "PC.h"
#include "X10_TX.h"

static volatile char state = 'I';	// Arduino state options:		'I' -> Idle 		'D' -> DE2 communication		'P' -> PC communication 
X10_TX X10TX_obj;
int main()
{
	sei();				// Enable Interrupt
	DE2 DE2board;		// DE-2 Board => UART 1
	PC myPC;			// PC		  => UART 0
	PORTB = 0b11100111;
	char adress;
	char command;
	
	while(1)
	{
		PORTB = DE2board.getPassword();
		if (state == 'D')							// DE-2 Board Communication
		{	
			cli();											// Disable Interrupt

			int rxData = UDR1;								// Password attempt from DE2-board
			DE2board.checkPassword(rxData);					// Check Password
			PORTB = rxData; 								// --- LED test ---
			state = 'I';									// return to Idle
			sei();											// Enable Interrupt
		}	
		if (state == 'P')							// PC Communication
		{
			cli();											// Disable Interrupt

			char choice = UDR0;								// Operation choice PC
			if ( myPC.UnitTestRequest(choice) )				// Unit Test Operation
			{
				
				myPC.unitTest(adress, command);	
		sei();
				X10TX_obj.transmit(adress, command);
										// Perform Unit test
			}	
			if ( myPC.passwordRequest(choice) )				// Change Password Operation
			{
				char newPassword = myPC.changePassword();	// Get a new Password	
				DE2board.setPassword(newPassword);			// Change DE-2 Password
				myPC.sendFeedback(newPassword);				// Confirm the new password
			}
			state = 'I';									// return to Idle
			sei();											// Enable Interrupt
		}
	}
return 0;
}


ISR(USART1_RX_vect) 	// DE-2 Board
{
	state = 'D';
	PORTB = 0b00011000; 		
}

ISR(USART0_RX_vect) 	// PC
{
	state = 'P';
	PORTB = 0b00111100;
}

ISR(INT0_vect) {
	
	X10TX_obj.x10_tx_interrupt();
}