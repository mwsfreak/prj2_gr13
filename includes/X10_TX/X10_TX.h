/* 
* X10_TX.h
*
* Created: 13-05-2020 22:01:26
* Author: Magnus Bisgård Franks
*/

#pragma once

class X10_TX
{
public:
	X10_TX();
	bool transmit(char address, char command);	//sætter startsekvens, adresse, kommando og stopsekvens ind i burstTrain
	
	void x10_tx_interrupt();

private:
	bool burstTrain_[16];
	int burstIndex_;	
	bool dataReady_;
};



