/*
 * lampDriver.cpp
 *
 * Created: 14-05-2020 11:21:53
 *  Author: HansK
 */ 
#include "dimmedLampDriver.h"
#include <avr/io.h>

dimmedLampDriver::dimmedLampDriver()
{
	//Initialiser timer 4 i 8bit PWM og sluk timeren.
	TCCR2A = (0b10000001);
	TCCR2B = (0b00000011);
	DDRB |= 1<<4;
	PORTB &= ~(1<<4);
	
	dimValue_ = 100;
	OCR2A = 0;
	
	return;
}

void dimmedLampDriver::turnOnLED()
{
	OCR2A = (((double)255*(double)100)/(double)dimValue_);
	return;
}

void dimmedLampDriver::turnOffLED()
{
	OCR2A = 0;
	
	return;
}

void dimmedLampDriver::setDimValue(int dimValue)
{
	if(dimValue >= 0 && dimValue <= 100)
	{
		dimValue_ = dimValue;
		OCR2A = (((double)255*(double)100)/(double)dimValue_);
	}
	
	return;
}