/*
 * X10_RX.cpp
 *
 * Created: 14-05-2020 11:11:10
 *  Author: HansK
 */ 
#include "X10_RX.h"
#include <avr/io.h>

X10_RX::X10_RX(int address)
{
	EICRA = 0b00000001; //Interrupt 0 aktiveres på rising edge og falling edge
	EIMSK = 0b00000001; // Interrupt 0 enables
	DDRH &= ~(1<<5); // PORTH5 (pin 8) sættes til at være input 
	
	bitCnt_ = 0;
	receivingData_ = false;

	// rawData initialiseres
	for (int i = 0; i < 8; i++)
	{
		rawData_[i] = false;
	}
	// convertedData initialiseres
	for (int i = 0; i < 4; i++)
	{
		convertedData_[i] = false;
	}
	// startBuffer initialiseres
	for (int i = 0; i < 8; i++)
	{
		startBuffer_[i] = false;
	}
	
	// start delen startArray initialiseres
	startArray_[0] = 1;
	startArray_[1] = 1;
	startArray_[2] = 1;
	startArray_[3] = 0;
	
	// adresse delen startArray initialiseres
	if (address == 1)
	{
		startArray_[4] = 0;
		startArray_[5] = 1;
		startArray_[6] = 0;
		startArray_[7] = 1;
	}
	else if (address == 2)
	{
		startArray_[4] = 0;
		startArray_[5] = 1;
		startArray_[6] = 1;
		startArray_[7] = 0;
	}
	else if (address == 3)
	{
		startArray_[4] = 1;
		startArray_[5] = 0;
		startArray_[6] = 0;
		startArray_[7] = 1;
	}
	else if (address == 4)
	{
		startArray_[4] = 1;
		startArray_[5] = 0;
		startArray_[6] = 1;
		startArray_[7] = 0;
	}
	return;
}

bool X10_RX::dataReady(bool dataBit)
{
	newDataReady_ = false;
	
	if (receivingData_ == false)
	{
		// Forskyd startBuffer 1 til højre
		for (int i = 0; i < 7; i++)
		{
			startBuffer_[i] = startBuffer_[i + 1];
		}
		
		// Indsæt det databit som er modtaget på den bagerste plads
		startBuffer_[7] = dataBit;

		// Kontroller om arrayet er = startArray
		for (int i = 0; i < 8; i++)
		{
			if (startBuffer_[i] == startArray_[i])
			{
				receivingData_ = true;
			}
			else
			{
				receivingData_ = false;
				break;
			}
		}
	}

	else if (receivingData_ == true )
	{
		if (bitCnt_ < 7)
		{
			rawData_[bitCnt_] = dataBit;
			bitCnt_++;
		}
		else if (bitCnt_ == 7)
		{
			rawData_[bitCnt_] = dataBit;
			newDataReady_ = convertData();
			receivingData_ = false;
			bitCnt_ = 0;
		}
		else
		{
			receivingData_ = false;
			bitCnt_ = 0;
		}
		
	}
	return newDataReady_;
}

bool X10_RX::convertData()
{
	// convert databit 0:
	if (rawData_[0] == 0 && rawData_[1] == 1)
	{
		convertedDataChar_ &= ~(1<<3);
	}
	else if (rawData_[0] == 1 && rawData_[1] == 0)
	{
		convertedDataChar_ |= (1<<3);
	}
	else
	{
		return 0;
	}

	// convert databit 1:
	if (rawData_[2] == 0 && rawData_[3] == 1)
	{
		convertedDataChar_ &= ~(1<<2);
	}
	else if (rawData_[2] == 1 && rawData_[3] == 0)
	{
		convertedDataChar_ |= (1<<2);
	}
	else
	{
		return 0;
	}

	// convert databit 2:
	if (rawData_[4] == 0 && rawData_[5] == 1)
	{
		convertedDataChar_ &= ~(1<<1);
	}
	else if (rawData_[4] == 1 && rawData_[5] == 0)
	{
		convertedDataChar_ |= (1<<1);
	}
	else
	{
		return 0;
	}

	// convert databit 3:
	if (rawData_[6] == 0 && rawData_[7] == 1)
	{
		convertedDataChar_ &= ~(1<<0);
	}
	else if (rawData_[6] == 1 && rawData_[7] == 0)
	{
		convertedDataChar_ |= (1<<0);
	}
	else
	{
		return 0;
	}

	return 1;
}

char X10_RX::getData()
{
	return convertedDataChar_;
}
