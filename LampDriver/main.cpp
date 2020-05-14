/*
 * X10_RX.cpp
 *
 * Created: 14-05-2020 11:10:36
 * Author : HansK
 */ 
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "X10_RX.h"
#include "lampDriver.h"
char state = 'I'; // I = Idle, R = Receiving

int main(void)
{		
	X10_RX lampReceiver(3);
	lampDriver myLamp;
	bool newDataReady = false; //Fortæller hvornår der er et helt datasæt uden fejl klar
	char dataChar = 0; // char som indeholder det sidst modtagne data
	
	myLamp.turnOnLED();
	_delay_ms(3000);
	myLamp.turnOffLED();
	
	
	sei();				// Enable Interrupt
	
    while (1) 
    {
		// Hvis state er = 'R' vil det sige at der har været et zerocrossing interrupt. 
		// Derfor skal dataReady kaldes
		if (state == 'R')
		{
			_delay_us(400); // vent til midten af burst som er 1ms
			newDataReady = lampReceiver.dataReady(PINH & (1 << 5)); //dataReady() returnerer true hvis der er et helt datasæt uden fejl klar.
			state = 'I';
		}
		
		
		if (newDataReady == true)
		{
			dataChar = lampReceiver.getData();
			
			if (dataChar == (0b00000001))
			{
				myLamp.turnOffLED();
			}
			else if (dataChar == (0b00000010))
			{
				myLamp.turnOnLED();
			}
			
			newDataReady = false;
		}
		
    }
}

ISR(INT0_vect)
{
	state = 'R';
}

