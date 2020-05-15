/************************************
 * DE2.h
 *
 * Created: 5/13/2020 14:46:53
 *  Author: Allan Lassesen
 *
 ************************************/
#pragma  once
#define  F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

class DE2
{
public:
	DE2();
	char getDE2state() const;
	void setDE2state(char state);
	char getPassword() const;
	void setPassword(char pasword);
	void checkPassword(char input);	
	~DE2();
private:
	char DE2state_;
	char password_;
	char activated_;
	char deactivated_;
	char error_;
};