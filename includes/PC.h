/************************************
 * PC.h
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

class PC
{
public:
	PC();
	void unitTest(char &adress,char &command);
	char changePassword();
	char recieveData();
	void sendFeedback(char feedback) const;
	bool passwordRequest(char command) const;
	bool UnitTestRequest(char command) const;
	~PC();
private:
	char changePassword_;
	char UnitTest_;
	char exit_;
};