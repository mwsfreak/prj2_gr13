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
	X10_RX lampReceiver(2);
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
			
			if (dataChar == (0b0001))
			{
				myLamp.turnOffLED();
			}
			else if (dataChar == (0b0010))
			{
				myLamp.turnOnLED();
			}
			else if (dataChar == (0b1000))
			{
				myLamp.setDimValue(32); // 255*0,125=32
			}
			else if (dataChar == (0b1001))
			{
				myLamp.setDimValue(64); //255*0,25=64
			}
			else if (dataChar == (0b1010))
			{
				myLamp.setDimValue(96); //255*0,375=96
			}
			else if (dataChar == (0b1011))
			{
				myLamp.setDimValue(128); // 255*0,5=128
			}
			else if (dataChar == (0b1100))
			{
				myLamp.setDimValue(159); // 255*0,625=159
			}
			else if (dataChar == (0b1101))
			{
				myLamp.setDimValue(191); // 255*0,75=191
			}
			else if (dataChar == (0b1110))
			{
				myLamp.setDimValue(223); // 255*0,875=223
			}
			else if (dataChar == (0b1111))
			{
				myLamp.setDimValue(255); //255*1,00=255
			}
			
			newDataReady = false;
		}
		
    }
}

ISR(INT0_vect)
{
	state = 'R';
}

