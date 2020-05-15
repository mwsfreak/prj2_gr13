/*
 * lampDriver.cpp
 *
 * Created: 14-05-2020 11:21:53
 *  Author: HansK
 */ 
#include "lampDriver.h"
#include <avr/io.h>

lampDriver::lampDriver()
{
	DDRH |= 1<<4;
	PORTH &= ~(1<<4);
	
	return;
}

void lampDriver::turnOnLED()
{
	PORTH |= 1<<4;
	
	return;
}

void lampDriver::turnOffLED()
{
	PORTH &= ~(1<<4);
	
	return;
}